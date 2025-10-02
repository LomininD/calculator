#include "min_max.h"


size_t max(size_t a, size_t b)
{
    if (a > b)
        return a;
    return b;
}


size_t min(size_t a, size_t b)
{
    if (a < b)
        return a;
    return b;
}
