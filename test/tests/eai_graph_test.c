//
// Created by DavideLoconte on 16-Jan-23.
//

#include "eai_graph_test.h"
#include "eai_graph.h"
#include <ulib.h>

inline static bool node_eq(ulib_uint a, ulib_uint b) { return a == b; }

EAI_GRAPH_DECL(test_graph, ulib_uint, ulib_uint)
EAI_GRAPH_IMPL(test_graph, node_eq)

EAI_GRAPH_DECL(test_graph2, ulib_uint, ulib_float)
EAI_GRAPH_IMPL(test_graph2, node_eq)

void eai_graph_test_1(void)
{
    EaiGraph(test_graph) graph = eai_graph(test_graph);
    eai_graph_deinit(test_graph, &graph);
}

void eai_graph_test_2(void)
{
    EaiGraph(test_graph) graph = eai_graph(test_graph);

    eai_graph_add_node(test_graph, &graph, 0);
    eai_graph_add_node(test_graph, &graph, 1);
    eai_graph_add_node(test_graph, &graph, 2);
    eai_graph_add_node(test_graph, &graph, 3);
    eai_graph_add_node(test_graph, &graph, 4);

    eai_graph_add_edge(test_graph, &graph, 0, 1, 0);
    eai_graph_add_edge(test_graph, &graph, 1, 2, 0);
    eai_graph_add_edge(test_graph, &graph, 2, 2, 0);
    eai_graph_add_edge(test_graph, &graph, 4, 3, 0);
    eai_graph_add_edge(test_graph, &graph, 1, 0, 0);

    utest_assert(eai_graph_has_node(test_graph, &graph, 0));
    utest_assert(eai_graph_has_node(test_graph, &graph, 1));
    utest_assert(eai_graph_has_node(test_graph, &graph, 4));
    utest_assert(!eai_graph_has_node(test_graph, &graph, 10));

    utest_assert(eai_graph_has_edge(test_graph, &graph, 0, 1));
    utest_assert(eai_graph_has_edge(test_graph, &graph, 1, 0));
    utest_assert(eai_graph_has_edge(test_graph, &graph, 2, 2));
    utest_assert(!eai_graph_has_edge(test_graph, &graph, 4, 2));
    utest_assert(!eai_graph_has_edge(test_graph, &graph, 2, 4));
    eai_graph_deinit(test_graph, &graph);
}

void eai_graph_test_3(void)
{
    EaiGraph(test_graph) graph = eai_graph(test_graph);

    eai_graph_add_node(test_graph, &graph, 0);
    eai_graph_add_node(test_graph, &graph, 1);
    eai_graph_add_node(test_graph, &graph, 2);
    eai_graph_add_node(test_graph, &graph, 3);
    eai_graph_add_node(test_graph, &graph, 4);

    eai_graph_add_edge(test_graph, &graph, 0, 1, 42);
    eai_graph_add_edge(test_graph, &graph, 1, 2, 0);
    eai_graph_add_edge(test_graph, &graph, 2, 2, 0);
    eai_graph_add_edge(test_graph, &graph, 4, 3, 0);
    eai_graph_add_edge(test_graph, &graph, 1, 0, 0);

    utest_assert_false(eai_graph_has_node(test_graph, &graph, 5));
    utest_assert_false(ubit_is_set(8, graph.flags, EAI_GRAPH_NOT_FOUND));

    utest_assert_false(eai_graph_add_node(test_graph, &graph, 4));
    utest_assert(ubit_is_set(8, graph.flags, EAI_GRAPH_DUPLICATE));

    utest_assert(eai_graph_has_node(test_graph, &graph, 4));
    utest_assert(eai_graph_has_edge(test_graph, &graph, 0, 1));
    utest_assert_false(eai_graph_has_edge(test_graph, &graph, 0, 5));

    eai_graph_get_edge(test_graph, &graph, 0, 5);
    utest_assert(ubit_is_set(8, graph.flags, EAI_GRAPH_NOT_FOUND));

    utest_assert(eai_graph_get_edge(test_graph, &graph, 0, 1) == 42);
    utest_assert_false(ubit_is_set(8, graph.flags, EAI_GRAPH_NOT_FOUND));

    utest_assert(eai_graph_has_edge(test_graph, &graph, 0, 1));
    utest_assert_false(ubit_is_set(8, graph.flags, EAI_GRAPH_NOT_FOUND));

    utest_assert(eai_graph_del_edge(test_graph, &graph, 0, 1));
    utest_assert_false(ubit_is_set(8, graph.flags, EAI_GRAPH_NOT_FOUND));
    utest_assert_false(eai_graph_has_edge(test_graph, &graph, 0, 1));
    eai_graph_get_edge(test_graph, &graph, 0, 1);
    utest_assert(ubit_is_set(8, graph.flags, EAI_GRAPH_NOT_FOUND));
    eai_graph_add_edge(test_graph, &graph, 0, 1, 42);

    utest_assert(eai_graph_has_node(test_graph, &graph, 1));
    utest_assert(eai_graph_has_edge(test_graph, &graph, 1, 2));
    utest_assert(eai_graph_has_edge(test_graph, &graph, 1, 0));
    utest_assert(eai_graph_has_edge(test_graph, &graph, 0, 1));

    utest_assert(eai_graph_del_node(test_graph, &graph, 1));

    utest_assert_false(eai_graph_has_node(test_graph, &graph, 1));
    utest_assert_false(eai_graph_has_edge(test_graph, &graph, 1, 2));
    utest_assert_false(eai_graph_has_edge(test_graph, &graph, 1, 0));
    utest_assert_false(eai_graph_has_edge(test_graph, &graph, 0, 1));

    utest_assert_false(eai_graph_del_node(test_graph, &graph, 1));
    utest_assert(ubit_is_set(8, graph.flags, EAI_GRAPH_NOT_FOUND));

    eai_graph_deinit(test_graph, &graph);
}

void eai_graph_test_4(void)
{
    EaiGraph(test_graph2) graph = eai_graph(test_graph2);

    eai_graph_add_node(test_graph2, &graph, 0);
    eai_graph_add_node(test_graph2, &graph, 1);
    eai_graph_add_node(test_graph2, &graph, 2);
    eai_graph_add_node(test_graph2, &graph, 3);
    eai_graph_add_node(test_graph2, &graph, 4);

    eai_graph_add_edge(test_graph2, &graph, 0, 1, 42.3);
    eai_graph_add_edge(test_graph2, &graph, 1, 2, 0.1);
    eai_graph_add_edge(test_graph2, &graph, 2, 2, 0.5);
    eai_graph_add_edge(test_graph2, &graph, 4, 3, 0.9);
    eai_graph_add_edge(test_graph2, &graph, 1, 0, 0.9);

    utest_assert(eai_graph_get_edge(test_graph2, &graph, 0, 1) == 42.3);

    ulib_uint count = 0;
    eai_graph_foreach_node(test_graph2, &graph, node) { count++; }
    utest_assert_uint(count, ==, 5);

    count = 0;
    eai_graph_foreach_edge(test_graph2, &graph, edge) {
        count++;
    }
    utest_assert_uint(count, ==, 5);

    count = 0;
    eai_graph_foreach_edge_to(test_graph2, &graph, 2, edge) {
        count++;
    }
    utest_assert_uint(count, ==, 2);

    count = 0;
    eai_graph_foreach_edge_from(test_graph2, &graph, 1, edge) {
        count++;
    }
    utest_assert_uint(count, ==, 2);

    count = 0;
    eai_graph_foreach_edge_from(test_graph2, &graph, 0, edge) {
        count++;
    }
    utest_assert_uint(count, ==, 1);

    count = 0;
    eai_graph_foreach_edge_to(test_graph2, &graph, 4, edge) {
        count++;
    }
    utest_assert_uint(count, ==, 0);

    count = 0;
    eai_graph_foreach_edge_from(test_graph2, &graph, 4, edge) {
        count++;
    }
    utest_assert_uint(count, ==, 1);

    eai_graph_deinit(test_graph2, &graph);
}
