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

#include "eai_k_fold.h"

/**
 * Append additional records at the end of the folds vector
 * @param f k fold instance
 */
static void eai_k_fold_append_vec(EaiKFold *f);

/**
 * Return the fold of the nth record. The fold is a number between 0 and k
 * indicating in which fold the sample belongs to the testset
 * @param f the KFold instance
 * @param idx the index
 * @return the fold
 */
static ulib_byte eai_k_fold_get_fold(EaiKFold *f, ulib_uint idx);

UVec(UString) * eai_k_fold_csv_foreach_init(EaiKFold *f,
                                            ulib_byte k,
                                            EaiCsvReader *reader,
                                            UIStream *csv,
                                            bool test)
{
    UVec(UString) *result = eai_csv_reader_start(reader, csv);
    f->i = 0;

    while(result != NULL) {
        ulib_uint fold = eai_k_fold_get_fold(f, f->i);
        if((fold != k && !test) || (fold == k && test)) {
            return result;
        }
        result = eai_csv_reader_next(reader);
        f->i += 1;
    }

    return NULL;
}

UVec(UString) *
    eai_k_fold_csv_foreach_next(EaiKFold *f, ulib_byte k, EaiCsvReader *reader, bool test)
{
    UVec(UString) *result = eai_csv_reader_next(reader);
    f->i += 1;

    while(result != NULL) {
        ulib_uint fold = eai_k_fold_get_fold(f, f->i);
        if((fold != k && !test) || (fold == k && test)) {
            return result;
        }
        result = eai_csv_reader_next(reader);
        f->i += 1;
    }

    return NULL;
}

EaiKFold eai_k_fold(ulib_byte k)
{
    EaiKFold result;
    result.k = k;
    result.folds = uvec(ulib_byte);
    result.i = 0;
    return result;
}

void eai_k_fold_deinit(EaiKFold *f) { uvec_deinit(ulib_byte, &f->folds); }

// Private impl ====================================================================================

static void eai_k_fold_append_vec(EaiKFold *f)
{
    UVec(ulib_byte) vec = uvec(ulib_byte);

    for(ulib_uint i = 0; i < f->k; i++) {
        uvec_push(ulib_byte, &vec, i);
    }

    uvec_foreach(ulib_byte, &vec, x) {
        ulib_uint y_idx = x.i + (urand() % (f->k - x.i));
        ulib_byte tmp = uvec_get(ulib_byte, &vec, y_idx);
        uvec_set(ulib_byte, &vec, y_idx, *x.item);
        *x.item = tmp;
    }

    uvec_append(ulib_byte, &f->folds, &vec);
    uvec_deinit(ulib_byte, &vec);
}

static ulib_byte eai_k_fold_get_fold(EaiKFold *f, ulib_uint idx)
{
    while(!uvec_index_is_valid(ulib_byte, &f->folds, idx)) {
        eai_k_fold_append_vec(f);
    }
    return uvec_get(ulib_byte, &f->folds, idx);
}
