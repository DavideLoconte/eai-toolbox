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

#ifndef EAI_TOOLBOX_EAI_REGRESSION_METRICS_H
#define EAI_TOOLBOX_EAI_REGRESSION_METRICS_H

#include <ulib.h>

typedef struct EaiRegressionMetrics_s {
    ulib_float mse;
    ulib_float mae;
    ulib_float max_err;
    ulib_float min_err;
    ulib_uint count;
} EaiRegressionMetrics;

/**
 * Init a EaiRegressionMetrics structure for evaluating regression tasks
 * @return the EaiRegressionMetrics structure
 */
EaiRegressionMetrics eai_regression_metrics(void);

/**
 * Deinit metrics structure. Currently is a no-op
 * @param metrics
 */
void eai_regression_metrics_deinit(EaiRegressionMetrics *metrics);

/**
 * Add a prediction to the EaiRegressionMetrics structure
 * @param metrics the structure
 * @param actual the real value
 * @param predicted the predicted value
 */
void eai_regression_metrics_add(EaiRegressionMetrics *metrics,
                                ulib_float actual,
                                ulib_float predicted);

/**
 * @param metrics EaiRegressionMetrics instance
 * @return the Mean Squared Error
 */
ulib_float eai_regression_metrics_mse(EaiRegressionMetrics *metrics);

/**
 * @param metrics EaiRegressionMetrics instance
 * @return the Root Mean Squared Error
 */
ulib_float eai_regression_metrics_rmse(EaiRegressionMetrics *metrics);

/**
 * @param metrics EaiRegressionMetrics instance
 * @return the mean absolute error
 */
ulib_float eai_regression_metrics_mae(EaiRegressionMetrics *metrics);

/**
 * @param metrics EaiRegressionMetrics instance
 * @return the maximum absolute error
 */
ulib_float eai_regression_metrics_max_error(EaiRegressionMetrics *metrics);

/**
 * @param metrics EaiRegressionMetrics instance
 * @return the minimum absolute error
 */
ulib_float eai_regression_metrics_min_error(EaiRegressionMetrics *metrics);

/**
 * @param metrics EaiRegressionMetrics instance
 * @return the number of samples inserted into metrics
 */
ulib_float eai_regression_metrics_count(EaiRegressionMetrics *metrics);

#endif // EAI_TOOLBOX_EAI_REGRESSION_METRICS_H
