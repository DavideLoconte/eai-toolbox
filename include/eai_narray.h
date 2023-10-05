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

#define TYPE ulib_float

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
    EaiNArray_##TYPE eai_narray_##TYPE(ulib_uint dimensions, ...);                                 \
    TYPE eai_narray_get_##TYPE(EaiNArray_##TYPE *na, ...);                                         \
    TYPE *eai_narray_get_ref_##TYPE(EaiNArray_##TYPE *na, ...);                                    \
    TYPE eai_narray_set_##TYPE(EaiNArray_##TYPE *na, ...);                                         \
    void eai_narray_deinit_##TYPE(EaiNArray_##TYPE *na);

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
        return &uvec_get(TYPE, &na->storage, index);                                               \
    }                                                                                              \
                                                                                                   \
    TYPE eai_narray_set_##TYPE(EaiNArray_##TYPE *na, ...)                                          \
    {                                                                                              \
        ulib_uint index = 0;                                                                       \
        TYPE old_val;                                                                              \
                                                                                                   \
        va_list argptr;                                                                            \
        va_start(argptr, na);                                                                      \
                                                                                                   \
        uvec_foreach(ulib_uint, &na->shape, dim) {                                                 \
            index = index * (*dim.item) + va_arg(argptr, ulib_uint);                               \
        }                                                                                          \
                                                                                                   \
        old_val = uvec_get(TYPE, &na->storage, index);                                             \
        uvec_set(TYPE, &na->storage, index, va_arg(argptr, TYPE));                                 \
        va_end(argptr);                                                                            \
                                                                                                   \
        return old_val;                                                                            \
    }                                                                                              \
                                                                                                   \
    void eai_narray_deinit_##TYPE(EaiNArray_##TYPE *na)                                            \
    {                                                                                              \
        uvec_deinit(TYPE, &na->storage);                                                           \
        uvec_deinit(ulib_uint, &na->shape);                                                        \
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
 * @param ... a list of n+1 arguments, where the first n are the coordinates of
 *            the element for each dimension and the n+1-th element is the value
 *            to set
 * @return the replaced value
 */
#define eai_narray_set(type, array, ...) eai_narray_set_##type(array, __VA_ARGS__)

/**
 * Deinit a n-dimensional array
 * @param type the type of data stored in the elements of the array
 * @param na the array
 */
#define eai_narray_deinit(type, array) eai_narray_deinit_##type(array)


#endif
