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

#ifndef EAI_TOOLBOX_EAI_K_FOLD_H
#define EAI_TOOLBOX_EAI_K_FOLD_H

#include "eai_csv_reader.h"
#include <ulib.h>

/**
 * Allows to read a resettable stream (such as a file stream or a string stream)
 * and subdivides it into k fold to perform evaluation
 */
typedef struct EaiKFold_s {
    UVec(ulib_byte) folds;
    ulib_byte k;
    ulib_uint i;
} EaiKFold;

/**
 * Do not use this function, use foreach macro instead
 * Start iteration
 * @param f the EaiKFold data structure
 * @param k the fold
 * @param reader csv reader
 * @param csv the csv stream
 * @param test true if you are iterating on testset samples
 * @return the first record
 */
UVec(UString) * eai_k_fold_csv_foreach_init(EaiKFold *f,
                                            ulib_byte k,
                                            EaiCsvReader *reader,
                                            UIStream *csv,
                                            bool test);

/**
 * Do not use this function, use foreach macro instead
 * Get next record
 * @param f the EaiKFold data structure
 * @param k the fold
 * @param reader csv reader
 * @param test true if you are iterating on trainset samples
 * @return the next record
 */
UVec(UString) *
    eai_k_fold_csv_foreach_next(EaiKFold *f, ulib_byte k, EaiCsvReader *reader, bool test);

/**
 * Iterate on trainset records from a input CSV stream
 * @param k_fold the EaiKFold data structure
 * @param reader a EaiCSVReader
 * @param csv a UIStream containing the input CSV file
 * @param fold the number of fold to iterate on
 * @param record [out] the iterator variable name to access the record inside the loop
 */
#define eai_k_fold_csv_foreach_train(k_fold, reader, csv, fold, record)                            \
    for(UVec(UString) *record = eai_k_fold_csv_foreach_init(k_fold, fold, reader, csv, false);     \
        record != NULL;                                                                            \
        record = eai_k_fold_csv_foreach_next(k_fold, fold, reader, false))

/**
 * Iterate on testset records from a input CSV stream
 * @param k_fold the EaiKFold data structure
 * @param reader a EaiCSVReader
 * @param csv a UIStream containing the input CSV file
 * @param fold the number of fold to iterate on
 * @param record [out] the iterator variable name to access the record inside the loop
 */
#define eai_k_fold_csv_foreach_test(k_fold, reader, csv, fold, record)                             \
    for(UVec(UString) *record = eai_k_fold_csv_foreach_init(k_fold, fold, reader, csv, true);      \
        record != NULL;                                                                            \
        record = eai_k_fold_csv_foreach_next(k_fold, fold, reader, true))

/**
 * Create a k fold instance
 * @param k the number of folds
 * @return k fold instance
 */
EaiKFold eai_k_fold(ulib_byte k);

/**
 * Deinit existing k fold instance
 * @param f the k fold instance
 */
void eai_k_fold_deinit(EaiKFold *f);

#endif // EAI_TOOLBOX_EAI_K_FOLD_H
