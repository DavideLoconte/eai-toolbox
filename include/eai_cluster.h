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

#ifndef EAI_TOOLBOX_EAI_CLUSTER_H
#define EAI_TOOLBOX_EAI_CLUSTER_H

#include "eai_model.h"

/**
 * @param n_cluster the number of clusters
 * @param max_iter the maximum number of iterations
 * @param seed seed for RNG
 * @return EaiModel for KMeans
 */
EaiModel eai_cluster_kmeans(ulib_uint n_cluster, ulib_uint max_iter, ulib_uint seed);

/**
 * @param model a trained kmeans model
 * @return a vector of integers, where the i-th elements contains the cluster id of the i-th training
 *         sample
 * @note null is returned if model is not trained
 * @note the returned vector has the same lifetime as EaiModel and should not manually de-inited
 */
UVec(ulib_uint) *eai_cluster_kmeans_clusters(EaiModel *model);

/**
 * @param model a trained kmeans model
 * @return a narray of floats, where the i-th row is the centroid of the cluster having i as cluster id
 * @note null is returned if model is not trained
 * @note the returned narray has the same lifetime as EaiModel and should not manually de-inited
 */
EaiNArray(ulib_float) *eai_cluster_kmeans_centroids(EaiModel *model);

/**
 * @param n_cluster the number of clusters
 * @param max_iter the maximum number of iterations
 * @param seed seed for RNG
 * @return EaiModel for KMedoids
 */
EaiModel eai_cluster_kmedoids(ulib_uint n_cluster, ulib_uint max_iter, ulib_uint seed);

/**
 * @param model a trained kmeans model
 * @return a vector of integers, where the i-th elements contains the cluster id of the i-th training
 *         sample
 * @note null is returned if model is not trained
 * @note the returned vector has the same lifetime as EaiModel and should not manually de-inited
 */
UVec(ulib_uint) *eai_cluster_kmedoids_clusters(EaiModel *model);

/**
 * @param model a trained kmeans model
 * @return a narray of floats, where the i-th row is the centroid of the cluster having i as cluster id
 * @note null is returned if model is not trained
 * @note the returned narray has the same lifetime as EaiModel and should not manually de-inited
 */
EaiNArray(ulib_float) *eai_cluster_kmedoids_centroids(EaiModel *model);

#endif
