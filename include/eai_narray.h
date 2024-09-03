/*
 * MIT License
 *
 * Copyright (c) 2022-2023 Davide Loconte <davide.loconte@hotmail.it>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef EAI_TOOLBOX_NARRAY_H
#define EAI_TOOLBOX_NARRAY_H

#include <stdarg.h>
#include <ulib.h>
#include "eai_compat.h"

EAI_BEGIN_DECLS

/**
 * Declares a new n-dimensional array type
 * @param TYPE array type
 */
#define EAI_NARRAY_DECL(TYPE)                                                                      \
    typedef struct EaiNArray_s_##TYPE {                                                            \
        UVec(ulib_uint) shape;                                                                     \
        TYPE *storage;                                                                             \
    } EaiNArray_##TYPE;                                                                            \
                                                                                                   \
    typedef struct EaiNArray_loop_s_##TYPE {                                                       \
        EaiNArray_##TYPE item;                                                                     \
        ulib_uint i;                                                                               \
        ulib_uint count;                                                                           \
        ulib_uint _view_size;                                                                      \
        TYPE *_storage;                                                                            \
    } EaiNArray_loop_##TYPE;                                                                       \
                                                                                                   \
    EaiNArray_##TYPE eai_narray_##TYPE(ulib_uint dimensions, ...);                                 \
                                                                                                   \
    EaiNArray_##TYPE eai_narray_from_shape_##TYPE(UVec(ulib_uint) *shape);                         \
    TYPE eai_narray_value_at_from_coordinates_##TYPE(EaiNArray_##TYPE *na, UVec(ulib_uint) *c);    \
    TYPE *eai_narray_value_at_ref_from_coordinates_##TYPE(EaiNArray_##TYPE *na, UVec(ulib_uint) *c);\
    TYPE eai_narray_set_value_at_from_coordinates_##TYPE(EaiNArray_##TYPE *na, TYPE val, UVec(ulib_uint) *c);\
                                                                                                   \
    EaiNArray_##TYPE eai_narray_get_##TYPE(EaiNArray_##TYPE *na, ulib_uint idx);                   \
                                                                                                   \
    TYPE eai_narray_value_at_##TYPE(EaiNArray_##TYPE *na, ...);                                    \
    TYPE *eai_narray_value_at_ref_##TYPE(EaiNArray_##TYPE *na, ...);                               \
    TYPE eai_narray_set_value_at_##TYPE(EaiNArray_##TYPE *na, TYPE val, ...);                      \
    void eai_narray_deinit_##TYPE(EaiNArray_##TYPE *na);                                           \
                                                                                                   \
    ulib_uint eai_narray_count_##TYPE(EaiNArray_##TYPE *na, ulib_uint axis);                       \
    ulib_uint eai_narray_axes_##TYPE(EaiNArray_##TYPE *na);                                        \
    UVec(ulib_uint) *eai_narray_shape_##TYPE(EaiNArray_##TYPE *na);                                \
                                                                                                   \
    EaiNArray_loop_##TYPE p_eai_narray_init_loop_##TYPE(EaiNArray_##TYPE *array);                  \
    void p_eai_narray_next_loop_##TYPE(EaiNArray_loop_##TYPE *iter);                               \
                                                                                                   \
    void eai_narray_copy_##TYPE(EaiNArray_##TYPE *dest, EaiNArray_##TYPE *src);                    \
    void eai_narray_copy_from_uvec_##TYPE(EaiNArray_##TYPE *dest, UVec(TYPE) *src);                \
    UVec(TYPE) eai_narray_copy_to_uvec_##TYPE(EaiNArray_##TYPE *src);                              \
                                                                                                   \
    ulib_uint eai_narray_size_##TYPE(EaiNArray_##TYPE *na);

/**
 * Implements a previously declared n dimensional array type
 * @param T array type.
 */
#define EAI_NARRAY_IMPL(TYPE)                                                                      \
    EaiNArray_##TYPE eai_narray_##TYPE(ulib_uint dims, ...)                                        \
    {                                                                                              \
        EaiNArray_##TYPE result;                                                                   \
        ulib_uint size = 1;                                                                        \
        result.shape = uvec(ulib_uint);                                                            \
                                                                                                   \
        va_list argptr;                                                                            \
        va_start(argptr, dims);                                                                    \
                                                                                                   \
        for(ulib_uint i = 0; i < dims; i++) {                                                      \
            ulib_uint dim = va_arg(argptr, ulib_uint);                                             \
            uvec_push(ulib_uint, &result.shape, dim);                                              \
            size *= dim;                                                                           \
        }                                                                                          \
        va_end(argptr);                                                                            \
                                                                                                   \
        result.storage = calloc(size, sizeof(TYPE));                                               \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    EaiNArray_##TYPE eai_narray_from_shape_##TYPE(UVec(ulib_uint) *shape)                          \
    {                                                                                              \
        EaiNArray_##TYPE result;                                                                   \
        ulib_uint size = 1;                                                                        \
        ulib_uint dims = 0;                                                                        \
        result.shape = uvec(ulib_uint);                                                            \
                                                                                                   \
        uvec_foreach(ulib_uint, shape, dim){                                                       \
             dims += 1;                                                                            \
             size *= *dim.item;                                                                    \
             uvec_push(ulib_uint, &result.shape, *dim.item);                                       \
        }                                                                                          \
                                                                                                   \
        result.storage = calloc(size, sizeof(TYPE));                                               \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    TYPE eai_narray_value_at_##TYPE(EaiNArray_##TYPE *na, ...)                                     \
    {                                                                                              \
        ulib_uint index = 0;                                                                       \
        va_list argptr;                                                                            \
        va_start(argptr, na);                                                                      \
                                                                                                   \
        uvec_foreach(ulib_uint, &na->shape, dim) {                                                 \
            ulib_uint size = va_arg(argptr, ulib_uint);                                            \
            index = index * (*dim.item) + size;                                                    \
        }                                                                                          \
                                                                                                   \
        va_end(argptr);                                                                            \
        return na->storage[index];                                                                 \
    }                                                                                              \
                                                                                                   \
    TYPE *eai_narray_value_at_ref_##TYPE(EaiNArray_##TYPE *na, ...)                                \
    {                                                                                              \
        ulib_uint index = 0;                                                                       \
        va_list argptr;                                                                            \
        va_start(argptr, na);                                                                      \
                                                                                                   \
        uvec_foreach(ulib_uint, &na->shape, dim) {                                                 \
            index = index * (*dim.item) + va_arg(argptr, ulib_uint);                               \
        }                                                                                          \
                                                                                                   \
        va_end(argptr);                                                                            \
        return &na->storage[index];                                                                \
    }                                                                                              \
                                                                                                   \
    TYPE eai_narray_set_value_at_##TYPE(EaiNArray_##TYPE *na, TYPE val, ...)                       \
    {                                                                                              \
        ulib_uint index = 0;                                                                       \
        TYPE old_val;                                                                              \
                                                                                                   \
        va_list argptr;                                                                            \
        va_start(argptr, val);                                                                     \
                                                                                                   \
        uvec_foreach(ulib_uint, &na->shape, dim) {                                                 \
            index = index * (*dim.item) + va_arg(argptr, ulib_uint);                               \
        }                                                                                          \
        va_end(argptr);                                                                            \
                                                                                                   \
        old_val = na->storage[index];                                                              \
        na->storage[index] = val;                                                                  \
        return old_val;                                                                            \
    }                                                                                              \
                                                                                                   \
    void eai_narray_deinit_##TYPE(EaiNArray_##TYPE *na)                                            \
    {                                                                                              \
        free(na->storage);                                                                         \
        uvec_deinit(ulib_uint, &na->shape);                                                        \
    }                                                                                              \
                                                                                                   \
    ulib_uint eai_narray_count_##TYPE(EaiNArray_##TYPE *na, ulib_uint axis)                        \
    {                                                                                              \
        return uvec_get(ulib_uint, &na->shape, axis);                                              \
    }                                                                                              \
                                                                                                   \
    ulib_uint eai_narray_axes_##TYPE(EaiNArray_##TYPE *na)                                         \
    {                                                                                              \
        return uvec_count(ulib_uint, &na->shape);                                                  \
    }                                                                                              \
                                                                                                   \
    UVec(ulib_uint) *eai_narray_shape_##TYPE(EaiNArray_##TYPE *na)                                 \
    {                                                                                              \
        return &na->shape;                                                                         \
    }                                                                                              \
                                                                                                   \
    EaiNArray_loop_##TYPE p_eai_narray_init_loop_##TYPE(EaiNArray_##TYPE *array)                   \
    {                                                                                              \
        EaiNArray_loop_##TYPE iter = { 0 };                                                        \
        const ulib_uint dims = uvec_count(ulib_uint, &array->shape);                               \
        if (dims == 0) return iter;                                                                \
                                                                                                   \
        ulib_uint *shape_data = uvec_data(ulib_uint, &array->shape);                               \
        iter._storage = array->storage;                                                            \
                                                                                                   \
        iter.count = shape_data[0];                                                                \
        iter._view_size = 1;                                                                       \
                                                                                                   \
        for(ulib_uint i = 1; i < dims; i++) {                                                      \
            iter._view_size *= shape_data[i];                                                      \
        }                                                                                          \
                                                                                                   \
        iter.item.shape = uvec_wrap(ulib_uint, &shape_data[1], dims - 1);                          \
        iter.item.storage = &iter._storage[iter.i * iter._view_size];                              \
        return iter;                                                                               \
    }                                                                                              \
                                                                                                   \
    void p_eai_narray_next_loop_##TYPE(EaiNArray_loop_##TYPE *iter)                                \
    {                                                                                              \
        iter->i++;                                                                                 \
        if (iter->i == iter->count) return;                                                        \
        iter->item.storage = &iter->_storage[iter->i * iter->_view_size];                          \
    }                                                                                              \
                                                                                                   \
    TYPE eai_narray_value_at_from_coordinates_##TYPE(EaiNArray_##TYPE *na, UVec(ulib_uint) *c){    \
        ulib_uint index = 0;                                                                       \
        ulib_uint *shape_data = uvec_data(ulib_uint, &na->shape);                                  \
                                                                                                   \
        uvec_foreach(ulib_uint, c, dim) {                                                          \
            ulib_uint size = *dim.item;                                                            \
            ulib_uint axis_size = shape_data[dim.i];                                               \
            index = index * axis_size + size;                                                      \
        }                                                                                          \
                                                                                                   \
        return na->storage[index];                                                                 \
    }                                                                                              \
                                                                                                   \
    TYPE *eai_narray_value_at_ref_from_coordinates_##TYPE(EaiNArray_##TYPE *na, UVec(ulib_uint) *c){\
        ulib_uint index = 0;                                                                       \
        ulib_uint *shape_data = uvec_data(ulib_uint, &na->shape);                                  \
                                                                                                   \
        uvec_foreach(ulib_uint, c, dim) {                                                          \
            ulib_uint size = *dim.item;                                                            \
            ulib_uint axis_size = shape_data[dim.i];                                               \
            index = index * axis_size + size;                                                      \
        }                                                                                          \
                                                                                                   \
        return &na->storage[index];                                                                \
    }                                                                                              \
                                                                                                   \
    TYPE eai_narray_set_value_at_from_coordinates_##TYPE(EaiNArray_##TYPE *na, TYPE val, UVec(ulib_uint) *c)\
    {                                                                                              \
        ulib_uint index = 0;                                                                       \
        ulib_uint *shape_data = uvec_data(ulib_uint, &na->shape);                                  \
                                                                                                   \
        uvec_foreach(ulib_uint, c, dim) {                                                          \
            ulib_uint size = *dim.item;                                                            \
            ulib_uint axis_size = shape_data[dim.i];                                               \
            index = index * axis_size + size;                                                      \
        }                                                                                          \
                                                                                                   \
        TYPE old_val = na->storage[index];                                                         \
        na->storage[index] = val;                                                                  \
        return old_val;                                                                            \
    }                                                                                              \
                                                                                                   \
    ulib_uint eai_narray_size_##TYPE(EaiNArray_##TYPE *na)                                         \
    {                                                                                              \
        ulib_uint ret = 1;                                                                         \
        uvec_foreach(ulib_uint, &na->shape, dim) {                                                 \
            ret *= *dim.item;                                                                      \
        }                                                                                          \
        return ret;                                                                                \
    }                                                                                              \
                                                                                                   \
    EaiNArray_##TYPE eai_narray_get_##TYPE(EaiNArray_##TYPE *na, ulib_uint idx)                    \
    {                                                                                              \
        EaiNArray(TYPE) item;                                                                      \
        const ulib_uint dims = uvec_count(ulib_uint, &na->shape);                                  \
        ulib_uint *shape_data = uvec_data(ulib_uint, &na->shape);                                  \
        ulib_uint view_size = 1;                                                                   \
                                                                                                   \
        for(ulib_uint i = 1; i < dims; i++) {                                                      \
            view_size *= shape_data[i];                                                            \
        }                                                                                          \
                                                                                                   \
        item.shape = uvec_wrap(ulib_uint, &shape_data[1], dims - 1);                               \
        item.storage = &na->storage[idx * view_size];                                              \
        return item;                                                                               \
    }                                                                                              \
                                                                                                   \
    void eai_narray_copy_##TYPE(EaiNArray_##TYPE *dest, EaiNArray_##TYPE *src)                     \
    {                                                                                              \
        ulib_uint size = eai_narray_size_##TYPE(dest);                                             \
        memcpy(dest->storage, src->storage, size * sizeof(TYPE));                                  \
    }                                                                                              \
                                                                                                   \
    void eai_narray_copy_from_uvec_##TYPE(EaiNArray_##TYPE *dest, UVec(TYPE) *src)                 \
    {                                                                                              \
        TYPE *data = uvec_data(TYPE, src);                                                         \
        ulib_uint size = eai_narray_size_##TYPE(dest);                                             \
        memcpy(dest->storage, data, size * sizeof(TYPE));                                          \
    }                                                                                              \
                                                                                                   \
    UVec(TYPE) eai_narray_copy_to_uvec_##TYPE(EaiNArray_##TYPE *src)                               \
    {                                                                                              \
        ulib_uint size = eai_narray_size_##TYPE(src);                                              \
        TYPE *data = malloc(sizeof(TYPE) * size);                                                  \
        memcpy(data, src->storage, size);                                                          \
        return uvec_assign(TYPE, data, size);                                                      \
    }


/**
 * Declares a n-dimensional array vector
 * @param the array type
 */
#define EaiNArray(type) EaiNArray_##type

/**
 * Create a new N-dimensional array
 * @param type the type of data stored in the elements of the array
 * @param dimensions N, the number of dimensions
 * @param ... the max length of each dimension
 * @return an n-dimensional array
 */
#define eai_narray(type, dimensions, ...) eai_narray_##type(dimensions, __VA_ARGS__)

/**
 * @param type the type of data stored in the elements of the array
 * @param na the n-dimensional array
 * @param ... coordinates of the element for each dimension
 * @return the element indexed by the coordinates
 */
#define eai_narray_value_at(type, array, ...) eai_narray_value_at_##type(array, __VA_ARGS__)

/**
 * @param type the type of data stored in the elements of the array
 * @param na the n-dimensional array
 * @param ... coordinates of the element for each dimension
 * @return the memory address of the element indexed by the coordinates
 */
#define eai_narray_value_at_ref(type, array, ...) eai_narray_value_at_ref_##type(array, __VA_ARGS__)

/**
 * @param type the type of data stored in the elements of the array
 * @param na the n-dimensional array
 * @param val the value to set
 * @param ... the coordinates of the element for each dimension
 * @return the replaced value
 */
#define eai_narray_set_value_at(type, array, val, ...) eai_narray_set_value_at_##type(array, val, __VA_ARGS__)

/**
 * Deinit a n-dimensional array
 * @param type the type of data stored in the elements of the array
 * @param na the array
 */
#define eai_narray_deinit(type, array) eai_narray_deinit_##type(array)

/**
 * Return the length of an axis
 * @param type the type of data stored in the elements of the array
 * @param array the array
 * @param axis the axis
 * @return the length of the given axis
 */
#define eai_narray_count(type, array, axis) eai_narray_count_##type(array, axis)

/**
 * Return the number of axes
 * @param type the type of the data
 * @param array the array
 * @return the axes count
 */
#define eai_narray_axes(type, array) eai_narray_axes_##type(array)

/**
 * Return the shape of the array
 * @param type the type of the data
 * @param array the array itself
 * @return a vector representing the shape of the array
 * @note the returned array should not be free-ed and has the same lifetime of the narray
 */
#define eai_narray_shape(type, array) eai_narray_shape_##type(array)

/**
 * @param type the type of data stored in the elements of the array
 * @param array the n-dimensional array
 * @param coordinates a vector containing the coordinates of the element for each dimension
 * @return the element indexed by the coordinates
 */
#define eai_narray_value_at_from_coordinates(type, array, coordinates) eai_narray_value_at_from_coordinates_##type(array, coordinates)

/**
 * @param type the type of data stored in the elements of the array
 * @param array the n-dimensional array
 * @param coordinates a vector containing the coordinates of the element for each dimension
 * @return a reference to the element indexed by the coordinates
 */
#define eai_narray_value_at_ref_from_coordinates(type, array, coordinates) eai_narray_value_at_ref_from_coordinates_##type(array, coordinates)

/**
 * @param type the type of data stored in the elements of the array
 * @param array the n-dimensional array
 * @param val the value to insert into the array
 * @param coordinates a vector containing the coordinates of the element for each dimension
 * @return the value being replaced
 */
#define eai_narray_set_value_at_from_coordinates(type, array, val, coordinates) eai_narray_set_value_at_from_coordinates_##type(array, val, coordinates)

/**
 * @param type the type of data stored in the elements of the array
 * @param shape a vector containing the desidered shape
 * @return an instance of narray having the specified shape
 */
#define eai_narray_from_shape(type, shape) eai_narray_from_shape_##type(shape)

/**
 * @param type the type of data stored in the elements of the array
 * @param array the array
 * @return the size of the underlying storage
 */
#define eai_narray_size(type, array) eai_narray_size_##type(array)

/**
 * Iterate through the first axis of the input array
 * @param type the type of the narray
 * @param array the array itself
 * @param iter [out] name of the iterator
 */
#define eai_narray_foreach(type, array, iter) for(EaiNArray_loop_##type iter = p_eai_narray_init_loop_##type(array);    \
                                                  iter.i != iter.count;                                                 \
                                                  p_eai_narray_next_loop_##type(&iter))

/**
 * @param type the type of the narray
 * @param array the array
 * @param idx the index
 * @return the sub-narray at the specified index for the first axis
 * @note the returned narray is a view, and should not be deallocated.
 *       Changes on this array propagates to the original array.
 *       Lifetime of this narray is tied to the starting narray
 */
#define eai_narray_get(type, array, idx) eai_narray_get_##type(array, idx)

/**
 * Copy the source narray into the destination
 * @param type the type of the narray
 * @param dest the destination array
 * @param src the source array
 * @note dest and src should have the same shape, otherwise copy will results
 *       in a buffer overflow
 * @note the destination and the source are two distinct objects, you are responsible for deallocating
 *       them
 */
#define eai_narray_copy(type, dest, src) eai_narray_copy_##type(dest, src)

/**
 * Copy the source vector into the destination
 * @param type the type of the narray
 * @param dest the destination narray
 * @param src the source vector
 * @note destination size has to be large enough to accommodate the source vector
 * @note the destination and the source are two distinct objects, you are responsible for deallocating
 *       them
 */
#define eai_narray_copy_from_uvec(type, dest, src) eai_narray_copy_from_uvec_##type(dest, src)

/**
 * Create a new vector from by flattening source narray
 * @param type the type of narray
 * @param src the narray source
 * @return the vector of the specified type obtained by flattening src
 * @note you have to declare the target uvec types
 * @note the destination and the source are two distinct objects, you are responsible for deallocating
 *       them
 */
#define eai_narray_copy_to_uvec(type, src) eai_narray_copy_to_uvec_##type(src)

EAI_END_DECLS

#endif
