//
// Created by DavideLoconte on 20-Nov-23.
//

#include "eai_graph_dev.h"

static inline bool edge_eq(EaiGraphEdge_dev a, EaiGraphEdge_dev b) { return a.to == b.to; }
static inline bool edge_cmp(EaiGraphEdge_dev a, EaiGraphEdge_dev b) { return a.to < b.to; }
static inline bool node_eq(EaiGraphNode_dev a, EaiGraphNode_dev b) { return a == b; }

static inline uvec_ret uvec_resize_EaiGraphNode_dev(UVec_EaiGraphNode_dev *vec, ulib_uint size)
{
    EaiGraphNode_dev *data;
    size = ulib_uint_ceil2(size);
    ulib_byte exp = ((vec)->_s[(sizeof(struct p_uvec_large_EaiGraphNode_dev)) - 1]);
    if(((exp) & (((ulib_byte) 0x80)))) {
        if(((exp) == ((ulib_byte) 0xFE)))
            return UVEC_OK;
        data = (EaiGraphNode_dev *) realloc(vec->_l._data, sizeof(*(vec->_l._data)) * (size));
        if(!data)
            return UVEC_ERR;
    } else {
        data = (EaiGraphNode_dev *) malloc(sizeof(*(data)) * (size));
        if(!data)
            return UVEC_ERR;
        memcpy(data, vec->_s, exp * sizeof(EaiGraphNode_dev));
        vec->_l._count = exp;
    }
    (((vec)->_s[(sizeof(struct p_uvec_large_EaiGraphNode_dev)) -
                1]) = (ulib_byte) (ulib_uint_log2(size) | ((ulib_byte) 0x80)));
    vec->_l._data = data;
    return UVEC_OK;
}
static inline uvec_ret uvec_expand_if_required_EaiGraphNode_dev(UVec_EaiGraphNode_dev *vec)
{
    ulib_uint count = uvec_count_EaiGraphNode_dev(vec);
    return uvec_size_EaiGraphNode_dev(vec) > count ? UVEC_OK :
                                                     uvec_resize_EaiGraphNode_dev(vec, count + 1);
}
__attribute__((__unused__)) uvec_ret uvec_reserve_EaiGraphNode_dev(UVec_EaiGraphNode_dev *vec,
                                                                   ulib_uint size)
{
    return size <= uvec_size_EaiGraphNode_dev(vec) ? UVEC_OK :
                                                     uvec_resize_EaiGraphNode_dev(vec, size);
}
__attribute__((__unused__)) uvec_ret uvec_set_range_EaiGraphNode_dev(UVec_EaiGraphNode_dev *vec,
                                                                     EaiGraphNode_dev const *array,
                                                                     ulib_uint start,
                                                                     ulib_uint n)
{
    if(!n)
        return UVEC_OK;
    ulib_uint const old_c = uvec_count_EaiGraphNode_dev(vec), new_c = start + n;
    if(new_c > old_c) {
        if(uvec_reserve_EaiGraphNode_dev(vec, new_c))
            return UVEC_ERR;
        p_uvec_set_count_EaiGraphNode_dev(vec, new_c);
    }
    EaiGraphNode_dev *data = uvec_data_EaiGraphNode_dev(vec);
    memcpy(&(data[start]), array, n * sizeof(EaiGraphNode_dev));
    return UVEC_OK;
}
__attribute__((__unused__)) uvec_ret uvec_copy_EaiGraphNode_dev(UVec_EaiGraphNode_dev const *src,
                                                                UVec_EaiGraphNode_dev *dest)
{
    ulib_uint count = uvec_count_EaiGraphNode_dev(src);
    uvec_ret ret = uvec_reserve_EaiGraphNode_dev(dest, count);
    if(!ret) {
        memcpy(uvec_data_EaiGraphNode_dev(dest),
               uvec_data_EaiGraphNode_dev(src),
               count * sizeof(EaiGraphNode_dev));
        p_uvec_set_count_EaiGraphNode_dev(dest, count);
    }
    return ret;
}
__attribute__((__unused__)) void uvec_copy_to_array_EaiGraphNode_dev(UVec_EaiGraphNode_dev const
                                                                         *vec,
                                                                     EaiGraphNode_dev array[])
{
    ulib_uint count = uvec_count_EaiGraphNode_dev(vec);
    if(count)
        memcpy(array, uvec_data_EaiGraphNode_dev(vec), count * sizeof(EaiGraphNode_dev));
}
__attribute__((__unused__)) uvec_ret uvec_shrink_EaiGraphNode_dev(UVec_EaiGraphNode_dev *vec)
{
    ulib_byte exp = ((vec)->_s[(sizeof(struct p_uvec_large_EaiGraphNode_dev)) - 1]);
    if(((exp) == ((ulib_byte) 0xFE)))
        return UVEC_OK;
    ulib_uint count = uvec_count_EaiGraphNode_dev(vec);
    if(!count) {
        uvec_deinit_EaiGraphNode_dev(vec);
        return UVEC_OK;
    }
    if(count <= ((sizeof(struct p_uvec_large_EaiGraphNode_dev) - 1) / sizeof(EaiGraphNode_dev))) {
        if((!((exp) & (((ulib_byte) 0x80)))))
            return UVEC_OK;
        EaiGraphNode_dev *old_data = vec->_l._data;
        memcpy(vec->_s, old_data, count * sizeof(EaiGraphNode_dev));
        free(old_data);
        (((vec)->_s[(sizeof(struct p_uvec_large_EaiGraphNode_dev)) - 1]) = (ulib_byte) (count));
    } else if(!((exp) == ((ulib_byte) 0xFF))) {
        EaiGraphNode_dev *data = (EaiGraphNode_dev *) realloc(vec->_l._data,
                                                              sizeof(*(vec->_l._data)) * (count));
        if(!data)
            return UVEC_ERR;
        (((vec)->_s[(sizeof(struct p_uvec_large_EaiGraphNode_dev)) -
                    1]) = (ulib_byte) (((ulib_byte) 0xFF)));
        vec->_l._data = data;
    }
    return UVEC_OK;
}
__attribute__((__unused__)) uvec_ret uvec_push_EaiGraphNode_dev(UVec_EaiGraphNode_dev *vec,
                                                                EaiGraphNode_dev item)
{
    if(uvec_expand_if_required_EaiGraphNode_dev(vec))
        return UVEC_ERR;
    ulib_uint count = uvec_count_EaiGraphNode_dev(vec);
    uvec_data_EaiGraphNode_dev(vec)[count] = item;
    p_uvec_set_count_EaiGraphNode_dev(vec, count + 1);
    return UVEC_OK;
}
__attribute__((__unused__)) EaiGraphNode_dev uvec_pop_EaiGraphNode_dev(UVec_EaiGraphNode_dev *vec)
{
    ulib_uint count = uvec_count_EaiGraphNode_dev(vec) - 1;
    EaiGraphNode_dev ret = uvec_data_EaiGraphNode_dev(vec)[count];
    p_uvec_set_count_EaiGraphNode_dev(vec, count);
    return ret;
}
__attribute__((__unused__)) EaiGraphNode_dev uvec_remove_at_EaiGraphNode_dev(UVec_EaiGraphNode_dev
                                                                                 *vec,
                                                                             ulib_uint idx)
{
    EaiGraphNode_dev *data = uvec_data_EaiGraphNode_dev(vec);
    EaiGraphNode_dev item = data[idx];
    ulib_uint count = uvec_count_EaiGraphNode_dev(vec);
    if(idx < count - 1) {
        size_t block_size = (count - idx - 1) * sizeof(EaiGraphNode_dev);
        memmove(&(data[idx]), &(data[idx + 1]), block_size);
    }
    p_uvec_set_count_EaiGraphNode_dev(vec, count - 1);
    return item;
}
__attribute__((__unused__)) uvec_ret uvec_insert_at_EaiGraphNode_dev(UVec_EaiGraphNode_dev *vec,
                                                                     ulib_uint idx,
                                                                     EaiGraphNode_dev item)
{
    if(uvec_expand_if_required_EaiGraphNode_dev(vec))
        return UVEC_ERR;
    ulib_uint count = uvec_count_EaiGraphNode_dev(vec);
    EaiGraphNode_dev *data = uvec_data_EaiGraphNode_dev(vec);
    if(idx < count) {
        size_t block_size = (count - idx) * sizeof(EaiGraphNode_dev);
        memmove(&(data[idx + 1]), &(data[idx]), block_size);
    }
    data[idx] = item;
    p_uvec_set_count_EaiGraphNode_dev(vec, count + 1);
    return UVEC_OK;
}
__attribute__((__unused__)) void uvec_remove_range_EaiGraphNode_dev(UVec_EaiGraphNode_dev *vec,
                                                                    ulib_uint start,
                                                                    ulib_uint n)
{
    if(!n)
        return;
    ulib_uint const move_aside = uvec_count_EaiGraphNode_dev(vec) - (start + n);
    if(move_aside) {
        EaiGraphNode_dev *const data = uvec_data_EaiGraphNode_dev(vec) + start;
        memmove(data, data + n, move_aside * sizeof(EaiGraphNode_dev));
    }
    p_uvec_set_count_EaiGraphNode_dev(vec, move_aside + start);
}
__attribute__((__unused__)) uvec_ret uvec_insert_range_EaiGraphNode_dev(UVec_EaiGraphNode_dev *vec,
                                                                        EaiGraphNode_dev const
                                                                            *array,
                                                                        ulib_uint start,
                                                                        ulib_uint n)
{
    if(!n)
        return UVEC_OK;
    ulib_uint count = uvec_count_EaiGraphNode_dev(vec);
    ulib_uint const move_aside = count - start;
    if(uvec_reserve_EaiGraphNode_dev(vec, (count += n)))
        return UVEC_ERR;
    EaiGraphNode_dev *const data = uvec_data_EaiGraphNode_dev(vec) + start;
    if(move_aside)
        memmove(data + n, data, move_aside * sizeof(EaiGraphNode_dev));
    memcpy(data, array, n * sizeof(EaiGraphNode_dev));
    p_uvec_set_count_EaiGraphNode_dev(vec, count);
    return UVEC_OK;
}
__attribute__((__unused__)) void uvec_remove_all_EaiGraphNode_dev(UVec_EaiGraphNode_dev *vec)
{
    p_uvec_set_count_EaiGraphNode_dev(vec, 0);
}
__attribute__((__unused__)) void uvec_reverse_EaiGraphNode_dev(UVec_EaiGraphNode_dev *vec)
{
    EaiGraphNode_dev *data = uvec_data_EaiGraphNode_dev(vec);
    ulib_uint count = uvec_count_EaiGraphNode_dev(vec);
    for(ulib_uint i = 0; i < count / 2; ++i) {
        ulib_uint swap_idx = count - i - 1;
        do {
            EaiGraphNode_dev p_ulib_swap_temp = data[i];
            data[i] = data[swap_idx];
            data[swap_idx] = p_ulib_swap_temp;
        } while(0);
    }
}
__attribute__((__unused__)) ulib_uint uvec_index_of_EaiGraphNode_dev(UVec_EaiGraphNode_dev const
                                                                         *vec,
                                                                     EaiGraphNode_dev item)
{
    EaiGraphNode_dev *data = uvec_data_EaiGraphNode_dev(vec);
    ulib_uint count = uvec_count_EaiGraphNode_dev(vec);
    if(0) {
        if(count > (sizeof(ulib_ptr) * 8 * 2)) {
            EaiGraphNode_dev *p = ulib_mem_mem(data,
                                               count * sizeof(EaiGraphNode_dev),
                                               &item,
                                               sizeof(item));
            return p ? (ulib_uint) (p - data) : count;
        }
    }
    for(ulib_uint i = 0; i < count; ++i) {
        if(node_eq(data[i], item))
            return i;
    }
    return count;
}
__attribute__((__unused__)) ulib_uint uvec_index_of_reverse_EaiGraphNode_dev(
    UVec_EaiGraphNode_dev const *vec,
    EaiGraphNode_dev item)
{
    EaiGraphNode_dev *data = uvec_data_EaiGraphNode_dev(vec);
    ulib_uint count = uvec_count_EaiGraphNode_dev(vec);
    for(ulib_uint i = count; i-- != 0;) {
        if(node_eq(data[i], item))
            return i;
    }
    return count;
}
__attribute__((__unused__)) _Bool uvec_remove_EaiGraphNode_dev(UVec_EaiGraphNode_dev *vec,
                                                               EaiGraphNode_dev item)
{
    ulib_uint idx = uvec_index_of_EaiGraphNode_dev(vec, item);
    if(idx < uvec_count_EaiGraphNode_dev(vec)) {
        uvec_remove_at_EaiGraphNode_dev(vec, idx);
        return 1;
    }
    return 0;
}
__attribute__((__unused__)) _Bool uvec_equals_EaiGraphNode_dev(UVec_EaiGraphNode_dev const *vec,
                                                               UVec_EaiGraphNode_dev const *other)
{
    if(vec == other)
        return 1;
    ulib_uint count = uvec_count_EaiGraphNode_dev(vec), ocount = uvec_count_EaiGraphNode_dev(other);
    if(count != ocount)
        return 0;
    if(!count)
        return 1;
    EaiGraphNode_dev *data = uvec_data_EaiGraphNode_dev(vec);
    EaiGraphNode_dev *o_data = uvec_data_EaiGraphNode_dev(other);
    if(0) {
        return memcmp(data, o_data, count * sizeof(EaiGraphNode_dev)) == 0;
    }
    for(ulib_uint i = 0; i < count; ++i) {
        if(!node_eq(data[i], o_data[i]))
            return 0;
    }
    return 1;
}
__attribute__((__unused__)) uvec_ret uvec_push_unique_EaiGraphNode_dev(UVec_EaiGraphNode_dev *vec,
                                                                       EaiGraphNode_dev item)
{
    ulib_uint count = uvec_count_EaiGraphNode_dev(vec);
    return uvec_index_of_EaiGraphNode_dev(vec, item) < count ?
               UVEC_NO :
               uvec_push_EaiGraphNode_dev(vec, item);
}

static inline uvec_ret uvec_resize_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec, ulib_uint size)
{
    EaiGraphEdge_dev *data;
    size = ulib_uint_ceil2(size);
    ulib_byte exp = ((vec)->_s[(sizeof(struct p_uvec_large_EaiGraphEdge_dev)) - 1]);
    if(((exp) & (((ulib_byte) 0x80)))) {
        if(((exp) == ((ulib_byte) 0xFE)))
            return UVEC_OK;
        data = (EaiGraphEdge_dev *) realloc(vec->_l._data, sizeof(*(vec->_l._data)) * (size));
        if(!data)
            return UVEC_ERR;
    } else {
        data = (EaiGraphEdge_dev *) malloc(sizeof(*(data)) * (size));
        if(!data)
            return UVEC_ERR;
        memcpy(data, vec->_s, exp * sizeof(EaiGraphEdge_dev));
        vec->_l._count = exp;
    }
    (((vec)->_s[(sizeof(struct p_uvec_large_EaiGraphEdge_dev)) -
                1]) = (ulib_byte) (ulib_uint_log2(size) | ((ulib_byte) 0x80)));
    vec->_l._data = data;
    return UVEC_OK;
}
static inline uvec_ret uvec_expand_if_required_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec)
{
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec);
    return uvec_size_EaiGraphEdge_dev(vec) > count ? UVEC_OK :
                                                     uvec_resize_EaiGraphEdge_dev(vec, count + 1);
}
__attribute__((__unused__)) uvec_ret uvec_reserve_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec,
                                                                   ulib_uint size)
{
    return size <= uvec_size_EaiGraphEdge_dev(vec) ? UVEC_OK :
                                                     uvec_resize_EaiGraphEdge_dev(vec, size);
}
__attribute__((__unused__)) uvec_ret uvec_set_range_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec,
                                                                     EaiGraphEdge_dev const *array,
                                                                     ulib_uint start,
                                                                     ulib_uint n)
{
    if(!n)
        return UVEC_OK;
    ulib_uint const old_c = uvec_count_EaiGraphEdge_dev(vec), new_c = start + n;
    if(new_c > old_c) {
        if(uvec_reserve_EaiGraphEdge_dev(vec, new_c))
            return UVEC_ERR;
        p_uvec_set_count_EaiGraphEdge_dev(vec, new_c);
    }
    EaiGraphEdge_dev *data = uvec_data_EaiGraphEdge_dev(vec);
    memcpy(&(data[start]), array, n * sizeof(EaiGraphEdge_dev));
    return UVEC_OK;
}
__attribute__((__unused__)) uvec_ret uvec_copy_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev const *src,
                                                                UVec_EaiGraphEdge_dev *dest)
{
    ulib_uint count = uvec_count_EaiGraphEdge_dev(src);
    uvec_ret ret = uvec_reserve_EaiGraphEdge_dev(dest, count);
    if(!ret) {
        memcpy(uvec_data_EaiGraphEdge_dev(dest),
               uvec_data_EaiGraphEdge_dev(src),
               count * sizeof(EaiGraphEdge_dev));
        p_uvec_set_count_EaiGraphEdge_dev(dest, count);
    }
    return ret;
}
__attribute__((__unused__)) void uvec_copy_to_array_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev const
                                                                         *vec,
                                                                     EaiGraphEdge_dev array[])
{
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec);
    if(count)
        memcpy(array, uvec_data_EaiGraphEdge_dev(vec), count * sizeof(EaiGraphEdge_dev));
}
__attribute__((__unused__)) uvec_ret uvec_shrink_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec)
{
    ulib_byte exp = ((vec)->_s[(sizeof(struct p_uvec_large_EaiGraphEdge_dev)) - 1]);
    if(((exp) == ((ulib_byte) 0xFE)))
        return UVEC_OK;
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec);
    if(!count) {
        uvec_deinit_EaiGraphEdge_dev(vec);
        return UVEC_OK;
    }
    if(count <= ((sizeof(struct p_uvec_large_EaiGraphEdge_dev) - 1) / sizeof(EaiGraphEdge_dev))) {
        if((!((exp) & (((ulib_byte) 0x80)))))
            return UVEC_OK;
        EaiGraphEdge_dev *old_data = vec->_l._data;
        memcpy(vec->_s, old_data, count * sizeof(EaiGraphEdge_dev));
        free(old_data);
        (((vec)->_s[(sizeof(struct p_uvec_large_EaiGraphEdge_dev)) - 1]) = (ulib_byte) (count));
    } else if(!((exp) == ((ulib_byte) 0xFF))) {
        EaiGraphEdge_dev *data = (EaiGraphEdge_dev *) realloc(vec->_l._data,
                                                              sizeof(*(vec->_l._data)) * (count));
        if(!data)
            return UVEC_ERR;
        (((vec)->_s[(sizeof(struct p_uvec_large_EaiGraphEdge_dev)) -
                    1]) = (ulib_byte) (((ulib_byte) 0xFF)));
        vec->_l._data = data;
    }
    return UVEC_OK;
}
__attribute__((__unused__)) uvec_ret uvec_push_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec,
                                                                EaiGraphEdge_dev item)
{
    if(uvec_expand_if_required_EaiGraphEdge_dev(vec))
        return UVEC_ERR;
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec);
    uvec_data_EaiGraphEdge_dev(vec)[count] = item;
    p_uvec_set_count_EaiGraphEdge_dev(vec, count + 1);
    return UVEC_OK;
}
__attribute__((__unused__)) EaiGraphEdge_dev uvec_pop_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec)
{
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec) - 1;
    EaiGraphEdge_dev ret = uvec_data_EaiGraphEdge_dev(vec)[count];
    p_uvec_set_count_EaiGraphEdge_dev(vec, count);
    return ret;
}
__attribute__((__unused__)) EaiGraphEdge_dev uvec_remove_at_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev
                                                                                 *vec,
                                                                             ulib_uint idx)
{
    EaiGraphEdge_dev *data = uvec_data_EaiGraphEdge_dev(vec);
    EaiGraphEdge_dev item = data[idx];
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec);
    if(idx < count - 1) {
        size_t block_size = (count - idx - 1) * sizeof(EaiGraphEdge_dev);
        memmove(&(data[idx]), &(data[idx + 1]), block_size);
    }
    p_uvec_set_count_EaiGraphEdge_dev(vec, count - 1);
    return item;
}
__attribute__((__unused__)) uvec_ret uvec_insert_at_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec,
                                                                     ulib_uint idx,
                                                                     EaiGraphEdge_dev item)
{
    if(uvec_expand_if_required_EaiGraphEdge_dev(vec))
        return UVEC_ERR;
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec);
    EaiGraphEdge_dev *data = uvec_data_EaiGraphEdge_dev(vec);
    if(idx < count) {
        size_t block_size = (count - idx) * sizeof(EaiGraphEdge_dev);
        memmove(&(data[idx + 1]), &(data[idx]), block_size);
    }
    data[idx] = item;
    p_uvec_set_count_EaiGraphEdge_dev(vec, count + 1);
    return UVEC_OK;
}
__attribute__((__unused__)) void uvec_remove_range_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec,
                                                                    ulib_uint start,
                                                                    ulib_uint n)
{
    if(!n)
        return;
    ulib_uint const move_aside = uvec_count_EaiGraphEdge_dev(vec) - (start + n);
    if(move_aside) {
        EaiGraphEdge_dev *const data = uvec_data_EaiGraphEdge_dev(vec) + start;
        memmove(data, data + n, move_aside * sizeof(EaiGraphEdge_dev));
    }
    p_uvec_set_count_EaiGraphEdge_dev(vec, move_aside + start);
}
__attribute__((__unused__)) uvec_ret uvec_insert_range_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec,
                                                                        EaiGraphEdge_dev const
                                                                            *array,
                                                                        ulib_uint start,
                                                                        ulib_uint n)
{
    if(!n)
        return UVEC_OK;
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec);
    ulib_uint const move_aside = count - start;
    if(uvec_reserve_EaiGraphEdge_dev(vec, (count += n)))
        return UVEC_ERR;
    EaiGraphEdge_dev *const data = uvec_data_EaiGraphEdge_dev(vec) + start;
    if(move_aside)
        memmove(data + n, data, move_aside * sizeof(EaiGraphEdge_dev));
    memcpy(data, array, n * sizeof(EaiGraphEdge_dev));
    p_uvec_set_count_EaiGraphEdge_dev(vec, count);
    return UVEC_OK;
}
__attribute__((__unused__)) void uvec_remove_all_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec)
{
    p_uvec_set_count_EaiGraphEdge_dev(vec, 0);
}
__attribute__((__unused__)) void uvec_reverse_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec)
{
    EaiGraphEdge_dev *data = uvec_data_EaiGraphEdge_dev(vec);
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec);
    for(ulib_uint i = 0; i < count / 2; ++i) {
        ulib_uint swap_idx = count - i - 1;
        do {
            EaiGraphEdge_dev p_ulib_swap_temp = data[i];
            data[i] = data[swap_idx];
            data[swap_idx] = p_ulib_swap_temp;
        } while(0);
    }
}
__attribute__((__unused__)) ulib_uint uvec_index_of_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev const
                                                                         *vec,
                                                                     EaiGraphEdge_dev item)
{
    EaiGraphEdge_dev *data = uvec_data_EaiGraphEdge_dev(vec);
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec);
    if(0) {
        if(count > (sizeof(ulib_ptr) * 8 * 2)) {
            EaiGraphEdge_dev *p = ulib_mem_mem(data,
                                               count * sizeof(EaiGraphEdge_dev),
                                               &item,
                                               sizeof(item));
            return p ? (ulib_uint) (p - data) : count;
        }
    }
    for(ulib_uint i = 0; i < count; ++i) {
        if(edge_eq(data[i], item))
            return i;
    }
    return count;
}
__attribute__((__unused__)) ulib_uint uvec_index_of_reverse_EaiGraphEdge_dev(
    UVec_EaiGraphEdge_dev const *vec,
    EaiGraphEdge_dev item)
{
    EaiGraphEdge_dev *data = uvec_data_EaiGraphEdge_dev(vec);
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec);
    for(ulib_uint i = count; i-- != 0;) {
        if(edge_eq(data[i], item))
            return i;
    }
    return count;
}
__attribute__((__unused__)) _Bool uvec_remove_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec,
                                                               EaiGraphEdge_dev item)
{
    ulib_uint idx = uvec_index_of_EaiGraphEdge_dev(vec, item);
    if(idx < uvec_count_EaiGraphEdge_dev(vec)) {
        uvec_remove_at_EaiGraphEdge_dev(vec, idx);
        return 1;
    }
    return 0;
}
__attribute__((__unused__)) _Bool uvec_equals_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev const *vec,
                                                               UVec_EaiGraphEdge_dev const *other)
{
    if(vec == other)
        return 1;
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec), ocount = uvec_count_EaiGraphEdge_dev(other);
    if(count != ocount)
        return 0;
    if(!count)
        return 1;
    EaiGraphEdge_dev *data = uvec_data_EaiGraphEdge_dev(vec);
    EaiGraphEdge_dev *o_data = uvec_data_EaiGraphEdge_dev(other);
    if(0) {
        return memcmp(data, o_data, count * sizeof(EaiGraphEdge_dev)) == 0;
    }
    for(ulib_uint i = 0; i < count; ++i) {
        if(!edge_eq(data[i], o_data[i]))
            return 0;
    }
    return 1;
}
__attribute__((__unused__)) uvec_ret uvec_push_unique_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec,
                                                                       EaiGraphEdge_dev item)
{
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec);
    return uvec_index_of_EaiGraphEdge_dev(vec, item) < count ?
               UVEC_NO :
               uvec_push_EaiGraphEdge_dev(vec, item);
}
__attribute__((__unused__)) ulib_uint uvec_index_of_min_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev const
                                                                             *vec)
{
    ulib_uint min_idx = 0;
    EaiGraphEdge_dev *data = uvec_data_EaiGraphEdge_dev(vec);
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec);
    for(ulib_uint i = 1; i < count; ++i) {
        if(edge_cmp(data[i], data[min_idx]))
            min_idx = i;
    }
    return min_idx;
}
__attribute__((__unused__)) ulib_uint uvec_index_of_max_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev const
                                                                             *vec)
{
    ulib_uint max_idx = 0;
    EaiGraphEdge_dev *data = uvec_data_EaiGraphEdge_dev(vec);
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec);
    for(ulib_uint i = 1; i < count; ++i) {
        if(edge_cmp(data[max_idx], data[i]))
            max_idx = i;
    }
    return max_idx;
}
static inline EaiGraphEdge_dev p_uvec_qsort_pivot_EaiGraphEdge_dev(EaiGraphEdge_dev *a,
                                                                   ulib_uint len)
{
    ulib_uint const hi = len - 1, mid = hi / 2;
    if(edge_cmp(a[mid], a[0])) {
        do {
            EaiGraphEdge_dev p_ulib_swap_temp = a[mid];
            a[mid] = a[0];
            a[0] = p_ulib_swap_temp;
        } while(0);
    }
    if(edge_cmp(a[hi], a[mid])) {
        do {
            EaiGraphEdge_dev p_ulib_swap_temp = a[hi];
            a[hi] = a[mid];
            a[mid] = p_ulib_swap_temp;
        } while(0);
        if(edge_cmp(a[mid], a[0])) {
            do {
                EaiGraphEdge_dev p_ulib_swap_temp = a[mid];
                a[mid] = a[0];
                a[0] = p_ulib_swap_temp;
            } while(0);
        }
    }
    return a[mid];
}
static inline void p_uvec_qsort_EaiGraphEdge_dev(EaiGraphEdge_dev *a, ulib_uint right)
{
    ulib_uint top = 0, left = 0, stack[(sizeof(ulib_uint) * 8 * 2)];
    while(1) {
        for(; right > left + (sizeof(ulib_uint) * 8); ++right) {
            EaiGraphEdge_dev pivot = p_uvec_qsort_pivot_EaiGraphEdge_dev(a + left, right - left);
            stack[top] = right;
            for(ulib_uint i = left - 1;;) {
                while(edge_cmp(a[++i], pivot)) {}
                while(edge_cmp(pivot, a[--right])) {}
                if(i >= right)
                    break;
                do {
                    EaiGraphEdge_dev p_ulib_swap_temp = a[i];
                    a[i] = a[right];
                    a[right] = p_ulib_swap_temp;
                } while(0);
            }
            if(stack[top] > right + (sizeof(ulib_uint) * 8)) {
                if(++top == (sizeof(ulib_uint) * 8 * 2))
                    return;
            }
        }
        if(top == 0)
            return;
        left = right;
        right = stack[--top];
    }
}
static inline void p_uvec_isort_EaiGraphEdge_dev(EaiGraphEdge_dev *a, ulib_uint len)
{
    for(ulib_uint i = 1; i < len; ++i) {
        EaiGraphEdge_dev item = a[i];
        ulib_int j = i - 1;
        for(; j >= 0 && edge_cmp(item, a[j]); --j) {
            a[j + 1] = a[j];
        }
        a[j + 1] = item;
    }
}
__attribute__((__unused__)) void uvec_sort_range_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec,
                                                                  ulib_uint start,
                                                                  ulib_uint len)
{
    EaiGraphEdge_dev *array = uvec_data_EaiGraphEdge_dev(vec) + start;
    if(len > (sizeof(ulib_uint) * 8)) {
        p_uvec_qsort_EaiGraphEdge_dev(array, len);
    }
    p_uvec_isort_EaiGraphEdge_dev(array, len);
}
__attribute__((__unused__)) ulib_uint uvec_insertion_index_sorted_EaiGraphEdge_dev(
    UVec_EaiGraphEdge_dev const *vec,
    EaiGraphEdge_dev item)
{
    EaiGraphEdge_dev const *const data = uvec_data_EaiGraphEdge_dev(vec);
    EaiGraphEdge_dev const *cur = data;
    ulib_uint len = uvec_count_EaiGraphEdge_dev(vec);
    while(len > (sizeof(ulib_uint) * 8)) {
        if(edge_cmp(cur[(len >>= 1)], item)) {
            cur += len;
        }
    }
    for(EaiGraphEdge_dev const *last = cur + len; cur < last && edge_cmp(*cur, item); ++cur) {}
    return (ulib_uint) (cur - data);
}
__attribute__((__unused__)) ulib_uint uvec_index_of_sorted_EaiGraphEdge_dev(
    UVec_EaiGraphEdge_dev const *vec,
    EaiGraphEdge_dev item)
{
    ulib_uint const i = uvec_insertion_index_sorted_EaiGraphEdge_dev(vec, item);
    EaiGraphEdge_dev *data = uvec_data_EaiGraphEdge_dev(vec);
    ulib_uint count = uvec_count_EaiGraphEdge_dev(vec);
    return data && i < count && edge_eq(data[i], item) ? i : count;
}
__attribute__((__unused__)) uvec_ret uvec_insert_sorted_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec,
                                                                         EaiGraphEdge_dev item,
                                                                         ulib_uint *idx)
{
    ulib_uint i = uvec_insertion_index_sorted_EaiGraphEdge_dev(vec, item);
    if(idx)
        *idx = i;
    return uvec_insert_at_EaiGraphEdge_dev(vec, i, item);
}
__attribute__((__unused__)) uvec_ret uvec_insert_sorted_unique_EaiGraphEdge_dev(
    UVec_EaiGraphEdge_dev *vec,
    EaiGraphEdge_dev item,
    ulib_uint *idx)
{
    EaiGraphEdge_dev *data = uvec_data_EaiGraphEdge_dev(vec);
    ulib_uint i = uvec_insertion_index_sorted_EaiGraphEdge_dev(vec, item);
    if(idx)
        *idx = i;
    if(i < uvec_count_EaiGraphEdge_dev(vec) && edge_eq(data[i], item))
        return UVEC_NO;
    return uvec_insert_at_EaiGraphEdge_dev(vec, i, item);
}
__attribute__((__unused__)) _Bool uvec_remove_sorted_EaiGraphEdge_dev(UVec_EaiGraphEdge_dev *vec,
                                                                      EaiGraphEdge_dev item)
{
    ulib_uint i = uvec_index_of_sorted_EaiGraphEdge_dev(vec, item);
    if(!((i) < uvec_count_EaiGraphEdge_dev(vec)))
        return 0;
    uvec_remove_at_EaiGraphEdge_dev(vec, i);
    return 1;
}

static inline ulib_uint p_eai_graph_dev_edge_to_start_ex(EaiGraph_dev *g, ulib_uint ei)
{
    ulib_uint start_i = uvec_insertion_index_sorted(ulib_uint, &g->limits, ei);
    ulib_uint *D = uvec_data(ulib_uint, &g->limits);
    ulib_uint count = uvec_count(ulib_uint, &g->limits);
    for(; start_i < count && ei == D[start_i] ; start_i++);
    return start_i;
}

EaiGraph_dev eai_graph_dev(void)
{
    EaiGraph_dev result;
    result.nodes = uvec(EaiGraphNode_dev);
    result.edges = uvec(EaiGraphEdge_dev);
    result.limits = uvec(ulib_uint);
    result.flags = ubit_none(8);
    return result;
}

void eai_graph_deinit_dev(EaiGraph_dev *g)
{
    uvec_deinit(ulib_uint, &g->limits);
    uvec_deinit(EaiGraphEdge_dev, &g->edges);
    uvec_deinit(EaiGraphNode_dev, &g->nodes);
}

bool eai_graph_add_node_dev(EaiGraph_dev *g, EaiGraphNode_dev n)
{
    g->flags = ubit_none(8);

    if(uvec_contains(EaiGraphNode_dev, &g->nodes, n)) {
        g->flags = ubit_set(8, EAI_GRAPH_DUPLICATE, g->flags);
        return false;
    }

    uvec_ret ret = uvec_push(EaiGraphNode_dev, &g->nodes, n);

    if(ret != UVEC_OK) {
        g->flags = ubit_set(8, EAI_GRAPH_MEM_ERROR, g->flags);
        return false;
    }

    ret = uvec_push(ulib_uint, &g->limits, uvec_count(EaiGraphEdge_dev, &g->edges));

    if(ret != UVEC_OK) {
        uvec_remove(EaiGraphNode_dev, &g->nodes, n);
        g->flags = ubit_set(8, EAI_GRAPH_MEM_ERROR, g->flags);
        return false;
    }

    return true;
}

bool eai_graph_add_edge_dev(EaiGraph_dev *g,
                            EaiGraphNode_dev start,
                            EaiGraphNode_dev end,
                            ulib_uint val)
{
    ulib_uint *limits = uvec_data(ulib_uint, &g->limits);
    ulib_uint limit_count = uvec_count(ulib_uint, &g->limits);

    g->flags = ubit_none(8);
    ulib_uint start_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, start);
    ulib_uint end_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, end);

    if(!uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, start_idx) ||
       !uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, end_idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return false;
    }

    EaiGraphEdge_dev edge;
    edge.to = end_idx;
    edge.val = val;

    ulib_uint offset_start = 0;
    ulib_uint offset_end = limits[start_idx];
    if (start_idx > 0) {
        offset_start = limits[start_idx-1];
    }

    UVec(EaiGraphEdge_dev) edges  = uvec_view(EaiGraphEdge_dev, &g->edges, offset_start, offset_end - offset_start);
    ulib_uint insert_idx = uvec_insertion_index_sorted(EaiGraphEdge_dev, &edges, edge);
    uvec_ret ret = uvec_insert_at(EaiGraphEdge_dev, &g->edges, offset_start + insert_idx, edge);

    if(ret != UVEC_OK) {
        g->flags = ubit_set(8, EAI_GRAPH_MEM_ERROR, g->flags);
        return false;
    }

    for (ulib_uint i = start_idx; i < limit_count; i++) {
        uvec_set(ulib_uint, &g->limits, i, 1 + limits[i]);
    }

    return true;
}

bool eai_graph_set_node_dev(EaiGraph_dev *g, EaiGraphNode_dev old, EaiGraphNode_dev new)
{
    g->flags = ubit_none(8);
    ulib_uint idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, old);

    if(!uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return false;
    }

    uvec_data(EaiGraphNode_dev, &g->nodes)[idx] = new;

    return true;
}

ulib_uint eai_graph_set_edge_dev(EaiGraph_dev *g,
                                        EaiGraphNode_dev start,
                                        EaiGraphNode_dev end,
                                        ulib_uint e)
{
    ulib_uint old_val;
    memset(&old_val, 0, sizeof(ulib_uint));

    g->flags = ubit_none(8);
    ulib_uint start_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, start);
    ulib_uint end_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, end);

    if(!uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, start_idx) ||
       !uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, end_idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return old_val;
    }

    EaiGraphEdge_dev new;
    new.to = end_idx;
    new.val = e;

    ulib_uint offset_start = 0;
    ulib_uint offset_end = uvec_get(ulib_uint, &g->limits, start_idx);
    if (start_idx > 0) {
        offset_start = uvec_get(ulib_uint, &g->limits, start_idx - 1);
    }

    UVec(EaiGraphEdge_dev) edges  = uvec_view(EaiGraphEdge_dev, &g->edges, offset_start, offset_end - offset_start);
    ulib_uint edge_idx = uvec_index_of_sorted(EaiGraphEdge_dev, &edges, new);
    if(!uvec_index_is_valid(EaiGraphEdge_dev, &edges, edge_idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return old_val;
    }

    old_val = uvec_get(EaiGraphEdge_dev, &edges, edge_idx).val;
    uvec_set(EaiGraphEdge_dev, &edges, edge_idx, new);
    return old_val;
}

ulib_uint eai_graph_get_edge_dev(EaiGraph_dev *g,
                                 EaiGraphNode_dev start,
                                 EaiGraphNode_dev end)
{
    ulib_uint old_val;
    memset(&old_val, 0, sizeof(ulib_uint));

    g->flags = ubit_none(8);
    ulib_uint start_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, start);
    ulib_uint end_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, end);

    if(!uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, start_idx) ||
       !uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, end_idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return old_val;
    }

    EaiGraphEdge_dev dummy;
    dummy.to = end_idx;

    ulib_uint offset_start = 0;
    ulib_uint offset_end = uvec_get(ulib_uint, &g->limits, start_idx);
    if (start_idx > 0) {
        offset_start = uvec_get(ulib_uint, &g->limits, start_idx - 1);
    }

    UVec(EaiGraphEdge_dev) edges  = uvec_view(EaiGraphEdge_dev, &g->edges, offset_start, offset_end - offset_start);
    ulib_uint edge_idx = uvec_index_of_sorted(EaiGraphEdge_dev, &edges, dummy);

    if(!uvec_index_is_valid(EaiGraphEdge_dev, &edges, edge_idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return old_val;
    }

    return uvec_get(EaiGraphEdge_dev, &edges, edge_idx).val;
}

bool eai_graph_del_node_dev(EaiGraph_dev *g, EaiGraphNode_dev node)
{
    g->flags = ubit_none(8);
    ulib_uint idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, node);

    if(!uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return false;
    }

    ulib_uint offset_start = 0;
    ulib_uint offset_end = uvec_get(ulib_uint, &g->limits, idx);
    if (idx > 0) {
        offset_start = uvec_get(ulib_uint, &g->limits, idx - 1);
    }
    ulib_uint edge_count = offset_end - offset_start;

    for (ulib_uint i = offset_start; i < offset_end; i++) {
        uvec_remove_at(EaiGraphEdge_dev, &g->edges, i);
    }

    for (ulib_uint i = idx; uvec_index_is_valid(ulib_uint, &g->limits, i); i++) {
        ulib_uint old_val = uvec_get(ulib_uint, &g->limits, i);
        uvec_set(ulib_uint, &g->limits, i, old_val - edge_count);
    }

    uvec_foreach_reverse(EaiGraphEdge_dev, &g->edges, e) {
        if (e.item->to == node) {
            uvec_remove_at(EaiGraphEdge_dev, &g->edges, e.i);
            uvec_foreach_reverse(ulib_uint, &g->limits, l) {
                if (*l.item >= e.i) {
                    *l.item -= 1;
                } else {
                    break;
                }
            }
        }
    }

    uvec_remove_at(EaiGraphNode_dev, &g->nodes, idx);
    uvec_remove_at(ulib_uint, &g->limits, idx);
    return true;
}

ulib_uint eai_graph_del_edge_dev(EaiGraph_dev *g, EaiGraphNode_dev start, EaiGraphNode_dev end)
{
    ulib_uint old_val;
    memset(&old_val, 0, sizeof(ulib_uint));

    g->flags = ubit_none(8);
    ulib_uint start_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, start);
    ulib_uint end_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, end);

    if(!uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, start_idx) ||
       !uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, end_idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return old_val;
    }

    ulib_uint offset_start = 0;
    ulib_uint offset_end = uvec_get(ulib_uint, &g->limits, start_idx);
    if (start_idx > 0) {
        offset_start = uvec_get(ulib_uint, &g->limits, start_idx - 1);
    }

    EaiGraphEdge_dev dummy;
    dummy.to = end_idx;

    UVec(EaiGraphEdge_dev) edges  = uvec_view(EaiGraphEdge_dev, &g->edges, offset_start, offset_end - offset_start);
    ulib_uint idx = uvec_index_of_sorted(EaiGraphEdge_dev, &edges, dummy);
    old_val = uvec_get(EaiGraphEdge_dev, &edges, idx).val;
    uvec_remove_at(EaiGraphEdge_dev, &g->edges, idx + offset_start);

    uvec_foreach_reverse(ulib_uint, &g->limits, l) {

        if (*l.item > idx) {
            *l.item -= 1;
        } else {
            break;
        }
    }

    return old_val;
}

bool eai_graph_has_node_dev(EaiGraph_dev *g, EaiGraphNode_dev node)
{
    return uvec_contains(EaiGraphNode_dev, &g->nodes, node);
}

bool eai_graph_has_edge_dev(EaiGraph_dev *g, EaiGraphNode_dev start, EaiGraphNode_dev end)
{
    g->flags = ubit_none(8);
    ulib_uint start_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, start);
    ulib_uint end_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, end);

    if(!uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, start_idx) ||
       !uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, end_idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return false;
    }

    ulib_uint offset_start = 0;
    ulib_uint offset_end = uvec_get(ulib_uint, &g->limits, start_idx);
    if (start_idx > 0) {
        offset_start = uvec_get(ulib_uint, &g->limits, start_idx - 1);
    }

    UVec(EaiGraphEdge_dev) edges  = uvec_view(EaiGraphEdge_dev, &g->edges, offset_start, offset_end - offset_start);
    EaiGraphEdge_dev dummy;
    dummy.to = end_idx;

    return uvec_contains_sorted(EaiGraphEdge_dev, &edges, dummy);
}

EaiGraphEdgeLoop_dev p_eai_graph_start_iterator_dev(EaiGraph_dev *g)
{
    EaiGraphEdgeLoop_dev loop;
    EaiGraphEdge_dev edge = uvec_first(EaiGraphEdge_dev, &g->edges);
    loop.count = uvec_count(EaiGraphEdge_dev, &g->edges);
    loop.i = 0;
    loop.start_i = p_eai_graph_dev_edge_to_start_ex(g, 0);
    loop.start = uvec_get(EaiGraphNode_dev, &g->nodes, loop.start_i);
    loop.end = uvec_get(EaiGraphNode_dev, &g->nodes, edge.to);
    loop.end_i = edge.to;
    return loop;
}

EaiGraphEdgeLoop_dev p_eai_graph_start_iterator_from_dev(EaiGraph_dev *g, EaiGraphNode_dev from)
{
    EaiGraphEdgeLoop_dev loop = p_eai_graph_start_iterator_dev(g);
    if (loop.start != from) {
        p_eai_graph_next_from_dev(g, &loop, from);
    }
    return loop;
}

EaiGraphEdgeLoop_dev p_eai_graph_start_iterator_to_dev(EaiGraph_dev *g, EaiGraphNode_dev to)
{
    EaiGraphEdgeLoop_dev loop = p_eai_graph_start_iterator_dev(g);
    if (loop.end != to) {
        p_eai_graph_next_to_dev(g, &loop, to);
    }
    return loop;
}

void p_eai_graph_next_from_dev(EaiGraph_dev *g, EaiGraphEdgeLoop_dev *l, EaiGraphNode_dev from)
{
    do {
        p_eai_graph_next_dev(g, l);
    } while (l->i < l->count && l->start != from);
}

void p_eai_graph_next_to_dev(EaiGraph_dev *g, EaiGraphEdgeLoop_dev *l, EaiGraphNode_dev to)
{
    do {
        p_eai_graph_next_dev(g, l);
    } while (l->i < l->count && l->end != to);
}

void p_eai_graph_next_dev(EaiGraph_dev *g, EaiGraphEdgeLoop_dev *l)
{
    l->i += 1;
    if (l->i >= l->count) {
        return;
    }

    EaiGraphEdge_dev edge = uvec_get(EaiGraphEdge_dev, &g->edges, l->i);

    l->start_i = p_eai_graph_dev_edge_to_start_ex(g, l->i);
    l->start =  uvec_get(EaiGraphNode_dev, &g->nodes, l->start_i);
    l->end = uvec_get(EaiGraphNode_dev, &g->nodes, edge.to);
    l->end_i = edge.to;
}


