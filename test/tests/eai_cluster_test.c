#include "eai_cluster_test.h"
#include "eai_cluster.h"

// Private impl ================================================================

/**
 * @return test data for kmeans
 */
static UVec(UVec(ulib_float)) setup_kmeans_data(void);

/**
 * @param test data to deinit to free up resources 
 */
static void teardown_data(UVec(UVec(ulib_float)) *data);

// Public impl =================================================================

bool test_eai_cluster_kmeans(void)
{
    UVec(UVec(ulib_float)) data = setup_kmeans_data();
    const ulib_float expected_centroids[3][2] = {
        {1,1}, {7,1}, {4,7}
    };
    ulib_byte found [3] = {0, 0, 0};

    EaiKMeansConfig config = eai_cluster_kmeans_config();
    config.n_clusters = 3;

    EaiClusterResults cluster_results = eai_cluster_kmeans(&config, &data); 
    utest_assert_uint(uvec_count(UVec(ulib_float), &cluster_results.centroids), ==, 3);
    utest_assert_uint(uvec_count(ulib_uint, &cluster_results.cluster), ==, 12);
    utest_assert_uint(uvec_count(ulib_uint, &cluster_results.cluster_size), ==, 3);

    uvec_foreach(UVec(ulib_float), &cluster_results.centroids, centroid) { 
        utest_assert_uint(uvec_count(ulib_float, centroid.item), ==, 2);
        ulib_float x1 = uvec_get(ulib_float, centroid.item, 0);
        ulib_float x2 = uvec_get(ulib_float, centroid.item, 1);
        
        for (ulib_uint i = 0; i < 3; i++) {
            if (x1 == expected_centroids[i][0] && x2 == expected_centroids[i][1]) {
                found[i] = 1;
            }
        }
    }

    utest_assert_uint(found[0], ==, 1);
    utest_assert_uint(found[1], ==, 1);
    utest_assert_uint(found[2], ==, 1);
    teardown_data(&data);
    eai_cluster_deinit(&cluster_results);
    return true;
}

bool test_eai_cluster_kmedoids(void)
{
    return true; // TODO
}

static UVec(UVec(ulib_float)) setup_kmeans_data(void)
{
    const ulib_float test_data[12][2] = {
        {0,0}, {0,2}, {2,0}, {2,2},
        {6,0}, {8,0}, {6,2}, {8,2},
        {3,6}, {5,6}, {3,8}, {5,8}
    };

    UVec(UVec(ulib_float)) data = uvec(UVec(ulib_float));
    for (ulib_uint i = 0; i < 12; i++) {
        UVec(ulib_float) sample = uvec(ulib_float);
        uvec_push(ulib_float, &sample, test_data[i][0]); 
        uvec_push(ulib_float, &sample, test_data[i][1]); 
        uvec_push(UVec(ulib_float), &data, sample);
    } 
    
    return data;
}

static void teardown_data(UVec(UVec(ulib_float)) *data)
{
    for (ulib_uint i = 0; i < 12; i++) {
        UVec(ulib_float) sample = uvec_get(UVec(ulib_float), data, i);
        uvec_deinit(ulib_float, &sample);
    }
    uvec_deinit(UVec(ulib_float), data);
}

