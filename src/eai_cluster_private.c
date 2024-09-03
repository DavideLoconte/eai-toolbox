#include <ulib.h>
#include "eai_cluster_private.h"
#include "eai_math.h"

ulib_float eai_distance(EaiNArray(ulib_float)* a, EaiNArray(ulib_float)* b, ulib_uint shape)
{
    return eai_math_sqrt(eai_squared_distance(a, b, shape));
}

ulib_float eai_squared_distance(EaiNArray(ulib_float)* a, EaiNArray(ulib_float)* b, ulib_uint shape)
{
    ulib_float dist = 0;

    for (ulib_uint i = 0; i < shape; i++) {
        ulib_float x1 = a->storage[i];
        ulib_float x2 = b->storage[i];
        dist += (x1 - x2) * (x1 - x2);
    }

    return dist;
}

ulib_uint eai_cluster_get_data_count(EaiNArray(ulib_float) *data)
{
    return uvec_first(ulib_uint, eai_narray_shape(ulib_float, data));
}

ulib_uint eai_cluster_get_data_shape(EaiNArray(ulib_float) *data)
{
    return uvec_last(ulib_uint, eai_narray_shape(ulib_float, data));
}
