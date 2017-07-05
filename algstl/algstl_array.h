#ifndef __ALGSTL_ARRAY__
#define __ALGSTL_ARRAY__

//动态数组，基本同stl中vector

#include <iostream>
#include <cassert>
#include "algs_type.h"
#include "algstl_memory.h"
#include "algstl_iterator.h"
#include "algstl_algobase.h"
#include "algsstring.h"
#include "__tostring.h"

namespace algstl
{

template<typename _T, typename _Alloc=Allocator<_T>>
class Array
{
public:
    typedef _T ValueType;
    typedef _T& Reference;
    typedef const _T& ConstReference;
    typedef _Alloc Allocator;
    typedef _T* Iterator;
    typedef const _T* ConstIterator;
    typedef _T* Pointer;
    typedef typename algstl::ReverseIterator<Iterator> ReverseIterator;
    typedef Uint SizeType;

    Array()
    {
        start_ = nullptr;
        end_ = nullptr;
        cap_ = nullptr;
    }

    Array(SizeType n)
    {
        start_ = alloc_.allocate(n);
        end_ = start_ + n;
        cap_ = end_;
    }

    //复制构造函数，显示调用
    explicit Array(const Array &rhs)
    {
        auto s = rhs.size();
        start_ = alloc_.allocate(s);
        uninitialized_copy(rhs.begin(), rhs.end(), start_);
        end_ = start_ + s;
        cap_ = end_;
    }

    algs::String toString() const
    {
        algs::String tmp = algs::toString(size()) + " [";
        SizeType width = tmp.size();
        for (auto it = begin(); it != end(); ++it)
        {
            auto ex = algs::toString(*it);
            tmp += ex;
            if (width > 80) //看看是否可以动态获取到屏幕宽度 TODO
            {
                tmp += "\n";
                width = 0;
            }
            else
            {
                tmp += "    ";
                width += ex.size();
            }
        }

        tmp += "]";

        return tmp;
    }

    Reference operator[](SizeType i)
    {
        assert(i < size());
        return *(start_ + i);
    }

    ConstReference operator[](SizeType i) const
    {
        assert(i < size());
        return *(start_ + i);
    }

    //右值构造函数
    explicit Array(const Array &&rhs)
    {
        start_ = rhs.start_;
        end_ = rhs.end_;
        cap_ = rhs.cap_;
        rhs.start_ = nullptr;
        rhs.end_ = nullptr;
        rhs.cap_ = nullptr;
    }

    template<typename _Comp>
    Void sort(_Comp f);

    Iterator begin()
    {
        return start_;
    }

    Iterator end()
    {
        return end_;
    }

    ConstIterator begin() const
    {
        return start_;
    }

    ConstIterator end() const
    {
        return end_;
    }

    ~Array()
    {
        auto cur = start_;
        while (cur != end_)
        {
            alloc_.deconstruct(cur++);
        }
        alloc_.deallocate(start_, size());
        start_ = nullptr;
        end_ = nullptr;
        cap_ = nullptr;
    }

    SizeType size() const
    {
        return end_ - start_;
    }

    Array<ValueType> &operator=(const Array<ValueType> &rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        //先释放自己
        auto cur = start_;
        while (cur != end_)
        {
            alloc_.deconstruct(cur);
        }
        alloc_.deallocate(start_, size());

        //复制对方
        start_ = alloc_.allocate(rhs.size());
        uninitialized_copy(rhs.begin(), rhs.end(), start_);
        end_ = start_ + rhs.size();
        cap_ = end_;

        return *this;
    }

#if 0
    Array<ValueType> &operator+(const ValueType &rhs)
    {
        if (end_ == cap_)
        {
            SizeType oldsize = size();
            //需要重新分配内存
            SizeType newsize = (size() ? size() : 1) * incr_fact;
            ValueType *tmp = alloc_.allocate(newsize);

            //拷贝数据
            uninitialized_copy(start_, end_, tmp);

            //释放旧的内存
            auto cur = start_;
            while (cur != end_)
            {
                //析构
                alloc_.deconstruct(cur);
                ++cur;
            }
            cur = start_;
            alloc_.deallocate(start_, cap_ - start_);

            //指针指向新的地址
            start_ = tmp;
            end_ = start_ + oldsize;
            cap_ = start_ + newsize;
        }

        alloc_.construct(end_++, rhs);
        return *this;
    }
#endif

    Array<ValueType> &operator+=(const ValueType &rhs)
    {
        if (end_ == cap_)
        {
            SizeType oldsize = size();
            //需要重新分配内存
            SizeType newsize = (size() ? size() : 1) * incr_fact;
            ValueType *tmp = alloc_.allocate(newsize);

            //拷贝数据
            uninitialized_copy(start_, end_, tmp);

            //释放旧的内存
            auto cur = start_;
            while (cur != end_)
            {
                //析构
                alloc_.deconstruct(cur);
                ++cur;
            }
            cur = start_;
            alloc_.deallocate(start_, cap_ - start_);

            //指针指向新的地址
            start_ = tmp;
            end_ = start_ + oldsize;
            cap_ = start_ + newsize;
        }

        alloc_.construct(end_++, rhs);
        return *this;
    }

private:
    ValueType *start_;
    ValueType *end_;
    ValueType *cap_;
    static constexpr Double incr_fact = 2;
    Allocator alloc_;
};

template<typename _T, typename _Alloc>
inline Void swap(Array<_T, _Alloc> &lhs, Array<_T, _Alloc> &rhs)
{
    if (&lhs == &rhs)
    {
        return;
    }

    swap(lhs.start_, rhs.start_);
    swap(lhs.end_, rhs.end_);
    swap(lhs.cap_, rhs.cap_);
}

//先临时写这么一个sort方法
template<typename _T, typename _Alloc> template<typename _Comp>
Void Array<_T, _Alloc>::sort(_Comp f)
{
    if (size() == 0)
    {
        return;
    }
    //略去对f的二元算子检查
    //先用最简单的冒泡吧
    for (auto cur = end_ - 1; cur > start_; --cur)
    {
        for (auto flag = start_; flag < cur; ++flag)
        {
            if (not f(*flag, *(flag + 1)))
            {
                swap(*flag, *(flag + 1));
            }
        }
    }
}

}

#endif
