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

#include <ulib.h>

UVEC_DECL(UVec(ulib_float))

typedef struct EaiClusterResults_s {
    UVec(UVec(ulib_float)) centroids;
    UVec(ulib_uint) cluster;
    UVec(ulib_uint) cluster_size;
} EaiClusterResults;

typedef struct EaiKMeansConfig_s {
    ulib_uint max_iter;
    ulib_uint n_clusters;
    ulib_uint seed;
} EaiKMeansConfig;

typedef struct EaiKMedoidsConfig_s {
    ulib_uint max_iter;
    ulib_uint n_clusters; 
    ulib_uint seed;
} EaiKMedoidsConfig;

/**
 * @return an instance of EaiKMeansConfig initialized with default values
 */
EaiKMeansConfig eai_cluster_kmeans_config(void);

/**
 * Execute kmeans on the supplied data matrix 
 * @param config algorithm configuration
 * @param data dataset to cluster
 * @return the results of the clustering
 */
EaiClusterResults eai_cluster_kmeans(EaiKMeansConfig *config, UVec(UVec(ulib_float)) *data);


/**
 * @return an instance of EaiKMedoidsConfig initialized with default values
 */
EaiKMedoidsConfig eai_cluster_kmedoids_config(void);

/**
 * Execute kmedoids on the supplied data matrix 
 * @param config algorithm configuration
 * @param data dataset to cluster
 * @return the results of the clustering
 */ 
EaiClusterResults eai_cluster_kmedoids(EaiKMedoidsConfig *config, UVec(UVec(ulib_float)) *data);

/**
 * Free up memory allocated for EaiClusterResults structure
 * @param results the result
 */
void eai_cluster_deinit(EaiClusterResults *results);


#endif
