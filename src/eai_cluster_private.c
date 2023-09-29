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

void eai_select_random_centroids(EaiClusterResults *results,
                                 UVec(UVec(ulib_float)) * data,
                                 ulib_uint n_cluster)
{
    const ulib_uint data_count = uvec_count(UVec(ulib_float), data);
    UVec(ulib_uint) clusters = uvec(ulib_uint);

    for(ulib_uint i = 0; i < n_cluster; i++) {
        ulib_uint cluster = rand() % data_count;

        // TODO this is not a good selection strategy if data_count is close to n_cluster
        while(uvec_contains_sorted(ulib_uint, &clusters, cluster)) {
            cluster = rand() % data_count;
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

