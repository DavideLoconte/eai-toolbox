//
// Created by DavideLoconte on 02-Nov-22.
//
#include <time.h>
#include <utest.h>

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

//#include "eai_graph_dev.h"
#include "eai_profile.h"
//#include "tests/eai_graph_dev_test.h"
#include "eai_graph.h"

static inline bool node_eq(ulib_uint a, ulib_uint b) { return a == b; }

EAI_GRAPH_DECL(dev, ulib_uint, ulib_uint)
EAI_GRAPH_IMPL(dev, node_eq)

void test_graph_dev() {
    EaiGraph_dev graph = eai_graph_dev();

    for (ulib_uint i = 0; i < 500; i++) {
        eai_graph_add_node_dev(&graph, i);
    }

    for (ulib_uint i = 0; i < 5000; i++) {
        ulib_uint a = rand() % 500;
        ulib_uint b = rand() % 500;
        ulib_uint val = rand();

        if (!eai_graph_has_edge_dev(&graph, a, b)) {
            eai_graph_add_edge_dev(&graph, a, b, val);
        }
    }

    for (ulib_uint i = 0; i < 5000; i++) {
        ulib_uint a = rand() % 500;
        ulib_uint b = rand() % 500;
        ulib_uint val = rand();

        if (eai_graph_has_edge_dev(&graph, a, b)) {
//            printf("Removing edge %u->%u\n", a, b);
//            printf("Nodes: [");
//            uvec_foreach(EaiGraphNode_dev, &graph.nodes, n) {
//                printf("%u, ", n.i);
//            }
//            printf("]\nEdges: [");
//            uvec_foreach(EaiGraphEdge_dev, &graph.edges, e) {
//                printf("%u, ", e.item->to);
//            }
//            printf("]\nLimits: [");
//            uvec_foreach(ulib_uint, &graph.limits, l) {
//                printf("%u, ", *l.item);
//            }
            eai_graph_del_edge_dev(&graph, a, b);
        }

        if (!eai_graph_has_edge_dev(&graph, b, a)) {
            eai_graph_add_edge_dev(&graph, b, a, val);
        }
    }

    for (ulib_uint i = 0; i < 50; i++) {
        ulib_uint a = rand()  % 500;
        if (eai_graph_has_node_dev(&graph, a)) {
            eai_graph_del_node_dev(&graph, a);
        }
    }

    for (ulib_uint i = 0; i < 50; i++) {
        volatile ulib_uint x = 0;
        ulib_uint a = rand() % 100;
        ulib_uint b = rand() % 100;

        eai_graph_foreach_edge(dev, &graph, edge) {
            x += edge.start;
            x -= edge.end;
        }

        eai_graph_foreach_edge_from(dev, &graph, a, edge) {
            x += edge.start;
            x -= edge.end;
        }

        eai_graph_foreach_edge_to(dev, &graph, b, edge) {
            x += edge.start;
            x -= edge.end;
        }

        eai_graph_foreach_node(dev, &graph, node) {
            x += *node.item;
        }
    }

    eai_graph_deinit_dev(&graph);
}

utest_main({
//    utest_run("eai_csv_reader_test", EAI_CSV_READER_TESTS);
//    utest_run("eai_math_test", EAI_MATH_TESTS);
//    utest_run("eai_evaluation_confusion_matrix_test", EAI_CONFUSION_MATRIX_TESTS);
//    utest_run("eai_graph_test", EAI_GRAPH_TESTS);
//    utest_run("eai_graph_dev_test", EAI_GRAPH_DEV_TESTS);
    ulib_uint start = clock();
    test_graph_dev();
    printf("Clocks: %u\n", clock() - start);
//    utest_run("eai_cluster_test", EAI_CLUSTER_TESTS);
//    utest_run("eai_narray_test", EAI_NARRAY_TESTS);
//    utest_run("eai_profile_test", EAI_PROFILE_TESTS);
})
