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

#include "eai_confusion_matrix.h"
#include "eai_math.h"

#define EAI_MAX(x, y) (x > y ? x : y)

UVEC_IMPL(EaiConfusionMatrixRow)

EaiConfusionMatrix eai_confusion_matrix(void)
{
    EaiConfusionMatrix result;
    result.flags = 0x00;
    result.matrix = uvec(EaiConfusionMatrixRow);
    return result;
}

void eai_confusion_matrix_deinit(EaiConfusionMatrix *matrix)
{
    uvec_foreach(EaiConfusionMatrixRow, &matrix->matrix, row) {
        uvec_deinit(ulib_uint, row.item);
    }
    uvec_deinit(EaiConfusionMatrixRow, &matrix->matrix);
}

void eai_confusion_matrix_add(EaiConfusionMatrix *matrix, ulib_uint actual, ulib_uint predicted)
{
    UVec(ulib_uint) row;

    while(EAI_MAX(actual, predicted) >= uvec_count(EaiConfusionMatrixRow, &matrix->matrix)) {
        row = uvec(ulib_uint);
        uvec_ret ret = uvec_push(EaiConfusionMatrixRow, &matrix->matrix, row);
        if(ret != UVEC_OK) {
            matrix->flags = ubit_set(8, matrix->flags, EAI_CONFUSION_MATRIX_VEC_ERR);
            return;
        }
    }
    row = uvec_get(EaiConfusionMatrixRow, &matrix->matrix, actual);

    while(predicted >= uvec_count(ulib_uint, &row)) {
        uvec_ret ret = uvec_push(ulib_uint, &row, 0);
        if(ret != UVEC_OK) {
            matrix->flags = ubit_set(8, matrix->flags, EAI_CONFUSION_MATRIX_VEC_ERR);
            return;
        }
    }

    ulib_uint new = uvec_get(ulib_uint, &row, predicted) + 1;
    uvec_set(ulib_uint, &row, predicted, new);
    uvec_set(EaiConfusionMatrixRow, &matrix->matrix, actual, row);
}

void eai_confusion_matrix_del(EaiConfusionMatrix *matrix, ulib_uint actual, ulib_uint predicted)
{
    if(actual >= uvec_count(EaiConfusionMatrixRow, &matrix->matrix)) {
        matrix->flags = ubit_set(8, matrix->flags, EAI_CONFUSION_MATRIX_NOT_FOUND_ERR);
        return;
    }

    UVec(ulib_uint) row = uvec_get(EaiConfusionMatrixRow, &matrix->matrix, actual);

    if(predicted >= uvec_count(ulib_uint, &row)) {
        matrix->flags = ubit_set(8, matrix->flags, EAI_CONFUSION_MATRIX_NOT_FOUND_ERR);
        return;
    }

    ulib_uint new = uvec_get(ulib_uint, &row, predicted) - 1;
    uvec_set(ulib_uint, &row, predicted, new);
    uvec_set(EaiConfusionMatrixRow, &matrix->matrix, actual, row);
}

ulib_uint eai_confusion_matrix_get(EaiConfusionMatrix *matrix,
                                   ulib_uint actual,
                                   ulib_uint predicted)
{
    if(actual >= uvec_count(EaiConfusionMatrixRow, &matrix->matrix)) {
        matrix->flags = ubit_set(8, matrix->flags, EAI_CONFUSION_MATRIX_NOT_FOUND_ERR);
        return -1;
    }
    UVec(ulib_uint) row = uvec_get(EaiConfusionMatrixRow, &matrix->matrix, actual);

    if(predicted >= uvec_count(ulib_uint, &row)) {
        return 0;
    }

    return uvec_get(ulib_uint, &row, predicted);
}

ulib_float eai_confusion_matrix_get_accuracy(EaiConfusionMatrix *matrix)
{
    ulib_uint correct = 0;
    ulib_uint wrong = 0;

    uvec_foreach(EaiConfusionMatrixRow, &matrix->matrix, row) {
        uvec_foreach(ulib_uint, row.item, value) {
            if(value.i == row.i) {
                correct += *value.item;
            } else {
                wrong += *value.item;
            }
        }
    }

    if(correct + wrong == 0)
        return 0;
    return ((ulib_float) correct) / ((ulib_float) (correct + wrong));
}

ulib_float eai_confusion_matrix_get_precision(EaiConfusionMatrix *matrix, ulib_uint predicted)
{
    ulib_uint tp = 0;
    ulib_uint fp = 0;
    uvec_foreach(EaiConfusionMatrixRow, &matrix->matrix, row) {
        uvec_foreach(ulib_uint, row.item, value) {
            if(value.i != predicted)
                continue;

            if(value.i == row.i) {
                tp += *value.item;
            } else {
                fp += *value.item;
            }
        }
    }
    if((tp + fp) == 0)
        return 0.0;
    return ((ulib_float) tp) / ((ulib_float) (tp + fp));
}

ulib_float eai_confusion_matrix_get_recall(EaiConfusionMatrix *matrix, ulib_uint predicted)
{
    ulib_uint tp = 0;
    ulib_uint fn = 0;
    uvec_foreach(EaiConfusionMatrixRow, &matrix->matrix, row) {
        uvec_foreach(ulib_uint, row.item, value) {
            if(row.i != predicted)
                continue;
            if(value.i == row.i) {
                tp += *value.item;
            } else {
                fn += *value.item;
            }
        }
    }
    if((tp + fn) == 0)
        return 0.0;
    return ((ulib_float) tp) / ((ulib_float) (tp + fn));
}

ulib_float eai_confusion_matrix_get_fb_score(EaiConfusionMatrix *matrix,
                                             ulib_uint predicted,
                                             ulib_float beta)
{
    ulib_uint tp = 0;
    ulib_uint fn = 0;
    ulib_uint fp = 0;
    beta *= beta;

    uvec_foreach(EaiConfusionMatrixRow, &matrix->matrix, row) {
        uvec_foreach(ulib_uint, row.item, value) {
            if(value.i == row.i && row.i == predicted) {
                tp += *value.item;
            } else if(row.i == predicted) {
                fn += *value.item;
            } else if(value.i == predicted) {
                fp += *value.item;
            }
        }
    }

    if((tp + fp) == 0)
        return 0.0;
    ulib_float p = ((ulib_float) tp) / ((ulib_float) (tp + fp));
    if((tp + fn) == 0)
        return 0.0;
    ulib_float r = ((ulib_float) tp) / ((ulib_float) (tp + fn));
    if(((beta * p) + r) == 0.0)
        return 0.0;
    return (1.0 + beta) * ((p * r) / ((beta * p) + r));
}

ulib_float eai_confusion_matrix_get_mean_precision(EaiConfusionMatrix *matrix)
{
    ulib_float sum = 0;
    ulib_uint count = 0;
    uvec_foreach(EaiConfusionMatrixRow, &matrix->matrix, row) {
        sum += eai_confusion_matrix_get_precision(matrix, row.i);
        count += 1;
    }
    if(count == 0)
        return 0.0;
    return sum / (ulib_float) count;
}

ulib_float eai_confusion_matrix_get_mean_recall(EaiConfusionMatrix *matrix)
{
    ulib_float sum = 0;
    ulib_uint count = 0;
    uvec_foreach(EaiConfusionMatrixRow, &matrix->matrix, row) {
        sum += eai_confusion_matrix_get_recall(matrix, row.i);
        count += 1;
    }
    if(count == 0)
        return 0.0;
    return sum / (ulib_float) count;
}

ulib_float eai_confusion_matrix_get_mean_fb_score(EaiConfusionMatrix *matrix, ulib_float beta)
{
    ulib_float sum = 0;
    ulib_uint count = 0;
    uvec_foreach(EaiConfusionMatrixRow, &matrix->matrix, row) {
        sum += eai_confusion_matrix_get_fb_score(matrix, row.i, beta);
        count += 1;
    }
    if(count == 0)
        return 0.0;
    return sum / (ulib_float) count;
}

static char eai_confusion_matrix_rotate_placeholder(char placeholder)
{
    placeholder++;
    if(placeholder == ('z' + 1))
        placeholder = 'A';
    else if(placeholder == ('Z' + 1))
        placeholder = '0';
    else if(placeholder == ('9' + 1))
        placeholder = '!';
    return placeholder;
}

UString eai_confusion_matrix_to_string(EaiConfusionMatrix *matrix, UVec(UString) * symbols)
{
    UStrBuf buf = ustrbuf();
    char placeholder = 'a';
    ustrbuf_append_literal(&buf, "|");

    uvec_foreach(EaiConfusionMatrixRow, &matrix->matrix, row) {
        ustrbuf_append_format(&buf, "     %c     |", placeholder);
        placeholder = eai_confusion_matrix_rotate_placeholder(placeholder);
    }

    ustrbuf_append_literal(&buf, "   <-- Classified as\n");
    ulib_uint row_length = ustrbuf_length(&buf);

    for(ulib_uint i = 0; i < row_length; i++) {
        ustrbuf_append_literal(&buf, "-");
    }
    ustrbuf_append_literal(&buf, "\n");

    placeholder = 'a';
    for(ulib_uint i = 0; i < uvec_count(EaiConfusionMatrixRow, &matrix->matrix); i++) {
        ustrbuf_append_literal(&buf, ":");
        for(ulib_uint j = 0; j < uvec_count(EaiConfusionMatrixRow, &matrix->matrix); j++) {
            ulib_uint value = eai_confusion_matrix_get(matrix, i, j);
            ulib_uint numbers = value > 1000000000 ? 9 : eai_int_log10((ulib_int) value) + 1;

            if(5 >= (numbers / 2)) {
                for(ulib_uint k = 0; k < 5 - (numbers / 2); k++) {
                    ustrbuf_append_literal(&buf, " ");
                }
            }

            ustrbuf_append_format(&buf, "%d", value);

            if(5 >= (numbers / 2)) {
                for(ulib_uint k = 0; k < 5 - (numbers / 2); k++) {
                    ustrbuf_append_literal(&buf, " ");
                }
                if(numbers % 2 == 0)
                    ustrbuf_append_literal(&buf, " ");
            }
            ustrbuf_append_literal(&buf, ":");
        }
        ustrbuf_append_format(&buf, "   %c", placeholder);
        placeholder = eai_confusion_matrix_rotate_placeholder(placeholder);

        if(symbols && uvec_index_is_valid(UString, symbols, i)) {
            UString str = uvec_get(UString, symbols, i);
            ustrbuf_append_literal(&buf, " = ");
            ustrbuf_append_ustring(&buf, str);
        }

        ustrbuf_append_literal(&buf, "\n");
    }

    return ustrbuf_to_ustring(&buf);
}

UString eai_confusion_matrix_metrics_to_string(EaiConfusionMatrix *matrix, UVec(UString) * symbols)
{
    char placeholder = 'a';
    UStrBuf buf = ustrbuf();
    ustrbuf_append_literal(&buf, "|Precision|Recall   |F1-Score |Accuracy |\n");
    ustrbuf_append_literal(&buf, "-----------------------------------------\n");
    uvec_foreach(EaiConfusionMatrixRow, &matrix->matrix, row) {
        ustrbuf_append_format(&buf,
                              "|  %.3f  |  %.3f  |  %.3f  |         |   ",
                              eai_confusion_matrix_get_precision(matrix, row.i),
                              eai_confusion_matrix_get_recall(matrix, row.i),
                              eai_confusion_matrix_get_fb_score(matrix, row.i, 1));
        if(symbols && uvec_index_is_valid(UString, symbols, row.i)) {
            UString str = uvec_get(UString, symbols, row.i);
            ustrbuf_append_ustring(&buf, str);
        } else {
            ustrbuf_append_format(&buf, "%c", placeholder);
            placeholder = eai_confusion_matrix_rotate_placeholder(placeholder);
        }
        ustrbuf_append_literal(&buf, "\n");
    }

    ustrbuf_append_literal(&buf, "-----------------------------------------\n");
    ustrbuf_append_format(&buf,
                          "|  %.3f  |  %.3f  |  %.3f  |  %.3f  |   Average",
                          eai_confusion_matrix_get_mean_precision(matrix),
                          eai_confusion_matrix_get_mean_recall(matrix),
                          eai_confusion_matrix_get_mean_fb_score(matrix, 1),
                          eai_confusion_matrix_get_accuracy(matrix));

    ustrbuf_append_literal(&buf, "\n");
    return ustrbuf_to_ustring(&buf);
}
