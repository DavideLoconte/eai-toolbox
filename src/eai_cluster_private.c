#include "eai_cluster_private.h"

void eai_init_clustering(EaiClusterResults *results, UVec(UVec(ulib_float)) * data)
{
    const ulib_uint data_count = uvec_count(UVec(ulib_float), data);

    results->cluster_size = uvec(ulib_uint);
    results->cluster = uvec(ulib_uint);
    results->centroids = uvec(UVec(ulib_float));

    for(ulib_uint i = 0; i < data_count; i++) {
        uvec_push(ulib_uint, &results->cluster, 0);
    }
}

#define SAMPLES 8
void eai_select_random_centroids(EaiClusterResults *results,
                                 UVec(UVec(ulib_float)) * data,
                                 ulib_uint n_cluster)
{
    const ulib_uint data_count = uvec_count(UVec(ulib_float), data);
    UVec(ulib_uint) clusters = uvec(ulib_uint);

    ulib_uint cluster = rand() % data_count;
    uvec_insert_sorted(ulib_uint, &clusters, cluster, NULL);
    UVec(ulib_float) centroid = uvec(ulib_float);
    UVec(ulib_float) *data_point = &uvec_get(UVec(ulib_float), data, cluster);
    uvec_copy(ulib_float, data_point, &centroid);
    uvec_push(UVec(ulib_float), &results->centroids, centroid);
    uvec_push(ulib_uint, &results->cluster_size, 0);

    while(uvec_count(UVec(ulib_float), &results->centroids) < n_cluster) {
        ulib_float max_dist = -1;
        ulib_uint candidate_cluster[SAMPLES] = { rand() % data_count, rand() % data_count,
                                                 rand() % data_count, rand() % data_count,
                                                 rand() % data_count, rand() % data_count,
                                                 rand() % data_count, rand() % data_count };

        cluster = 0;
        UVec(ulib_float) last_cluster = uvec_last(UVec(ulib_float), &results->centroids);

        for(ulib_uint i = 0; i < SAMPLES; i++) {
            while(uvec_contains_sorted(ulib_uint, &clusters, candidate_cluster[i])) {
                candidate_cluster[i] += (rand() % 6) - 3;
                candidate_cluster[i] %= data_count;
            }

            UVec(ulib_float) *data_point = &uvec_get(UVec(ulib_float), data, cluster);
            ulib_float dist = eai_distance(data_point, &last_cluster);
            if(dist > max_dist) {
                max_dist = dist;
                cluster = candidate_cluster[i];
            }
        }

        uvec_insert_sorted(ulib_uint, &clusters, cluster, NULL);
        UVec(ulib_float) centroid = uvec(ulib_float);
        UVec(ulib_float) *data_point = &uvec_get(UVec(ulib_float), data, cluster);

        uvec_copy(ulib_float, data_point, &centroid);
        uvec_push(UVec(ulib_float), &results->centroids, centroid);
        uvec_push(ulib_uint, &results->cluster_size, 0);
    }
    uvec_deinit(ulib_uint, &clusters);
}

ulib_float eai_distance(UVec(ulib_float) * a, UVec(ulib_float) * b)
{
    return sqrt(eai_squared_distance(a, b));
}

ulib_float eai_squared_distance(UVec(ulib_float) * a, UVec(ulib_float) * b)
{
    ulib_float dist = 0;
    uvec_foreach(ulib_float, a, _x1) {
        ulib_float x1 = *_x1.item;
        ulib_float x2 = uvec_get(ulib_float, b, _x1.i);
        dist += (x1 - x2) * (x1 - x2);
    }
    return dist;
}

void eai_assign_clusters(EaiClusterResults *results, UVec(UVec(ulib_float)) * data)
{
    ulib_uint n_clusters = uvec_count(UVec(ulib_float), &results->centroids);

    uvec_foreach(ulib_uint, &results->cluster_size, value) {
        *value.item = 0;
    }

    uvec_foreach(UVec(ulib_float), data, sample) {
        ulib_uint min_cluster = 42;
        ulib_float min_distance = ULIB_FLOAT_MAX;

        for(ulib_uint i = 0; i < n_clusters; i++) {
            UVec(ulib_float) *centroid = &uvec_get(UVec(ulib_float), &results->centroids, i);
            ulib_float d = eai_distance(sample.item, centroid);
            if(d <= min_distance) {
                min_distance = d;
                min_cluster = i;
            }
        }

        ulib_uint previous_cluster_size = uvec_get(ulib_uint, &results->cluster_size, min_cluster);
        uvec_set(ulib_uint, &results->cluster, sample.i, min_cluster);
        uvec_set(ulib_uint, &results->cluster_size, min_cluster, previous_cluster_size + 1);
    }
}

