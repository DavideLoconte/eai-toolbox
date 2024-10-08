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

#ifndef EAI_TOOLBOX_EAI_MODEL_H
#define EAI_TOOLBOX_EAI_MODEL_H

#include "eai_narray_builtin.h"
#include "eai_compat.h"

EAI_BEGIN_DECLS

/**
 * This is a standard interface for models.
 * EaiModels returns this model which can be used with the standard function below
 */
typedef struct EaiModel_s {
    /**
     * Pointer to a function that train the ML model and discard any previous
     * training state
     * @param ctx pointer to the model
     * @param X the training data
     * @param y the label (if any)
     */
    void (*fit)(void *, EaiNArray(ulib_float) *, ...);

    /**
     * @param ctx pointer to the model
     * @param X the training data
     * @return the prediction results
     */
    EaiNArray(ulib_float) (*predict)(void *, EaiNArray(ulib_float) *);

    /**
     * @param ctx pointer to the model
     * @param X the training data
     * @param y the label (if any)
     * @return a score indicating the accuracy/performance of the model (higher is better)
     */
    ulib_float (*score)(void *, EaiNArray(ulib_float) *, ...);

    /**
     * Pointer to a function that performs an incremental model update with the input data
     * @param ctx pointer to the model
     * @param X the training data
     * @param y the label (if any)
     */
    void (*partial_fit)(void *, EaiNArray(ulib_float) *, ...);

    /**
     * Deinit model resources
     * @param ctx pointer to model
     */
    void (*deinit)(void *ctx);

    void *ctx;
} EaiModel;

/**
 * Fit the given model with the input training data
 * @param model the model
 * @param X the input data
 * @param ... label to fit (if any)
 */
#define eai_model_fit(model, ...) (model)->fit((model)->ctx, __VA_ARGS__)

/**
 * Predict the input data with the given model
 * @param model the model
 * @param X the input data
 * @note the returned EaiNArray must be manually de-inited
 */
#define eai_model_predict(model, X) (model)->predict((model)->ctx, X)

/**
 * Predict the input data with the given model
 * @param model the model
 * @param X the input data
 * @param ... label to fit (if any)
 */
#define eai_model_score(model, ...) (model)->score((model)->ctx, __VA_ARGS__)

/**
 * Predict the input data with the given model
 * @param model the model
 * @param X the input data
 * @param ... label to fit (if any)
 */
#define eai_model_partial_fit(model, ...) (model)->partial_fit((model)->ctx, __VA_ARGS__)

/**
 * Deinit the model
 * @param model the model to de-initialize
 */
#define eai_model_deinit(model) (model)->deinit((model)->ctx)

EAI_END_DECLS

#endif // EAI_TOOLBOX_EAI_MODEL_H
