#include <stdlib.h>

#include "eai_cluster_private.h"

/**
 * Assing each data point to its cluster
 * @param result the clustering results data structure
 * @param data the data
 */
static void assign_clusters(EaiClusterResults *results, UVec(UVec(ulib_float)) *data);

/**
 * Assing each data point to its cluster
 * @param result the clustering results data structure
 * @param data the data
 * @return the Frobenius norm of the difference in the cluster centers before and after the update 
 */
static ulib_float update_kmeans(EaiClusterResults *results, UVec(UVec(ulib_float)) *data);

// Public impl ================================================================= 

EaiKMeansConfig eai_cluster_kmeans_config(void)
{
    EaiKMeansConfig result;
    result.max_iter = 300;
    result.n_clusters = 8;
    result.seed = 0;
    return result;
}

EaiClusterResults eai_cluster_kmeans(EaiKMeansConfig *config, UVec(UVec(ulib_float)) *data)
{
    EaiClusterResults results;
    eai_init_clustering(&results, data);
    srand(config->seed); 
    eai_select_random_centroids(&results, data, config->n_clusters);

    for (ulib_uint i = 0; i < config->max_iter; i++) {
        assign_clusters(&results, data);
        update_kmeans(&results, data);
    }

    return results;
}

void eai_cluster_deinit(EaiClusterResults *results)
{
    uvec_foreach(UVec(ulib_float), &results->centroids, centroid) {
        uvec_deinit(ulib_float, centroid.item);
    }
    uvec_deinit(UVec(ulib_float), &results->centroids);
    uvec_deinit(ulib_uint, &results->cluster_size);
    uvec_deinit(ulib_uint, &results->cluster);
}

// Private impl ===============================================================

UVEC_IMPL(UVec(ulib_float))

void assign_clusters(EaiClusterResults *results, UVec(UVec(ulib_float)) *data)
{
    ulib_uint n_clusters = uvec_count(ulib_uint, &results->cluster);
    
    uvec_foreach(ulib_uint, &results->cluster_size, value) {
        *value.item = 0;
    }
    
    uvec_foreach(UVec(ulib_float), data, sample) {
        ulib_uint min_cluster;
        ulib_float min_distance = ULIB_FLOAT_MAX;
        
        for (ulib_uint i = 0; i < n_clusters; i++) {
            UVec(ulib_float) *centroid = &uvec_get(UVec(ulib_float), &results->centroids, i);
            ulib_float d = eai_distance(sample.item, centroid);
            if (d < min_distance) {
                min_distance = d;
                min_cluster = i;
            }
        } 
        ulib_uint previous_cluster_size = uvec_get(ulib_uint, &results->cluster_size, min_cluster);
        uvec_set(ulib_uint, &results->cluster, sample.i, min_cluster);
        uvec_set(ulib_uint, &results->cluster_size, min_cluster, previous_cluster_size + 1);
    }
}

void reinit_centroids(EaiClusterResults *results) 
{
    uvec_foreach(UVec(ulib_float), &results->centroids, _centroid) { 
        ulib_uint cluster_size = uvec_get(ulib_uint, &results->cluster_size, _centroid.i);
        if (cluster_size == 0) continue; // Do not update cluster with size 0
        uvec_foreach(ulib_float, _centroid.item, x) {
            *x.item = 0;
        }
    } 
}

ulib_float update_kmeans(EaiClusterResults *results, UVec(UVec(ulib_float)) *data)
{
    ulib_float update = 0.0;
    reinit_centroids(results);

    uvec_foreach(UVec(ulib_float), data, sample) {
        ulib_uint sample_cluster = uvec_get(ulib_uint, &results->cluster, sample.i);
        UVec(ulib_float) *centroid = &uvec_get(UVec(ulib_float), &results->centroids, sample_cluster);
        uvec_foreach(ulib_float, centroid, x) {
            *x.item += uvec_get(ulib_float, sample.item, x.i);
        }
    }

    uvec_foreach(UVec(ulib_float), &results->centroids, _centroid) { 
        ulib_uint cluster_size = uvec_get(ulib_uint, &results->cluster_size, _centroid.i);
        if (cluster_size == 0) continue; // Do not update cluster with size 0
        uvec_foreach(ulib_float, _centroid.item, x) {
            *x.item /= cluster_size;
        }
    } 

    return update;
}

