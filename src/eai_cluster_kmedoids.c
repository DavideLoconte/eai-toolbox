#include "eai_cluster.h"
#include "eai_cluster_private.h"
#include "ubase.h"

/**
 * The clustering context, data specific to kmedoids clustering algorithm
 * used to perform computation
 *
 * Terminology for the algorithm:
 *  - sample: is a data point to cluster
 *  - cluster: is the group of the points. in the code is represented by an integer index
 *  - centroid: a sample that represents the center of the cluster.
 *  - medoid: is the index of the data point having the same coordinates of the centroid
 *
 * @param results the results data structure to return
 * @param data reference to the data to cluster
 * @param distances a matrix of size n*n (where n is the number of data points)
 *                  where the ij-th element is the distance between the i-th
 *                  and j-th data point
 * @param medoids a vector of size n_cluster where the i-th element is the medoid
 *                of the i-th cluster, that is the index of the sample representing
 *                the cluster centroid
 *
 */
typedef struct EaiClusterContext_s {
    EaiClusterResults results;
    UVec(UVec(ulib_float)) * data;
    UVec(UVec(ulib_float)) distances;
    UVec(ulib_uint) medoids;
} EaiClusterContext;

/**
 *  Select initial centroids
 *  @param context the clustering algorithm cost
 *  @param n_clusters the number of clusters to select
 */
static void select_initial_centroids(EaiClusterContext *context, ulib_uint n_clusters);

/**
 * @param context the clustering algorithm cost
 * @return the cost of the current solution
 */
static ulib_float compute_cost(EaiClusterContext *context);

/**
 * Init eai cluster context
 * @param config the clustering algorithm configuration
 * @param data the data to cluster
 * @return an inited cluster context
 */

static EaiClusterContext eai_cluster_context(EaiKMedoidsConfig *config,
                                             UVec(UVec(ulib_float)) * data);

/**
 * @param context the clustering context
 * @param _a index of the first data point
 * @param _b index of the secon data point
 * @return pre-computed distance between _a and _b
 */
static ulib_float distance(EaiClusterContext *context, ulib_uint _a, ulib_uint _b);

/**
 * Precompute all distances between points
 * @param context the context
 */
static void precompute_distances(EaiClusterContext *context);

/**
 * Swap medoids with another point and update costs accordingly
 * @param context the clustering context
 * @param centorid the centroid idx
 * @param point the data point idx to swap
 */
static void swap_centroid(EaiClusterContext *context, ulib_uint centroid, ulib_uint point);

/**
 * Swap algorithm: for each centroid, for each datapoint, try to swap, if cost
 * increases rollback
 * @param the clustering context
 * @return true if no swap were executed and the algorithm converged
 */
static ulib_byte swap(EaiClusterContext *context, ulib_uint iteration);

/**
 * Deinit cluster context
 * @param the context to deinit
 */
static void eai_cluster_context_deinit(EaiClusterContext *context);

// Public impl =================================================================

EaiKMedoidsConfig eai_cluster_kmedoids_config(void)
{
    EaiKMedoidsConfig result;
    result.max_iter = 50;
    result.n_clusters = 8;
    result.seed = 0;
    return result;
}

EaiClusterResults eai_cluster_kmedoids(EaiKMedoidsConfig *config, UVec(UVec(ulib_float)) * data)
{
    EaiClusterContext context = eai_cluster_context(config, data);
    ulib_byte stop = 0;

    for(ulib_uint i = 0; i < config->max_iter && !stop; i++) {
        // Try to swap at least all clusters
        stop = !swap(&context, i) && i >= config->n_clusters;
    }

    eai_cluster_context_deinit(&context);
    return context.results;
}

// Private impl ================================================================

EaiClusterContext eai_cluster_context(EaiKMedoidsConfig *config, UVec(UVec(ulib_float) * data))
{
    EaiClusterContext context;
    eai_init_clustering(&context.results, data);
    context.data = data;
    context.distances = uvec(UVec(ulib_float));
    context.medoids = uvec(ulib_uint);

    precompute_distances(&context);
    srand(config->seed);
    select_initial_centroids(&context, config->n_clusters);
    eai_assign_clusters(&context.results, data);
    return context;
}

void precompute_distances(EaiClusterContext *context)
{
    uvec_foreach(UVec(ulib_float), context->data, sample) {
        UVec(ulib_float) distances = uvec(ulib_float);
        for(ulib_uint i = 0; i < sample.i; i++) {
            UVec(ulib_float) *other_sample = &uvec_get(UVec(ulib_float), context->data, i);
            ulib_float distance = eai_distance(sample.item, other_sample);
            uvec_push(ulib_float, &distances, distance);
        }

        uvec_push(UVec(ulib_float), &context->distances, distances);
    }
}

void select_initial_centroids(EaiClusterContext *context, ulib_uint n_clusters)
{
    UVec(ulib_float) sums = uvec(ulib_float);
    UVec(ulib_uint) points = uvec(ulib_uint);

    uvec_foreach(UVec(ulib_float), context->data, _sample) {
        ulib_float sum = 0;
        ulib_uint i;

        uvec_foreach(UVec(ulib_float), context->data, _other) {
            sum += distance(context, _sample.i, _other.i);
        }

        uvec_insert_sorted(ulib_float, &sums, sum, &i);
        uvec_push(ulib_uint, &points, i);
    }

    uvec_foreach(ulib_uint, &points, point) {
        if(point.i >= n_clusters) break;
        uvec_push(ulib_uint, &context->medoids, *point.item);

        UVec(ulib_float) centroid = uvec(ulib_float);
        UVec(ulib_float) *sample = &uvec_get(UVec(ulib_float), context->data, *point.item);
        uvec_copy(ulib_float, sample, &centroid);
        uvec_push(UVec(ulib_float), &context->results.centroids, centroid);
        uvec_push(ulib_uint, &context->results.cluster_size, 0);
    }

    uvec_deinit(ulib_float, &sums);
    uvec_deinit(ulib_uint, &points);
}

ulib_float compute_cost(EaiClusterContext *context)
{
    ulib_float sum = 0;
    uvec_foreach(ulib_uint, &context->results.cluster, _cluster) {
        const ulib_uint cluster = *_cluster.item;
        const ulib_uint medoid = uvec_get(ulib_uint, &context->medoids, cluster);
        sum += distance(context, medoid, _cluster.i);
    }
    return sum;
}

ulib_float distance(EaiClusterContext *context, ulib_uint _a, ulib_uint _b)
{
    if(_a == _b)
        return 0; // It is the same point

    ulib_uint a = _a > _b ? _a : _b;
    ulib_uint b = _a <= _b ? _a : _b;

    UVec(ulib_float) *distances = &uvec_get(UVec(ulib_float), &context->distances, a);
    return uvec_get(ulib_float, distances, b);
}

ulib_byte swap(EaiClusterContext *context, ulib_uint iteration)
{
    const ulib_uint n_clusters = uvec_count(ulib_uint, &context->medoids);
    const ulib_uint n_samples = uvec_count(UVec(ulib_float), context->data);
    const ulib_uint cluster = iteration % n_clusters;
    const ulib_uint medoid = uvec_get(ulib_uint, &context->medoids, cluster);

    ulib_byte swapped = 0;
    ulib_uint min_swap = medoid;
    ulib_uint min_cost = compute_cost(context);

    for(ulib_uint i = 0; i < n_samples; i++) {
        swap_centroid(context, cluster, i);
        ulib_float cost = compute_cost(context);
        if(min_cost > cost) {
            min_cost = cost;
            swapped = 1;
            min_swap = i;
        }
    }

    swap_centroid(context, cluster, min_swap);
    return swapped;
}

void swap_centroid(EaiClusterContext *context, ulib_uint cluster, ulib_uint point)
{
    UVec(ulib_float) *centroid = &uvec_get(UVec(ulib_float), &context->results.centroids, cluster);
    UVec(ulib_float) *data = &uvec_get(UVec(ulib_float), context->data, point);
    uvec_copy(ulib_float, data, centroid);
    uvec_set(ulib_uint, &context->medoids, cluster, point);
    eai_assign_clusters(&context->results, context->data);
}

void eai_cluster_context_deinit(EaiClusterContext *context)
{
    uvec_foreach(UVec(ulib_float), &context->distances, sample) {
        uvec_deinit(ulib_float, sample.item);
    }
    uvec_deinit(UVec(ulib_float), &context->distances);
    uvec_deinit(ulib_uint, &context->medoids);
}

