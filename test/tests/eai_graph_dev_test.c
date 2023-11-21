//
// Created by DavideLoconte on 16-Jan-23.
//

#include "eai_graph_dev_test.h"
#include "eai_graph_dev.h"
#include <ulib.h>

inline static bool node_eq(ulib_uint a, ulib_uint b) { return a == b; }


bool eai_graph_dev_test_1(void)
{
    EaiGraph_dev graph = eai_graph_dev();
    eai_graph_deinit_dev(&graph);
    return true;
}

bool eai_graph_dev_test_2(void)
{
    EaiGraph_dev graph = eai_graph_dev();

    eai_graph_add_node_dev(&graph, 0);
    eai_graph_add_node_dev(&graph, 1);
    eai_graph_add_node_dev(&graph, 2);
    eai_graph_add_node_dev(&graph, 3);
    eai_graph_add_node_dev(&graph, 4);

    eai_graph_add_edge_dev( &graph, 0, 1, 0);
    eai_graph_add_edge_dev( &graph, 1, 2, 0);
    eai_graph_add_edge_dev( &graph, 2, 2, 0);
    eai_graph_add_edge_dev( &graph, 4, 3, 0);
    eai_graph_add_edge_dev( &graph, 1, 0, 0);

    utest_assert(eai_graph_has_node_dev( &graph, 0));
    utest_assert(eai_graph_has_node_dev( &graph, 1));
    utest_assert(eai_graph_has_node_dev( &graph, 4));
    utest_assert(!eai_graph_has_node_dev( &graph, 10));

    utest_assert(eai_graph_has_edge_dev( &graph, 0, 1));
    utest_assert(eai_graph_has_edge_dev( &graph, 1, 0));
    utest_assert(eai_graph_has_edge_dev( &graph, 2, 2));
    utest_assert(!eai_graph_has_edge_dev( &graph, 4, 2));
    utest_assert(!eai_graph_has_edge_dev( &graph, 2, 4));
    eai_graph_deinit_dev( &graph);
    return true;
}

bool eai_graph_dev_test_3(void)
{
    EaiGraph_dev graph = eai_graph_dev();

    eai_graph_add_node_dev( &graph, 0);
    eai_graph_add_node_dev( &graph, 1);
    eai_graph_add_node_dev( &graph, 2);
    eai_graph_add_node_dev( &graph, 3);
    eai_graph_add_node_dev( &graph, 4);

    eai_graph_add_edge_dev( &graph, 0, 1, 42);
    eai_graph_add_edge_dev( &graph, 1, 2, 0);
    eai_graph_add_edge_dev( &graph, 2, 2, 0);
    eai_graph_add_edge_dev( &graph, 4, 3, 0);
    eai_graph_add_edge_dev( &graph, 1, 0, 0);

    utest_assert_false(eai_graph_has_node_dev( &graph, 5));
    utest_assert_false(ubit_is_set(8, graph.flags, EAI_GRAPH_NOT_FOUND));

    utest_assert_false(eai_graph_add_node_dev( &graph, 4));
    utest_assert(ubit_is_set(8, graph.flags, EAI_GRAPH_DUPLICATE));

    utest_assert(eai_graph_has_node_dev( &graph, 4));
    utest_assert(eai_graph_has_edge_dev( &graph, 0, 1));
    utest_assert_false(eai_graph_has_edge_dev( &graph, 0, 5));

    eai_graph_get_edge_dev( &graph, 0, 5);
    utest_assert(ubit_is_set(8, graph.flags, EAI_GRAPH_NOT_FOUND));

    utest_assert(eai_graph_get_edge_dev( &graph, 0, 1) == 42);
    utest_assert_false(ubit_is_set(8, graph.flags, EAI_GRAPH_NOT_FOUND));

    utest_assert(eai_graph_has_edge_dev( &graph, 0, 1));
    utest_assert_false(ubit_is_set(8, graph.flags, EAI_GRAPH_NOT_FOUND));

    utest_assert(eai_graph_del_edge_dev( &graph, 0, 1));

    utest_assert_false(ubit_is_set(8, graph.flags, EAI_GRAPH_NOT_FOUND));
    utest_assert_false(eai_graph_has_edge_dev( &graph, 0, 1));
    eai_graph_get_edge_dev( &graph, 0, 1);
    utest_assert(ubit_is_set(8, graph.flags, EAI_GRAPH_NOT_FOUND));

    eai_graph_add_edge_dev( &graph, 0, 1, 42);

    utest_assert(eai_graph_has_node_dev(&graph, 1));
    utest_assert(eai_graph_has_edge_dev(&graph, 1, 2));
    utest_assert(eai_graph_has_edge_dev(&graph, 1, 0));
    utest_assert(eai_graph_has_edge_dev(&graph, 0, 1));

    utest_assert(eai_graph_del_node_dev(&graph, 1));

    utest_assert_false(eai_graph_has_node_dev(&graph, 1));
    utest_assert_false(eai_graph_has_edge_dev(&graph, 1, 2));
    utest_assert_false(eai_graph_has_edge_dev(&graph, 1, 0));
    utest_assert_false(eai_graph_has_edge_dev(&graph, 0, 1));

    utest_assert_false(eai_graph_del_node_dev(&graph, 1));
    utest_assert(ubit_is_set(8, graph.flags, EAI_GRAPH_NOT_FOUND));

    eai_graph_deinit_dev( &graph);
    return true;
}

bool eai_graph_dev_test_4(void)
{
    EaiGraph_dev graph = eai_graph_dev();

    eai_graph_add_node_dev( &graph, 0);
    eai_graph_add_node_dev( &graph, 1);
    eai_graph_add_node_dev( &graph, 2);
    eai_graph_add_node_dev( &graph, 3);
    eai_graph_add_node_dev( &graph, 4);

    eai_graph_add_edge_dev( &graph, 0, 1, 42);
    eai_graph_add_edge_dev( &graph, 1, 2, 0);
    eai_graph_add_edge_dev( &graph, 2, 2, 0);
    eai_graph_add_edge_dev( &graph, 4, 3, 0);
    eai_graph_add_edge_dev( &graph, 1, 0, 0);

    utest_assert(eai_graph_get_edge_dev( &graph, 0, 1) == 42);

    ulib_uint count = 0;
    eai_graph_foreach_node_dev( &graph, node) { count++; }
    utest_assert_uint(count, ==, 5);

    count = 0;
    eai_graph_foreach_edge_dev( &graph, edge) {
        count++;
    }
    utest_assert_uint(count, ==, 5);

    count = 0;
    eai_graph_foreach_edge_to_dev(&graph, 2, edge) {
        count++;
    }
    utest_assert_uint(count, ==, 2);

    count = 0;
    eai_graph_foreach_edge_from_dev(&graph, 1, edge) {
        count++;
    }

    utest_assert_uint(count, ==, 2);

    count = 0;
    eai_graph_foreach_edge_from_dev( &graph, 0, edge) {
        count++;
    }
    utest_assert_uint(count, ==, 1);

    count = 0;
    eai_graph_foreach_edge_to_dev( &graph, 4, edge) {
        count++;
    }
    utest_assert_uint(count, ==, 0);

    count = 0;
    eai_graph_foreach_edge_from_dev( &graph, 4, edge) {
        count++;
    }
    utest_assert_uint(count, ==, 1);

    eai_graph_deinit_dev( &graph);
    return true;
}
