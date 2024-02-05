#ifndef EAI_TOOLBOX_EAI_CLUSTER_PRIVATE_H
#define EAI_TOOLBOX_EAI_CLUSTER_PRIVATE_H

#include "eai_cluster.h"

/**
 * Init clustering results
 * @param result the clustering results data structure
 * @param data the data
 * @param n_cluster the number of points to select
 */
void eai_init_clustering(EaiClusterResults *results,
                         EaiNArray(ulib_float) * data,
                         ulib_uint n_cluster);

/**
 * Fills Cluster results with randomly selected points from data
 * @param result the clustering results data structure
 * @param data the data
 * @param points the number of points to select
 */
void eai_select_random_centroids(EaiClusterResults *results,
                                 EaiNArray(ulib_float) *data,
                                 ulib_uint n_cluster);

/**
 * Compute the euclidean distance between two points
 * @param a the first point
 * @param b the second point
 * @return the euclidean distance between two points
 */
ulib_float eai_distance(UVec(ulib_float) *a, UVec(ulib_float) *b);

/**
 * Compute the squared euclidean distance between two points
 * @param a the first point
 * @param b the second point
 * @return the euclidean distance between two points
 */
ulib_float eai_squared_distance(UVec(ulib_float) *a, UVec(ulib_float) *b);

/**
 * Assing each data point to its cluster
 * @param result the clustering results data structure
 * @param data the data
 */
void eai_assign_clusters(EaiClusterResults *results, EaiNArray(ulib_float) *data);

/**
 * Return the number of data point
 * @param data the data
 * @return the number of points
 */
ulib_uint eai_cluster_get_data_count(EaiNArray(ulib_float) *data);

/**
 * Return the number of data point
 * @param data the data
 * @return the number of points
 */
ulib_uint eai_cluster_get_data_shape(EaiNArray(ulib_float) *data);

#endif
