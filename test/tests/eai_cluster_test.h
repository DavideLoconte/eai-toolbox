//
// Created by DavideLoconte on 29-Sep-23.
//

#ifndef EAI_TOOLBOX_EAI_CLUSTER_TEST_H
#define EAI_TOOLBOX_EAI_CLUSTER_TEST_H

#include <ulib.h>

bool test_eai_cluster_kmeans1(void);
bool test_eai_cluster_kmeans2(void);
bool test_eai_cluster_kmeans3(void);

bool test_eai_cluster_kmedoids1(void);
bool test_eai_cluster_kmedoids2(void);
bool test_eai_cluster_kmedoids3(void);

#define EAI_KMEANS_TESTS                                                                           \
    test_eai_cluster_kmeans3, test_eai_cluster_kmeans2, test_eai_cluster_kmeans1

#define EAI_KMEDOIDS_TESTS                                                                         \
    test_eai_cluster_kmedoids1, test_eai_cluster_kmedoids2, test_eai_cluster_kmedoids3

#define EAI_CLUSTER_TESTS EAI_KMEANS_TESTS, EAI_KMEDOIDS_TESTS

#endif // EAI_TOOLBOX_EAI_CLUSTER_TEST_H
