#include "eai_narray_builtin.h"

EAI_NARRAY_IMPL(ulib_uint)
EAI_NARRAY_IMPL(ulib_int)
EAI_NARRAY_IMPL(ulib_byte)
EAI_NARRAY_IMPL(ulib_float)

EaiNArray_ulib_float deb_eai_narray_get_ulib_float(EaiNArray_ulib_float *na, ulib_uint idx)
{
    EaiNArray_ulib_float item;
    const ulib_uint dims = uvec_count_ulib_uint(&na->shape);
    ulib_uint *shape_data = uvec_data_ulib_uint(&na->shape);
    ulib_uint view_size = 1;
    for(ulib_uint i = 1; i < dims; i++) {
        view_size *= shape_data[i];
    }
    item.shape = uvec_wrap_ulib_uint(&shape_data[1], dims - 1);
    item.storage = &na->storage[idx * view_size];
    return item;
}