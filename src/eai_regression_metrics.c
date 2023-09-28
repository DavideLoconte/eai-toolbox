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

#include "eai_regression_metrics.h"

EaiRegressionMetrics eai_regression_metrics(void)
{
    EaiRegressionMetrics metrics;
    metrics.count = 0;
    metrics.mae = 0;
    metrics.max_err = 0;
    metrics.min_err = 0;
    metrics.mse = 0;
    return metrics;
}

void eai_regression_metrics_add(EaiRegressionMetrics *metrics,
                                ulib_float actual,
                                ulib_float predicted)
{
    ulib_float abs_err = ulib_abs(actual - predicted);
    metrics->min_err = ulib_min(abs_err, metrics->min_err);
    metrics->max_err = ulib_max(abs_err, metrics->max_err);
    metrics->mse += abs_err * abs_err;
    metrics->mae += abs_err;
    metrics->count += 1;
}

ulib_float eai_regression_metrics_mse(EaiRegressionMetrics *metrics)
{
    return metrics->mse / metrics->count;
}

ulib_float eai_regression_metrics_rmse(EaiRegressionMetrics *metrics)
{
    return sqrt(metrics->mse / metrics->count);
}

ulib_float eai_regression_metrics_mae(EaiRegressionMetrics *metrics)
{
    return metrics->mae / metrics->count;
}

ulib_float eai_regression_metrics_max_error(EaiRegressionMetrics *metrics)
{
    return metrics->max_err;
}

ulib_float eai_regression_metrics_min_error(EaiRegressionMetrics *metrics)
{
    return metrics->min_err;
}

ulib_float eai_regression_metrics_count(EaiRegressionMetrics *metrics) { return metrics->count; }
