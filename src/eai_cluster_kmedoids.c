#include "eai_cluster.h"
#include "eai_cluster_private.h"
#include "eai_narray_builtin.h"

typedef struct EaiKMedoids_s {
    ulib_uint max_iter;
    ulib_uint n_clusters;
    ulib_uint seed;

    EaiNArray(ulib_float) centroids;
    UVec(ulib_uint) cluster;
    UVec(ulib_uint) cluster_size;
    UVec(ulib_uint) medoids;
    EaiNArray(ulib_float) distances;

    ulib_byte trained;
    ulib_uint data_shape;
    ulib_uint data_count;
} EaiKMedoids;

/**
 * The kmedoids fit method, finds cluster in the training data
 * @param m the model
 * @param data the data
 * @param ... unused, unsupervised algorithm
 */
static void eai_kmedoids_fit(void *m, EaiNArray(ulib_float) *data, ...);

/**
 * The kmedoids deinit methods
 * @param m the model
 */
static void eai_kmedoids_deinit(void *m);

/**
 * @param m the model
 * @param data the data to predict
 * @return the closest cluster index for each point
 */
static EaiNArray(ulib_float) eai_kmedoids_predict(void *m, EaiNArray(ulib_float) *data);

/**
 * @param m the model
 * @param data the test data
 * @param ... unused, unsupervised learning
 * @return the inverse of the distance of each sample from their cluster
 */
static ulib_float eai_kmedoids_score(void *m, EaiNArray(ulib_float) *data, ...);

/**
 * Init context kmedoids
 * @param model the model
 * @param data training data
 */
static void eai_kmedoids_init_ctx(EaiKMedoids *model, EaiNArray(ulib_float) *data);

/**
 * Precompute pairwise distances between points
 * @param model the model
 * @param data the data
 */
static void eai_kmedoids_precompute_distances(EaiKMedoids *model, EaiNArray(ulib_float) *data);

/**
 * Overall cost of the current solution
 * @param model the model
 * @param data the data
 * @return Sum of the squared distances between data points and their centroids
 */
static ulib_float eai_kmedoids_compute_cost(EaiKMedoids *model);

/**
 * Perform the swap step of the kmedoids algorithm, that is swapping the medoids
 * with all points in the cluster
 * @param model the model
 * @param cluster the cluster (index) to swap
 * @return true if a swap has occurred
 */
static ulib_byte eai_kmedoids_swap_step(EaiKMedoids *model, ulib_uint cluster);

/**
 * Swap medoid with the specified data index, and recompute clusters
 * @param model the model object
 * @param medoid the original medoid idx
 * @param idx the index of the new medoid
 */
static void eai_kmedoids_swap_medoids(EaiKMedoids *model, ulib_uint cluster, ulib_uint idx);

/**
 * Assign data points to their nearest cluster
 * @param model the model
 */
static void eai_kmedoids_assign_clusters(EaiKMedoids *model);

/**
 * Copy centroids coordinates into the model, in order to make them available to the user
 * after the fit step
 * @param model the model
 * @param data the data
 */
static void eai_kmedoids_copy_centroids(EaiKMedoids *model, EaiNArray(ulib_float) *data);

/**
 * In the context of KMedoids++, select a point as first cluster with a probability
 * proportional to the distance from the nearest centroid
 * @param distances a vector where the i-th element is the distance of the i-th point
 *                  to the nearest centroid
 * @param sum the sum of all distances
 * @param bag an inited and empty uvec of ulib_uint, it is used to perform extraction
 *            this is passed externally to avoid multiple alloc of a vector
 * @return a point selected with probability proportionally from distances
 */
static ulib_uint eai_kmedoids_select_cluster_from_distance(UVec(ulib_float) *distances, ulib_float sum, UVec(ulib_uint) *bag);

/**
 * In the context of KMedoids++, update distances vector so that the i-th element
 * is the distance of the i-th data point from the nearest neighbor
 * @param model the model
 * @param data the training data
 * @param clusters a vector containing all the cluster currently assigned (at least 1)
 * @param distances [out] the vector that will be populated with the distances
 * @return sum of all value contained in distances vector
 */
static ulib_float eai_kmedoids_distance_update(EaiKMedoids *model, EaiNArray(ulib_float) *data, UVec(ulib_float) *distances);

/**
 * Init the uvec structures in KMeans using KMedoids++
 * @param model the model
 * @param data the training data
 */
static void eai_kmedoids_select_random_centroids(EaiKMedoids *model, EaiNArray(ulib_float) *data);

// Public impl =====================================================================================

EaiModel eai_cluster_kmedoids(ulib_uint n_cluster, ulib_uint max_iter, ulib_uint seed)
{
    EaiModel ret;
    EaiKMedoids *ctx = malloc(sizeof(EaiKMedoids));

    ret.ctx = ctx;
    ret.score = &eai_kmedoids_score;
    ret.predict = &eai_kmedoids_predict;
    ret.fit = &eai_kmedoids_fit;
    ret.partial_fit = NULL;
    ret.deinit = &eai_kmedoids_deinit;

    ctx->trained = 0;
    ctx->max_iter = n_cluster;

    if (n_cluster < max_iter) {
        ctx->max_iter = max_iter;
    }

    ctx->n_clusters = n_cluster;
    ctx->seed = seed;
    return ret;
}

UVec(ulib_uint) *eai_cluster_kmedoids_clusters(EaiModel *model)
{
    EaiKMedoids *ctx = model->ctx;
    if (!ctx->trained) {
        return NULL;
    }
    return &ctx->cluster;
}

EaiNArray(ulib_float) *eai_cluster_kmedoids_centroids(EaiModel *model)
{
    EaiKMedoids *ctx = model->ctx;
    if (!ctx->trained) {
        return NULL;
    }
    return &ctx->centroids;
}

UVec(ulib_uint) *eai_cluster_kmedoids_medoids(EaiModel *model)
{
    EaiKMedoids *ctx = model->ctx;
    if (!ctx->trained) {
        return NULL;
    }
    return &ctx->medoids;
}


// Private impl ====================================================================================

static void eai_kmedoids_deinit(void *m)
{
    EaiKMedoids *ctx = m;
    if (ctx->trained) {
        eai_narray_deinit(ulib_float, &ctx->centroids);
        eai_narray_deinit(ulib_float, &ctx->distances);
        uvec_deinit(ulib_uint, &ctx->cluster);
        uvec_deinit(ulib_uint, &ctx->cluster_size);
        uvec_deinit(ulib_uint, &ctx->medoids);
    }
    free(ctx);
}

static void eai_kmedoids_fit(void *m, EaiNArray(ulib_float) *data, ...)
{
    EaiKMedoids *model = m;
    eai_kmedoids_init_ctx(m, data);
    eai_kmedoids_precompute_distances(m, data);
    eai_kmedoids_select_random_centroids(model, data);
    eai_kmedoids_assign_clusters(m);

    for (ulib_uint i = 0; i < model->max_iter; i++) {
        ulib_byte swap_occurred = false;
        for (ulib_uint j = 0; j < model->n_clusters; j++) {
            swap_occurred = eai_kmedoids_swap_step(model, j) || swap_occurred;
        }

        if (!swap_occurred) break;
    }

    eai_kmedoids_copy_centroids(m, data);
}

static void eai_kmedoids_init_ctx(EaiKMedoids *model, EaiNArray(ulib_float) *data)
{
    model->trained = true;
    model->data_shape = uvec_last(ulib_uint, eai_narray_shape(ulib_float, data));
    model->data_count = uvec_first(ulib_uint, eai_narray_shape(ulib_float, data));
    srand(model->seed);

    model->cluster_size = uvec(ulib_uint);
    model->cluster = uvec(ulib_uint);
    model->centroids = eai_narray(ulib_float, 2, model->n_clusters, model->data_shape);
    model->medoids = uvec(ulib_uint);

    for(ulib_uint i = 0; i < model->data_count; i++) {
        uvec_push(ulib_uint, &model->cluster, 0);
    }
}

static void eai_kmedoids_precompute_distances(EaiKMedoids *model, EaiNArray(ulib_float) *data)
{
    model->distances = eai_narray(ulib_float, 2, model->data_count, model->data_count);
    eai_narray_foreach(ulib_float, data, sample) {
        eai_narray_foreach(ulib_float, data, other_sample) {
            if (sample.i == other_sample.i) {
                eai_narray_set_value_at(ulib_float, &model->distances, 0, sample.i, other_sample.i);
            } else {
                ulib_float dist = eai_distance(&sample.item, &other_sample.item, model->data_shape);
                eai_narray_set_value_at(ulib_float, &model->distances, dist, sample.i, other_sample.i);
            }
        }
    }
}

static ulib_float eai_kmedoids_compute_cost(EaiKMedoids *model)
{
    ulib_float sum = 0;
    uvec_foreach(ulib_uint, &model->cluster, _cluster) {
        const ulib_uint cluster = *_cluster.item;
        const ulib_uint medoid = uvec_get(ulib_uint, &model->medoids, cluster);
        sum += eai_narray_value_at(ulib_float, &model->distances, medoid, _cluster.i);
    }
    return sum;
}

static ulib_byte eai_kmedoids_swap_step(EaiKMedoids *model, ulib_uint cluster)
{
    ulib_byte swapped = 0;
    const ulib_uint medoid = uvec_get(ulib_uint, &model->medoids, cluster);

    ulib_uint min_swap = medoid;
    ulib_float min_cost = eai_kmedoids_compute_cost(model);

    for(ulib_uint i = 0; i < model->data_count; i++) {
        if (uvec_contains(ulib_uint, &model->medoids, i)) continue; // Skip medoids

        eai_kmedoids_swap_medoids(model, cluster, i);
        ulib_float cost = eai_kmedoids_compute_cost(model);

        if(cost < min_cost) {
            min_cost = cost;
            swapped = 1;
            min_swap = i;
        }
    }

    eai_kmedoids_swap_medoids(model, cluster, min_swap);
    return swapped;
}

static void eai_kmedoids_swap_medoids(EaiKMedoids *model, ulib_uint cluster, ulib_uint idx)
{
    uvec_set(ulib_uint, &model->medoids, cluster, idx);
    eai_kmedoids_assign_clusters(model);
}

static void eai_kmedoids_assign_clusters(EaiKMedoids *model)
{
    ulib_uint *cluster_data = uvec_data(ulib_uint, &model->cluster);
    ulib_uint *medoids_data = uvec_data(ulib_uint, &model->medoids);

    for(ulib_uint current_point = 0; current_point < model->data_count; current_point++) {
        ulib_uint min_cluster = cluster_data[current_point];
        ulib_uint min_medoid = medoids_data[min_cluster];
        ulib_float min_cluster_dist = eai_narray_value_at(ulib_float, &model->distances, current_point, min_medoid);

        for (ulib_uint new_cluster = 0; new_cluster < model->n_clusters; new_cluster++) {
            ulib_uint new_medoid = medoids_data[new_cluster];
            ulib_float new_cluster_dist = eai_narray_value_at(ulib_float, &model->distances, current_point, new_medoid);

            if (new_cluster_dist < min_cluster_dist) {
                cluster_data[current_point] = new_cluster;
            }
        }
    }
}

static void eai_kmedoids_copy_centroids(EaiKMedoids *model, EaiNArray(ulib_float) *data)
{
    uvec_foreach(ulib_uint, &model->medoids, item) {
        EaiNArray(ulib_float) centroid = eai_narray_get(ulib_float, data, *item.item);
        EaiNArray(ulib_float) target = eai_narray_get(ulib_float, &model->centroids, item.i);
        eai_narray_copy(ulib_float, &target, &centroid);
    }
}

static EaiNArray(ulib_float) eai_kmedoids_predict(void *m, EaiNArray(ulib_float) *data)
{
    EaiKMedoids *model = m;
    EaiNArray(ulib_float) ret = eai_narray(ulib_float, 1, model->data_count);

    eai_narray_foreach(ulib_float, data, sample)
    {
        ulib_float min_cluster_dist = ULIB_FLOAT_MAX;
        ulib_float min_cluster = 0;

        uvec_foreach(ulib_uint, &model->cluster, cluster) {
            EaiNArray(ulib_float) centroid = eai_narray_get(ulib_float, data, *cluster.item);
            ulib_float dist = eai_squared_distance(&centroid, &sample.item, model->data_shape);
            if(dist < min_cluster_dist) {
                min_cluster_dist = dist;
                min_cluster = cluster.i;
            }
        }

        eai_narray_set_value_at(ulib_float, &ret, min_cluster, sample.i);
    }

    return ret;
}

static ulib_float eai_kmedoids_score(void *m, EaiNArray(ulib_float) *data, ...)
{
    EaiKMedoids *model = m;
    ulib_float ret = 0;

    eai_narray_foreach(ulib_float, data, sample)
    {
        ulib_float min_cluster_dist = ULIB_FLOAT_MAX;

        uvec_foreach(ulib_uint, &model->cluster, cluster) {
            EaiNArray(ulib_float) centroid = eai_narray_get(ulib_float, data, *cluster.item);
            ulib_float dist = eai_squared_distance(&centroid, &sample.item, model->data_shape);
            if(dist < min_cluster_dist) {
                min_cluster_dist = dist;
            }
        }

        ret += min_cluster_dist;
    }

    return ret;
}

static ulib_float eai_kmedoids_distance_update(EaiKMedoids *model, EaiNArray(ulib_float) *data, UVec(ulib_float) *distances)
{
    ulib_float sum = 0;

    uvec_remove_all(ulib_float, distances);
    eai_narray_foreach(ulib_float, data, sample)
    {
        ulib_float min_cluster_dist = ULIB_FLOAT_MAX;

        uvec_foreach(ulib_uint, &model->medoids, cluster) {
            ulib_float dist = eai_narray_value_at(ulib_float, &model->distances, *cluster.item, sample.i);
            if(dist < min_cluster_dist) {
                min_cluster_dist = dist;
            }
        }

        uvec_push(ulib_float, distances, min_cluster_dist);
        sum += min_cluster_dist;
    }

    return sum;
}

static ulib_uint eai_kmedoids_select_cluster_from_distance(UVec(ulib_float) *distances, ulib_float sum, UVec(ulib_uint) *bag)
{
    uvec_remove_all(ulib_uint, bag);
    uvec_reserve(ulib_uint, bag, (ulib_uint) (sum + 1));

    uvec_foreach(ulib_float, distances, item) {
        ulib_uint val = (ulib_uint) *item.item;
        for(ulib_uint i = 0; i < val; i++) {
            uvec_push(ulib_uint, bag, item.i);
        }
    }

    ulib_uint random_choice = rand() % uvec_count(ulib_uint, bag);
    ulib_uint ret = uvec_get(ulib_uint, bag, random_choice);

    return ret;
}

static void eai_kmedoids_select_random_centroids(EaiKMedoids *model, EaiNArray(ulib_float) *data)
{
    UVec(ulib_float) distances = uvec(ulib_float);
    UVec(ulib_uint) bag = uvec(ulib_uint);

    ulib_uint cluster = rand() % model->data_count;
    uvec_push(ulib_uint, &model->medoids, cluster);

    do {
        ulib_float distance_sum = eai_kmedoids_distance_update(model, data, &distances);
        cluster = eai_kmedoids_select_cluster_from_distance(&distances, distance_sum, &bag);
        uvec_push(ulib_uint, &model->medoids, cluster);
    } while (uvec_count(ulib_uint, &model->medoids) < model->n_clusters);

    uvec_deinit(ulib_float, &distances);
    uvec_deinit(ulib_uint, &bag);
}
