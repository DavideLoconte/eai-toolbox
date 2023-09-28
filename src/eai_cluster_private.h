#ifndef EAI_TOOLBOX_EAI_CLUSTER_PRIVATE_H
#define EAI_TOOLBOX_EAI_CLUSTER_PRIVATE_H

#include "eai_cluster.h"

/**
 * Init clustering results 
 * @param result the clustering results data structure
 * @param data the data
 * @param n_cluster the number of points to select
 */
void eai_init_clustering(EaiClusterResults *results, UVec(UVec(ulib_float)) *data);

/**
 * Fills Cluster results with randomly selected points from data
 * @param result the clustering results data structure
 * @param data the data
 * @param points the number of points to select
 */
void eai_select_random_centroids(EaiClusterResults *results, UVec(UVec(ulib_float)) *data, ulib_uint n_cluster);

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


#endif