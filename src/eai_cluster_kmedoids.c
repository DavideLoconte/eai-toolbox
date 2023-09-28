#include "eai_cluster.h"
#include "eai_cluster_private.h"

/**
 * Initialize KMedoids clustering results by selecting the k points
 * @param config the algorithm configuration
 * @param data the data to cluster
 * @param n_clusters the number of clusters
 */
static void select_initial_centroids(EaiClusterResults *results, UVec(UVec(ulib_float)) * data, ulib_uint n_clusters);

/**
 * 
 * @param config the algorithm configuration
 * @param data the data to cluster
 * @return the cost of the current solution
 */
static ulib_float compute_cost(EaiClusterResults *results, UVec(UVec(ulib_float)) *data);

// Public impl =================================================================

EaiKMedoidsConfig eai_cluster_kmedoids_config(void)
{
    EaiKMedoidsConfig result;
    result.max_iter = 300;
    result.n_clusters = 8;
    result.seed = 0;
    return result;
}

EaiClusterResults eai_cluster_kmedoids(EaiKMedoidsConfig *config, UVec(UVec(ulib_float)) * data)
{
    EaiClusterResults results;
    eai_init_clustering(&results, data);
    srand(config->seed);
    select_initial_centroids(&results, data, config->n_clusters);

    for(ulib_uint i = 0; i < config->max_iter; i++) {
        perror("KMedoids not implemented");
    }

    return results;
}

void select_initial_centroids(EaiClusterResults *results, UVec(UVec(ulib_float)) * data, ulib_uint n_clusters)
{
    UVec(ulib_float) sums = uvec(ulib_float);
    UVec(ulib_uint) points = uvec(ulib_uint);

    uvec_foreach(UVec(ulib_float), data, _sample) {
        ulib_float sum = 0;
        ulib_uint i = 0;
        UVec(ulib_float) *sample = _sample.item; 

        uvec_foreach(UVec(ulib_float), data, _other) {
            UVec(ulib_float) *other = _other.item;
            sum += eai_distance(sample, other);
        }

        uvec_insert_sorted(ulib_float, &sums, sum, &i);
        uvec_insert_at(ulib_uint, &points, _sample.i, i);
    }
    
    uvec_foreach(ulib_uint, &points, point) {
        if (point.i >= n_clusters) break;
        UVec(ulib_float) centroid = uvec(ulib_float);
        UVec(ulib_float) *data_point = &uvec_get(UVec(ulib_float), data, *point.item); 
        uvec_copy(ulib_float, data_point, &centroid);
        uvec_push(UVec(ulib_float), &results->centroids, centroid);        
    }

    uvec_deinit(ulib_float, &sums);    
    uvec_deinit(ulib_uint, &points);    
}


static ulib_float compute_cost(EaiClusterResults *results, UVec(UVec(ulib_float)) *data)
{
    abort(); // TODO
}
