#include <stdlib.h>

#include "eai_cluster_private.h"

typedef struct EaiKMeans_s {
    ulib_uint max_iter;
    ulib_uint n_clusters;
    ulib_uint seed;
    EaiNArray(ulib_float) centroids;
    UVec(ulib_uint) cluster;
    UVec(ulib_uint) cluster_size;
    ulib_byte trained;
    ulib_uint data_shape;
    ulib_uint data_count;
} EaiKMeans;

/**
 * The kmeans fit method, finds cluster in the training data
 * @param m the model
 * @param data the data
 * @param ... unused, unsupervised algorithm
 */
static void eai_kmeans_fit(void *m, EaiNArray(ulib_float) *data, ...);

/**
 * @param m the model
 * @param data the data to predict
 * @param ... unused, unsupervised algorithm
 * @return an array where each element is the cluster index of the input data
 */
static EaiNArray(ulib_float) eai_kmeans_predict(void *m, EaiNArray(ulib_float) *data);

/**
 * @param m the model
 * @param data the data to predict
 * @param ... unused, unsupervised algorithm
 * @return a value inversely proportional to the error in clustering (the sum of the distances between
 *         each point and its centroid)
 */
static ulib_float eai_kmeans_score(void *m, EaiNArray(ulib_float) *data, ...);

/**
 * @param m the model
 * @return deinit the kmeans model
 */
static void eai_kmeans_deinit(void *m);

/**
 * Allocate uvec structures in EaiKMeans
 * @param model the model
 */
static void eai_kmeans_init_clustering(EaiKMeans *model);

/**
 * In the context of KMeans++, select a point as first cluster with a probability
 * proportional to the distance from the nearest centroid
 * @param distances a vector where the i-th element is the distance of the i-th point
 *                  to the nearest centroid
 * @param sum the sum of all distances
 * @param bag an inited and empty uvec of ulib_uint, it is used to perform extraction
 *            this is passed externally to avoid multiple alloc of a vector
 * @return a point selected with probability proportionally from distances
 */
static ulib_uint eai_kmeans_select_cluster_from_distance(UVec(ulib_float) *distances, ulib_float sum, UVec(ulib_uint) *bag);

/**
 * In the context of KMeans++, update distances vector so that the i-th element
 * is the distance of the i-th data point from the nearest neighbor
 * @param model the model
 * @param data the training data
 * @param clusters a vector containing all the cluster currently assigned (at least 1)
 * @param distances [out] the vector that will be populated with the distances
 * @return sum of all value contained in distances vector
 */
static ulib_float eai_kmeans_distance_update(EaiKMeans *model, EaiNArray(ulib_float) *data, UVec(ulib_uint) *clusters, UVec(ulib_float) *distances);

/**
 * Init the uvec structures in KMeans using KMeans++
 * @param model the model
 * @param data the training data
 */
static void eai_kmeans_select_random_centroids(EaiKMeans *model, EaiNArray(ulib_float) *data);

/**
 * Assign the closest centroid to each training data
 * @param model the model
 * @param data the training data
 */
static void eai_kmeans_assign_clusters(EaiKMeans *model, EaiNArray(ulib_float) *data);

/**
 * Update KMeans internal data with the newly assigned clusters
 * @param model the model
 * @param data the training data
 */
static void eai_kmeans_update(EaiKMeans *model, EaiNArray(ulib_float) *data);

/**
 * Update KMeans internal data with the newly assigned clusters
 * @param model the model
 */
static void eai_kmeans_reset_centroids(EaiKMeans *model);

/**
 * If an empty cluster is encoutered, it will be randomly reassigned to another point
 * @param model the model
 * @param data the training data
 */
static void eai_kmeans_reassign_empty_clusters(EaiKMeans *model, EaiNArray(ulib_float) *data);

// Public impl =================================================================

EaiModel eai_cluster_kmeans(ulib_uint n_cluster, ulib_uint max_iter, ulib_uint seed)
{
    EaiModel model;

    model.ctx = malloc(sizeof(EaiKMeans));
    model.fit = &eai_kmeans_fit;
    model.predict = &eai_kmeans_predict;
    model.score = &eai_kmeans_score;
    model.deinit = &eai_kmeans_deinit;
    model.partial_fit = NULL;

    ((EaiKMeans *) model.ctx)->n_clusters = n_cluster;
    ((EaiKMeans *) model.ctx)->max_iter = max_iter;
    ((EaiKMeans *) model.ctx)->seed = seed;
    ((EaiKMeans *) model.ctx)->trained = false;

    return model;
}

UVec(ulib_uint) *eai_cluster_kmeans_clusters(EaiModel *model)
{
    EaiKMeans *kmeans = model->ctx;
    if (!kmeans->trained) return NULL;
    return &kmeans->cluster;
}

EaiNArray(ulib_float) *eai_cluster_kmeans_centroids(EaiModel *model)
{
    EaiKMeans *kmeans = model->ctx;
    if (!kmeans->trained) return NULL;
    return &kmeans->centroids;
}

// Private impl ====================================================================================

static void eai_kmeans_deinit(void *m)
{
    EaiKMeans *model = m;
    if (model->trained) {
        uvec_deinit(ulib_uint, &model->cluster);
        uvec_deinit(ulib_uint, &model->cluster_size);
        eai_narray_deinit(ulib_float, &model->centroids);
    }
    free(m);
}

static void eai_kmeans_fit(void *m, EaiNArray(ulib_float) *data, ...)
{
    EaiKMeans *model = m;
    model->data_shape = uvec_last(ulib_uint, eai_narray_shape(ulib_float, data));
    model->data_count = uvec_first(ulib_uint, eai_narray_shape(ulib_float, data));

    srand(model->seed);
    eai_kmeans_init_clustering(model);
    eai_kmeans_select_random_centroids(model, data);
    eai_kmeans_assign_clusters(model, data);

    for(ulib_uint i = 0; i < model->max_iter; i++) {
        eai_kmeans_update(model, data);
        eai_kmeans_reassign_empty_clusters(model, data);
        eai_kmeans_assign_clusters(model, data);
    }
}

static void eai_kmeans_init_clustering(EaiKMeans *model)
{
    model->cluster_size = uvec(ulib_uint);
    model->cluster = uvec(ulib_uint);
    model->centroids = eai_narray(ulib_float, 2, model->n_clusters, model->data_shape);
    model->trained = true;

    for(ulib_uint i = 0; i < model->data_count; i++) {
        uvec_push(ulib_uint, &model->cluster_size, 0);
        uvec_push(ulib_uint, &model->cluster, 0);
    }
}

static ulib_float eai_kmeans_distance_update(EaiKMeans *model, EaiNArray(ulib_float) *data, UVec(ulib_uint) *clusters, UVec(ulib_float) *distances)
{
    ulib_float sum = 0;

    uvec_remove_all(ulib_float, distances);
    eai_narray_foreach(ulib_float, data, sample)
    {
        ulib_float min_cluster_dist = ULIB_FLOAT_MAX;

        uvec_foreach(ulib_uint, clusters, cluster) {
            EaiNArray(ulib_float) centroid = eai_narray_get(ulib_float, data, *cluster.item);
            ulib_float dist = eai_squared_distance(&centroid, &sample.item, model->data_shape);
            if(dist < min_cluster_dist) {
                min_cluster_dist = dist;
            }
        }

        uvec_push(ulib_float, distances, min_cluster_dist);
        sum += min_cluster_dist;
    }

    return sum;
}

static ulib_uint eai_kmeans_select_cluster_from_distance(UVec(ulib_float) *distances, ulib_float sum, UVec(ulib_uint) *bag)
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

static void eai_kmeans_select_random_centroids(EaiKMeans *model, EaiNArray(ulib_float) *data)
{
    UVec(ulib_uint) clusters = uvec(ulib_uint);
    UVec(ulib_float) distances = uvec(ulib_float);
    UVec(ulib_uint) bag = uvec(ulib_uint);

    ulib_uint cluster = rand() % model->data_count;
    uvec_push(ulib_uint, &clusters, cluster);

    do {
        ulib_float distance_sum = eai_kmeans_distance_update(model, data, &clusters, &distances);
        cluster = eai_kmeans_select_cluster_from_distance(&distances, distance_sum, &bag);
        uvec_push(ulib_uint, &clusters, cluster);
    } while (uvec_count(ulib_uint, &clusters) < model->n_clusters);

    uvec_foreach(ulib_uint, &clusters, x) {
        for(ulib_uint i = 0; i < model->data_shape; i++) {
            ulib_float val = eai_narray_value_at(ulib_float, data, *x.item, i);
            eai_narray_set_value_at(ulib_float, &model->centroids, val, x.i, i);
        }
    }

    uvec_deinit(ulib_uint, &clusters);
    uvec_deinit(ulib_float, &distances);
    uvec_deinit(ulib_uint, &bag);
}

static void eai_kmeans_assign_clusters(EaiKMeans *model, EaiNArray(ulib_float) *data)
{
    uvec_foreach(ulib_uint, &model->cluster_size, value) {
        *value.item = 0;
    }

    eai_narray_foreach(ulib_float, data, sample) {
        ulib_uint min_cluster = 0;
        ulib_float min_distance = ULIB_FLOAT_MAX;

        for(ulib_uint i = 0; i < model->n_clusters; i++) {
            EaiNArray(ulib_float) centroid = eai_narray_get(ulib_float, &model->centroids, i);
            ulib_float d = eai_distance(&sample.item, &centroid, model->data_shape);

            if(d <= min_distance) {
                min_distance = d;
                min_cluster = i;
            }
        }

        ulib_uint previous_cluster_size = uvec_get(ulib_uint, &model->cluster_size, min_cluster);
        uvec_set(ulib_uint, &model->cluster, sample.i, min_cluster);
        uvec_set(ulib_uint, &model->cluster_size, min_cluster, (previous_cluster_size + 1));
    }
}

static void eai_kmeans_update(EaiKMeans *model, EaiNArray(ulib_float) *data)
{
    eai_kmeans_reset_centroids(model);

    eai_narray_foreach(ulib_float, data, sample) {
        ulib_uint cluster = uvec_get(ulib_uint, &model->cluster, sample.i);
        EaiNArray(ulib_float) centroid = eai_narray_get(ulib_float, &model->centroids, cluster);

        for (ulib_uint i = 0; i < model->data_shape; i++) {
            centroid.storage[i] += sample.item.storage[i];
        }
    }

    for (ulib_uint i = 0; i < model->n_clusters; i++) {
        ulib_uint cluster_size = uvec_get(ulib_uint, &model->cluster_size, i);
        if (cluster_size == 0) {
            continue;
        }
        EaiNArray(ulib_float) centroid = eai_narray_get(ulib_float, &model->centroids, i);
        for (ulib_uint j = 0; j < model->data_shape; j++) {
            centroid.storage[j] /= (ulib_float) cluster_size;
        }
    }
}

static void eai_kmeans_reset_centroids(EaiKMeans *model)
{
    ulib_uint *cluster_size_data = uvec_data(ulib_uint, &model->cluster_size);

    for (ulib_uint i = 0; i < model->n_clusters; i++) {
        EaiNArray(ulib_float) centroid = eai_narray_get_ulib_float(&model->centroids, i);
        ulib_uint cluster_size = cluster_size_data[i];
        if (cluster_size == 0) {
            continue;
        }
        memset(centroid.storage, 0, model->data_shape * sizeof(ulib_float));
    }
}

static void eai_kmeans_reassign_empty_clusters(EaiKMeans *model, EaiNArray(ulib_float) *data)
{
    ulib_uint *cluster_size_data = uvec_data(ulib_uint, &model->cluster_size);
    for (ulib_uint i = 0; i < model->n_clusters; i++) {
        if (cluster_size_data[i] != 0) {
            continue;
        }

        ulib_uint random_point = rand() % model->data_count;
        EaiNArray(ulib_float) centroid = eai_narray_get(ulib_float, &model->centroids, i);
        EaiNArray(ulib_float) new_centroid = eai_narray_get(ulib_float, data, random_point);
        eai_narray_copy(ulib_float, &centroid, &new_centroid);
    }
}

static EaiNArray(ulib_float) eai_kmeans_predict(void *m, EaiNArray(ulib_float) *data)
{
    EaiKMeans *model = m;
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

static ulib_float eai_kmeans_score(void *m, EaiNArray(ulib_float) *data, ...)
{
    EaiKMeans *model = m;
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

