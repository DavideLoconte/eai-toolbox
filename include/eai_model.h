//
// Created by DavideLoconte on 07-Feb-24.
//

#ifndef EAI_TOOLBOX_EAI_MODEL_H
#define EAI_TOOLBOX_EAI_MODEL_H

#include "eai_narray_builtin.h"

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


#endif // EAI_TOOLBOX_EAI_MODEL_H
