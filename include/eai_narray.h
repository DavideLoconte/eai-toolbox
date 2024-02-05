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

/**
 * Declares a new n-dimensional array type
 * @param TYPE array type
 */
#define EAI_NARRAY_DECL(TYPE)                                                                      \
    typedef struct EaiNArray_s_##TYPE {                                                            \
        UVec(ulib_uint) shape;                                                                     \
        UVec(TYPE) storage;                                                                        \
    } EaiNArray_##TYPE;                                                                            \
                                                                                                   \
    typedef struct EaiNArray_loop_s_##TYPE {                                                       \
        EaiNArray_##TYPE item;                                                                     \
        ulib_uint i;                                                                               \
        ulib_uint count;                                                                           \
        ulib_uint __view_size;                                                                     \
                                                                                                   \
    } EaiNArray_loop_##TYPE;                                                                       \
                                                                                                   \
    EaiNArray_##TYPE eai_narray_##TYPE(ulib_uint dimensions, ...);                                 \
    EaiNArray_##TYPE eai_narray_from_shape_##TYPE(UVec(ulib_uint) *shape);                         \
    TYPE eai_narray_get_##TYPE(EaiNArray_##TYPE *na, ...);                                         \
    TYPE eai_narray_get_from_coordinates_##TYPE(EaiNArray_##TYPE *na, UVec(ulib_uint) *c);         \
    TYPE *eai_narray_get_ref_from_coordinates_##TYPE(EaiNArray_##TYPE *na, UVec(ulib_uint) *c);    \
    TYPE eai_narray_set_from_coordinates_##TYPE(EaiNArray_##TYPE *na, TYPE val, UVec(ulib_uint) *c);\
    TYPE *eai_narray_get_ref_##TYPE(EaiNArray_##TYPE *na, ...);                                    \
    TYPE eai_narray_set_##TYPE(EaiNArray_##TYPE *na, TYPE val, ...);                               \
    void eai_narray_deinit_##TYPE(EaiNArray_##TYPE *na);                                           \
    ulib_uint eai_narray_count_##TYPE(EaiNArray_##TYPE *na, ulib_uint axis);                       \
    ulib_uint eai_narray_axes_##TYPE(EaiNArray_##TYPE *na);                                        \
    UVec(ulib_uint) *eai_narray_shape_##TYPE(EaiNArray_##TYPE *na);                                \
    EaiNArray_loop_##TYPE p_eai_narray_init_loop_##TYPE(EaiNArray_##TYPE array);                   \
    void p_eai_narray_next_loop_##TYPE(EaiNArray_##TYPE array, EaiNArray_loop_##TYPE *iter);

/**
 * Implements a previously declared n dimensional array type
 * @param T array type.
 */
#define EAI_NARRAY_IMPL(TYPE)                                                                      \
    EaiNArray_##TYPE eai_narray_##TYPE(ulib_uint dims, ...)                                        \
    {                                                                                              \
        EaiNArray_##TYPE result;                                                                   \
        ulib_uint size = 1;                                                                        \
        result.storage = uvec(TYPE);                                                               \
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
        uvec_reserve(TYPE, &result.storage, size);                                                 \
        for(ulib_uint i = 0; i < size; i++) {                                                      \
            TYPE value;                                                                            \
            memset(&value, 0, sizeof(TYPE));                                                       \
            uvec_push(TYPE, &result.storage, value);                                               \
        }                                                                                          \
        uvec_shrink(TYPE, &result.storage);                                                        \
                                                                                                   \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    EaiNArray_##TYPE eai_narray_from_shape_##TYPE(UVec(ulib_uint) *shape)                          \
    {                                                                                              \
        EaiNArray_##TYPE result;                                                                   \
        ulib_uint size = 1;                                                                        \
        ulib_uint dims = 0;                                                                        \
        result.storage = uvec(TYPE);                                                               \
        result.shape = uvec(ulib_uint);                                                            \
                                                                                                   \
        uvec_foreach(ulib_uint, shape, dim){                                                       \
             dims += 1;                                                                            \
             size *= *dim.item;                                                                    \
             uvec_push(ulib_uint, &result.shape, *dim.item);                                       \
        }                                                                                          \
                                                                                                   \
        uvec_reserve(TYPE, &result.storage, size);                                                 \
        for(ulib_uint i = 0; i < size; i++) {                                                      \
            TYPE value;                                                                            \
            memset(&value, 0, sizeof(TYPE));                                                       \
            uvec_push(TYPE, &result.storage, value);                                               \
        }                                                                                          \
        uvec_shrink(TYPE, &result.storage);                                                        \
                                                                                                   \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    TYPE eai_narray_get_##TYPE(EaiNArray_##TYPE *na, ...)                                          \
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
        return uvec_get(TYPE, &na->storage, index);                                                \
    }                                                                                              \
                                                                                                   \
    TYPE *eai_narray_get_ref_##TYPE(EaiNArray_##TYPE *na, ...)                                     \
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
        TYPE *data = uvec_data(TYPE, &na->storage);                                                \
        return &data[index];                                                                       \
    }                                                                                              \
                                                                                                   \
    TYPE eai_narray_set_##TYPE(EaiNArray_##TYPE *na, TYPE val, ...)                                \
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
        old_val = uvec_get(TYPE, &na->storage, index);                                             \
        uvec_set(TYPE, &na->storage, index, val);                                                  \
        return old_val;                                                                            \
    }                                                                                              \
                                                                                                   \
    void eai_narray_deinit_##TYPE(EaiNArray_##TYPE *na)                                            \
    {                                                                                              \
        uvec_deinit(TYPE, &na->storage);                                                           \
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
    EaiNArray_loop_##TYPE p_eai_narray_init_loop_##TYPE(EaiNArray_##TYPE array)                    \
    {                                                                                              \
        EaiNArray_loop_##TYPE iter;                                                                \
                                                                                                   \
                                                                                                   \
                                                                                                   \
        return iter;                                                                               \
    }                                                                                              \
                                                                                                   \
    void p_eai_narray_next_loop_##TYPE(EaiNArray_##TYPE array, EaiNArray_loop_##TYPE *iter)        \
    {                                                                                              \
                                                                                                   \
    }                                                                                              \
                                                                                                   \
    TYPE eai_narray_get_from_coordinates_##TYPE(EaiNArray_##TYPE *na, UVec(ulib_uint) *c){         \
        ulib_uint index = 0;                                                                       \
                                                                                                   \
        uvec_foreach(ulib_uint, c, dim) {                                                          \
            ulib_uint size = *dim.item;                                                            \
            ulib_uint axis_size = uvec_get(ulib_uint, &na->shape, dim.i);                          \
            index = index * axis_size + size;                                                      \
        }                                                                                          \
                                                                                                   \
        return uvec_get(TYPE, &na->storage, index);                                                \
    }                                                                                              \
                                                                                                   \
    TYPE *eai_narray_get_ref_from_coordinates_##TYPE(EaiNArray_##TYPE *na, UVec(ulib_uint) *c){     \
        ulib_uint index = 0;                                                                       \
                                                                                                   \
        uvec_foreach(ulib_uint, c, dim) {                                                          \
            ulib_uint size = *dim.item;                                                            \
            ulib_uint axis_size = uvec_get(ulib_uint, &na->shape, dim.i);                          \
            index = index * axis_size + size;                                                      \
        }                                                                                          \
                                                                                                   \
        TYPE *data = uvec_data(TYPE, &na->storage);                                                \
        return &data[index];                                                                       \
    }                                                                                              \
                                                                                                   \
    TYPE eai_narray_set_from_coordinates_##TYPE(EaiNArray_##TYPE *na, TYPE val, UVec(ulib_uint) *c)\
    {                                                                                               \
        ulib_uint index = 0;                                                                       \
                                                                                                   \
        uvec_foreach(ulib_uint, c, dim) {                                                          \
            ulib_uint size = *dim.item;                                                            \
            ulib_uint axis_size = uvec_get(ulib_uint, &na->shape, dim.i);                          \
            index = index * axis_size + size;                                                      \
        }                                                                                          \
                                                                                                   \
        TYPE *data = uvec_data(TYPE, &na->storage);                                                \
        TYPE old_val = data[index];                                                                \
        data[index] = val;                                                                         \
        return old_val;                                                                            \
    }                                                                                              \
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
#define eai_narray_get(type, array, ...) eai_narray_get_##type(array, __VA_ARGS__)

/**
 * @param type the type of data stored in the elements of the array
 * @param na the n-dimensional array
 * @param ... coordinates of the element for each dimension
 * @return the memory address of the element indexed by the coordinates
 */
#define eai_narray_get_ref(type, array, ...) eai_narray_get_ref_##type(array, __VA_ARGS__)

/**
 * @param type the type of data stored in the elements of the array
 * @param na the n-dimensional array
 * @param val the value to set
 * @param ... the coordinates of the element for each dimension
 * @return the replaced value
 */
#define eai_narray_set(type, array, val, ...) eai_narray_set_##type(array, val, __VA_ARGS__)

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
#define eai_narray_get_from_coordinates(type, array, coordinates) eai_narray_get_from_coordinates_##type(array, coordinates)

/**
 * @param type the type of data stored in the elements of the array
 * @param array the n-dimensional array
 * @param coordinates a vector containing the coordinates of the element for each dimension
 * @return a reference to the element indexed by the coordinates
 */
#define eai_narray_get_ref_from_coordinates(type, array, coordinates) eai_narray_get_ref_from_coordinates_##type(array, coordinates)

/**
 * @param type the type of data stored in the elements of the array
 * @param array the n-dimensional array
 * @param val the value to insert into the array
 * @param coordinates a vector containing the coordinates of the element for each dimension
 * @return the value being replaced
 */
#define eai_narray_set_from_coordinates(type, array, val, coordinates) eai_narray_set_from_coordinates_##type(array, val, coordinates)

/**
 * @param type the type of data stored in the elements of the array
 * @param shape a vector containing the desidered shape
 * @return an instance of narray having the specified shape
 */
#define eai_narray_from_shape(type, shape) eai_narray_from_shape_##type(shape)


/**
 * Iterate through the first axis of the input array
 * @param type the type of the narray
 * @param array the array itself
 * @param iter [out] name of the iterator
 */
#define eai_narray_foreach(type, array, iter) for(EaiNArray_loop_##type iter = p_eai_narray_init_loop_##type(array);    \
                                                  iter.i != iter.count;                                                 \
                                                  eai_narray_next_loop_##type(array, &iter));

#endif
