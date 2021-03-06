#ifndef __ALGS_STRING__
#define __ALGS_STRING__

#include <cassert>
#include <cstring>
#include <ios>
#include <iostream>
#include "algs_type.h"
#include "algstl_memory.h"

#define NDEBUG

namespace algs
{
template<typename _Alloc = algstl::Allocator<Char>>
class StringBase
{
    template<typename _A>
    friend StringBase<_A> operator+(const Char *lhs, const StringBase<_A> &rhs);
    template<typename _A>
    friend StringBase<_A> operator+(const StringBase<_A> &lhs, const Char *rhs);
    template<typename _A>
    friend StringBase<_A> operator+(const StringBase<_A> &lhs,
                                    const StringBase<_A> &rhs);

    template<typename _A>
    friend std::ostream &operator<<(std::ostream &os,
                                    const StringBase<_A> &rhs);
    template<typename _A>
    friend std::ostream &operator>>(std::ostream &os,
                                    const StringBase<_A> &rhs);

    public:
    //实际存储字符串的缓冲区类
    //维护引用计数等
    template<typename _BufAlloc>
    struct StringBuf
    {
        typedef Uint SizeType;
        typedef _BufAlloc Allocator;
        typedef const Char *Iterator;
        Char *buf_;
        Char *mstart_;
        Char *mend_;
        Char *mcapacity_;
        Int ref_count_ = 0;

        inline SizeType size() const
        {
            return mend_ - mstart_;
        }

        inline Int incr()
        {
            return ++ref_count_;
        }

        inline Int decr()
        {
            return --ref_count_;
        }

        inline SizeType capacity() const
        {
            return mcapacity_ - mstart_;
        }

        ~StringBuf()
        {
            if (buf_)
            {
                buf_allocator.deallocate(buf_, capacity());
            }
        }

        StringBuf(SizeType n = 1)
        {
            n += 8;  //本来8字节圆整只需要加7，但是考虑到cStr方法一定要多留出一个字节的空间放结束符，所以这里多分配点
            n &= SizeType(-1) ^ 0x7;  // 8字节圆整对齐
            buf_       = buf_allocator.allocate(n);
            mstart_    = buf_;
            mend_      = mstart_;
            mcapacity_ = buf_ + n;
        }

        void append(const Char *buf, SizeType n)
        {
            if (!buf)
            {
                return;
            }
            assert(size() + n < capacity());
            assert(buf_);
            mend_ = static_cast<Char *>(mempcpy(mend_, buf, n));
        }

        void fill(SizeType n, Char c)
        {
            assert(n < capacity());
            assert(buf_);
            memset(buf_, c, n);
            mend_ += n;
        }

        private:
        Allocator buf_allocator;
    };

    typedef StringBuf<_Alloc> BufferType;
    typedef typename BufferType::SizeType SizeType;
    typedef typename BufferType::Iterator Iterator;

    Iterator begin() const
    {
        return sbuf_->mstart_;
    }

    Iterator end() const
    {
        return sbuf_->mend_;
    }

    StringBase(Iterator first, Iterator last) : StringBase(first, last - first)
    {}

    StringBase() : sbuf_(nullptr)
    {
        sbuf_ = new BufferType(1);
        assert(sbuf_);  //暂时如此
        sbuf_->append("\0", 1);
        sbuf_->incr();
    }

    StringBase(const StringBase &rhs)
    {
        sbuf_ = rhs.sbuf_;
        sbuf_->incr();  //增加引用计数
    }

    StringBase(const Char *rhs)
    {
        SizeType n = rhs ? strlen(rhs) : 1;
        sbuf_      = new BufferType(n);
        assert(sbuf_);
        sbuf_->append(rhs, n);
        sbuf_->incr();
    }

    StringBase(const Char *rhs, SizeType n)
    {
        sbuf_ = new BufferType(n);
        assert(sbuf_);
        sbuf_->append(rhs, n);
        sbuf_->incr();
    }

    //由n个c组成的字符串
    StringBase(SizeType n, Char c)
    {
        sbuf_ = new BufferType(n);
        assert(sbuf_);
        sbuf_->fill(n, c);
        sbuf_->incr();
    }

    StringBase(SizeType n)
    {
        sbuf_ = new BufferType(n);
        assert(sbuf_);
        sbuf_->incr();
    }

    StringBase &operator=(const StringBase &rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        if (!sbuf_->decr())
        {
            delete sbuf_;
        }

        sbuf_ = rhs.sbuf_;
        sbuf_->incr();
        return *this;
    }

    StringBase &operator=(const Char *rhs)
    {
        if (!sbuf_->decr())
        {
            delete sbuf_;
        }

        SizeType n = rhs ? strlen(rhs) : 1;
        sbuf_      = new BufferType(n);
        assert(sbuf_);
        sbuf_->append(rhs, n);
        sbuf_->incr();
        return *this;
    }

    ~StringBase()
    {
        if (!sbuf_->decr())
        {
            delete sbuf_;
        }
    }

    const Char *cStr() const
    {
        *sbuf_->mend_ = '\0';
        return sbuf_->mstart_;
    }

    const SizeType size() const
    {
        return sbuf_->size();
    }

    StringBase &operator+=(const StringBase &rhs)
    {
        auto p = new BufferType(size() + rhs.size());
        assert(p);
        p->append(cStr(), size());
        p->append(rhs.cStr(), rhs.size());

        if (!sbuf_->decr())
        {
            delete sbuf_;
        }

        sbuf_ = p;
        sbuf_->incr();

        return *this;
    }

    protected:
    void append(const Char *rhs, SizeType n)
    {
        sbuf_->append(rhs, n);
    }

    private:
    BufferType *sbuf_;
};
typedef StringBase<> String;

template<typename _A>
StringBase<_A> operator+(const StringBase<_A> &lhs, const StringBase<_A> &rhs)
{
    StringBase<_A> ret(lhs.size() + rhs.size());
    ret.append(lhs.cStr(), lhs.size());
    ret.append(rhs.cStr(), rhs.size());

    return ret;
}

template<typename _A>
StringBase<_A> operator+(const StringBase<_A> &lhs, const Char *rhs)
{
    auto rhs_size = strlen(rhs);
    StringBase<_A> ret(lhs.size() + rhs_size);
    ret.append(lhs.cStr(), lhs.size());
    ret.append(rhs, rhs_size);

    return ret;
}

template<typename _A>
StringBase<_A> operator+(const Char *lhs, const StringBase<_A> &rhs)
{
    auto lhs_size = strlen(lhs);
    StringBase<_A> ret(lhs_size + rhs.size());
    ret.append(lhs, lhs_size);
    ret.append(rhs.cStr(), rhs.size());

    return ret;
}

template<typename _A>
bool operator==(const StringBase<_A> &lhs, const StringBase<_A> &rhs)
{
    return !strncmp(lhs.cStr(), rhs.cStr(), lhs.size());
}

template<typename _A>
bool operator==(const StringBase<_A> &lhs, const Char *rhs)
{
    return !strncmp(lhs.cStr(), rhs, lhs.size());
}

template<typename _A>
bool operator==(const Char *lhs, const StringBase<_A> &rhs)
{
    return !strncmp(lhs, rhs.cStr(), rhs.size());
}

template<typename _A>
std::ostream &operator<<(std::ostream &os, const StringBase<_A> &rhs)
{
    os << rhs.cStr();
    return os;
}

template<typename _A>
std::istream &operator>>(std::istream &is, StringBase<_A> &rhs)
{
    rhs = "";  //先清空自身数据

    auto buf   = is.rdbuf();
    auto csize = buf->in_avail();  //获取buf中目前可读的字符数

    if (csize == -1)
    {
        return is;
    }

    Char *p = new Char[csize];
    auto q  = 0;
    assert(p);

    auto c = buf->sgetc();

    if (c == EOF)
    {
        is.sync();  //略去后续
        is.clear(is.rdstate() | std::ios_base::eofbit | std::ios_base::failbit);
        return is;
    }

    while (isspace(c))
    {
        buf->snextc();
    }

    while (buf->sgetc() != EOF)
    {
        auto c = buf->sbumpc();
        if (isspace(c))
        {
            break;
        }
        p[q++] = c;
    }

    p[q] = '\0';

    rhs = p;

    delete[] p;

    return is;
}
}

#endif
