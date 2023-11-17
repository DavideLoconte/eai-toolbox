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

#ifndef EAI_TOOLBOX_EAI_CONFUSION_MATRIX_H
#define EAI_TOOLBOX_EAI_CONFUSION_MATRIX_H

#include <ulib.h>

#define EAI_CONFUSION_MATRIX_VEC_ERR ubit_bit(8, 0)
#define EAI_CONFUSION_MATRIX_ERR ubit_bit(8, 1)
#define EAI_CONFUSION_MATRIX_NOT_FOUND_ERR ubit_bit(8, 2)

typedef UVec(ulib_uint) EaiConfusionMatrixRow;
UVEC_DECL(EaiConfusionMatrixRow)

typedef struct EaiConfusionMatrix_s {
    UVec(EaiConfusionMatrixRow) matrix;
    UBit(8) flags;
} EaiConfusionMatrix;

/**
 * Instantiate a confusion matrix
 * @return a confusion matrix instance
 */
EaiConfusionMatrix eai_confusion_matrix(void);

/**
 * Destroy a confusion matrix and release allocated memory
 * @param matrix the confusion matrix
 */
void eai_confusion_matrix_deinit(EaiConfusionMatrix *matrix);

/**
 * Add a element to the confusion matrix
 * @param matrix the confusion matrix
 * @param actual the actual class
 * @param predicted the predicted class
 */
void eai_confusion_matrix_add(EaiConfusionMatrix *matrix, ulib_uint actual, ulib_uint predicted);

/**
 * Delete an element from the confusion matrix
 * @param matrix the matrix
 * @param actual the actual class
 * @param predicted the predicted one
 */
void eai_confusion_matrix_del(EaiConfusionMatrix *matrix, ulib_uint actual, ulib_uint predicted);

/**
 * Get an element from the confusion matrix
 * @param matrix the matrix
 * @param actual the actual class
 * @param predicted the predicted one
 */
ulib_uint eai_confusion_matrix_get(EaiConfusionMatrix *matrix,
                                   ulib_uint actual,
                                   ulib_uint predicted);

/**
 * Accuracy from confusion matrix
 * @param matrix the confusion matrix
 * @return the accuracy of the confusion matrix
 */
ulib_float eai_confusion_matrix_get_accuracy(EaiConfusionMatrix *matrix);

/**
 * Get precision of a specific class from confusion matrix
 * @param matrix the confusion matrix
 * @param predicted the class for which computing the precision value. In binary classification
 *                  tasks, this value should represent the positive outcome
 * @return the precision
 */
ulib_float eai_confusion_matrix_get_precision(EaiConfusionMatrix *matrix, ulib_uint predicted);

/**
 * Get recall of a specific class from confusion matrix
 * @param matrix the confusion matrix
 * @param predicted the class for which computing the precision value. In binary classification
 *                  tasks, this value should represent the positive outcome
 * @return the recall
 */
ulib_float eai_confusion_matrix_get_recall(EaiConfusionMatrix *matrix, ulib_uint predicted);

/**
 * Get f1 score of a specific class from confusion matrix
 * @param matrix the confusion matrix
 * @param predicted the class for which computing the precision value. In binary classification
 *                  tasks, this value should represent the positive outcome
 * @param beta  the value of beta. > 1 values weights more the recall term, < 1 weights more the
 * precision term
 * @return the f1 score
 */
ulib_float eai_confusion_matrix_get_fb_score(EaiConfusionMatrix *matrix,
                                             ulib_uint predicted,
                                             ulib_float beta);

/**
 * Get mean precision from confusion matrix
 * @param matrix the confusion matrix
 * @return mean precision
 */
ulib_float eai_confusion_matrix_get_mean_precision(EaiConfusionMatrix *matrix);

/**
 * Get mean recall from confusion matrix
 * @param matrix the confusion matrix
 * @return mean recall
 */
ulib_float eai_confusion_matrix_get_mean_recall(EaiConfusionMatrix *matrix);

/**
 * Get mean f1 score from confusion matrix
 * @param matrix the confusion matrix
 * @param beta  the value of beta. > 1 values weights more the recall term, < 1 weights more the
 * precision term
 * @return mean f1 score
 */
ulib_float eai_confusion_matrix_get_mean_fb_score(EaiConfusionMatrix *matrix, ulib_float beta);

/**
 * Generate a pretty string from the confusion matrix
 * @param matrix the matrix
 * @param symbols a vector that holds the names for each class
 *                user is responsible of allocating and deallocating it
 * @return a UString formatted for logging confusion matrix. User is responsible of deallocating it
 */
UString eai_confusion_matrix_to_string(EaiConfusionMatrix *matrix, UVec(UString) * symbols);

/**
 * Generate a pretty string for logging metrics (accuracy, precision, recall, f1)
 * @param matrix the matrix
 * @param symbols a vector that holds the names for each class
 *                user is responsible of allocating and deallocating it
 * @return a UString formatted for logging confusion matrix metrics. User is responsible of
 * deallocating it
 */
UString eai_confusion_matrix_metrics_to_string(EaiConfusionMatrix *matrix, UVec(UString) * symbols);

#endif // EAI_TOOLBOX_EAI_CONFUSION_MATRIX_H
