#ifndef __ALGS_MATH__
#define __ALGS_MATH__

#include "algs_type.h"

namespace algs
{

class AlgsMath
{
public:
    static Double log(Double i);
    inline static Double abs(Double v)
    {
        return v >= 0 ? v : -v;
    }
};

}

#endif
