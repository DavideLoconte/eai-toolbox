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
 * @param data the training data
 */
static void eai_kmeans_init_clustering(EaiKMeans *model, EaiNArray(ulib_float) *data);

/**
 * Init the uvec structures in KMeans
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
 * @param data the training data
 */
static void eai_kmeans_reset_centroids(EaiKMeans *model, EaiNArray(ulib_float) *data);

// Public impl =================================================================

EaiModel eai_cluster_kmeans(ulib_uint n_cluster, ulib_uint max_iter, ulib_uint seed)
{
    EaiModel model;

    model.ctx = malloc(sizeof(EaiKMeans));
    model.fit = &eai_kmeans_fit;
    model.predict = NULL;
    model.score = NULL;
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
    eai_kmeans_init_clustering(model, data);
    eai_kmeans_select_random_centroids(model, data);
    eai_kmeans_assign_clusters(model, data);

    for(ulib_uint i = 0; i < model->max_iter; i++) {
        eai_kmeans_update(model, data);
        eai_kmeans_assign_clusters(model, data);
    }
}

static void eai_kmeans_init_clustering(EaiKMeans *model, EaiNArray(ulib_float) *data)
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

static void eai_kmeans_select_random_centroids(EaiKMeans *model, EaiNArray(ulib_float) *data)
{
    UVec(ulib_uint) clusters = uvec(ulib_uint);

    while(uvec_count(ulib_uint, &clusters) < model->n_clusters) {
        uvec_insert_sorted_unique(ulib_uint, &clusters, rand() % model->data_count, NULL);
    }

    uvec_foreach(ulib_uint, &clusters, x) {
        for(ulib_uint i = 0; i < model->data_shape; i++) {
            ulib_float val = eai_narray_value_at(ulib_float, data, *x.item, i);
            eai_narray_set_value_at(ulib_float, &model->centroids, val, x.i, i);
        }
    }

    uvec_deinit(ulib_uint, &clusters);
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
    eai_kmeans_reset_centroids(model, data);

    eai_narray_foreach(ulib_float, data, sample) {
        ulib_uint cluster = uvec_get(ulib_uint, &model->cluster, sample.i);
        EaiNArray(ulib_float) centroid = eai_narray_get(ulib_float, &model->centroids, cluster);

        for (ulib_uint i = 0; i < model->data_shape; i++) {
            centroid.storage[i] += sample.item.storage[i];
        }
    }

    for (ulib_uint i = 0; i < model->n_clusters; i++) {
        ulib_uint cluster_size = uvec_get(ulib_uint, &model->cluster_size, i);
        if (cluster_size == 0) continue;
        EaiNArray(ulib_float) centroid = eai_narray_get(ulib_float, &model->centroids, i);
        for (ulib_uint j = 0; j < model->data_shape; j++) {
            centroid.storage[j] /= (ulib_float) cluster_size;
        }
    }
}

static void eai_kmeans_reset_centroids(EaiKMeans *model, EaiNArray(ulib_float) *data)
{
    for (ulib_uint i = 0; i < model->n_clusters; i++) {
        EaiNArray(ulib_float) centroid = deb_eai_narray_get_ulib_float(&model->centroids, i);
        ulib_uint cluster_size = uvec_get(ulib_uint, &model->cluster_size, i);
        if (cluster_size == 0) continue;
        memset(centroid.storage, 0, model->data_shape * sizeof(ulib_float));
    }
}

static EaiNArray(ulib_float) eai_kmeans_predict(void *m, EaiNArray(ulib_float) *data)
{
    return eai_narray(ulib_float, 1, 1);
}

static ulib_float eai_kmeans_score(void *m, EaiNArray(ulib_float) *data, ...)
{
    return 0.0;
}
