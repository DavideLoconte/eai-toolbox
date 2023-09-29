//
// Created by DavideLoconte on 29-Sep-23.
//

#ifndef EAI_TOOLBOX_EAI_CLUSTER_TEST_H
#define EAI_TOOLBOX_EAI_CLUSTER_TEST_H

#include <ulib.h>

bool test_eai_cluster_kmeans(void);
bool test_eai_cluster_kmedoids(void);

#define EAI_CLUSTER_TESTS test_eai_cluster_kmeans, test_eai_cluster_kmedoids

#endif // EAI_TOOLBOX_EAI_CLUSTER_TEST_H
