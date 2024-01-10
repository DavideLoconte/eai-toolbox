/*
 * MIT License
 *
 * Copyright (c) 2022-2023 Davide Loconte <davide.loconte@hotmail.it>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef EAI_TOOLBOX_EAI_GRAPH_H
#define EAI_TOOLBOX_EAI_GRAPH_H

#include <ulib.h>

#define EAI_GRAPH_MEM_ERROR ubit_bit(8, 1)
#define EAI_GRAPH_NOT_FOUND ubit_bit(8, 2)
#define EAI_GRAPH_DUPLICATE ubit_bit(8, 3)

/**
 * @brief Declare a graph of type T, with node type N and Edge type E
 * Declare a graph of type T. The graph stores on nodes and edges generic
 * data. Keep in mind that nodes has to be equatable, an equality function
 * need to be defined in implementation
 * @param T graph type name
 * @param N node type
 * @param E edge type
 */
#define EAI_GRAPH_DECL(T, N, E)                                                                    \
                                                                                                   \
    typedef N EaiGraphNode_##T;                                                                    \
    typedef E EaiGraphEdge_##T;                                                                    \
                                                                                                   \
    typedef struct EaiGraphEdgeEntry_s_##T {                                                       \
        ulib_uint to;                                                                              \
        EaiGraphEdge_##T val;                                                                      \
    } EaiGraphEdgeEntry_##T;                                                                       \
                                                                                                   \
    UVEC_DECL_EQUATABLE(EaiGraphNode_##T)                                                          \
    UVEC_DECL_COMPARABLE(EaiGraphEdgeEntry_##T)                                                    \
    typedef UVec(EaiGraphEdgeEntry_##T) EaiGraphEdgeList_##T;                                      \
    UVEC_DECL(EaiGraphEdgeList_##T)                                                                \
                                                                                                   \
    typedef struct EaiGraph_s_##T {                                                                \
        UVec(EaiGraphNode_##T) nodes;                                                              \
        UVec(EaiGraphEdgeList_##T) edges;                                                          \
        UBit(8) flags;                                                                             \
    } EaiGraph_##T;                                                                                \
                                                                                                   \
    typedef struct EaiGraphEdgeLoop_s_##T {                                                        \
        EaiGraphNode_##T start, end;                                                               \
        EaiGraphEdge_##T val;                                                                      \
        ulib_uint __a;                                                                             \
        ulib_uint __b;                                                                             \
    } EaiGraphEdgeLoop_##T;                                                                        \
                                                                                                   \
    EaiGraph_##T eai_graph_##T(void);                                                              \
                                                                                                   \
    bool eai_graph_nodes_are_equal_##T(EaiGraphNode_##T a, EaiGraphNode_##T b);                    \
    bool eai_graph_edge_eq_##T(EaiGraphEdgeEntry_##T a, EaiGraphEdgeEntry_##T b);                  \
    bool eai_edge_cmp_##T(EaiGraphEdgeEntry_##T a, EaiGraphEdgeEntry_##T b);                       \
                                                                                                   \
    void eai_graph_deinit_##T(EaiGraph_##T *g);                                                    \
    bool eai_graph_add_node_##T(EaiGraph_##T *g, EaiGraphNode_##T n);                              \
                                                                                                   \
    bool eai_graph_add_edge_##T(EaiGraph_##T *g,                                                   \
                                EaiGraphNode_##T u,                                                \
                                EaiGraphNode_##T v,                                                \
                                EaiGraphEdge_##T e);                                               \
                                                                                                   \
    bool eai_graph_set_node_##T(EaiGraph_##T *g, EaiGraphNode_##T start, EaiGraphNode_##T end);    \
    E eai_graph_set_edge_##T(EaiGraph_##T *g,                                                      \
                             EaiGraphNode_##T u,                                                   \
                             EaiGraphNode_##T v,                                                   \
                             EaiGraphEdge_##T e);                                                  \
                                                                                                   \
    bool eai_graph_has_node_##T(EaiGraph_##T *g, EaiGraphNode_##T n);                              \
    bool eai_graph_has_edge_##T(EaiGraph_##T *g, EaiGraphNode_##T u, EaiGraphNode_##T v);          \
    EaiGraphEdge_##T eai_graph_get_edge_##T(EaiGraph_##T *g,                                       \
                                            EaiGraphNode_##T u,                                    \
                                            EaiGraphNode_##T v);                                   \
    bool eai_graph_del_node_##T(EaiGraph_##T *g, EaiGraphNode_##T node);                           \
    EaiGraphEdge_##T eai_graph_del_edge_##T(EaiGraph_##T *g,                                       \
                                            EaiGraphNode_##T start,                                \
                                            EaiGraphNode_##T end);                                 \
    EaiGraphEdgeLoop_##T p_eai_graph_start_iterator_##T(EaiGraph_##T *g);                          \
    EaiGraphEdgeLoop_##T p_eai_graph_start_iterator_from_##T(EaiGraph_##T *g,                      \
                                                             EaiGraphNode_##T from);               \
    EaiGraphEdgeLoop_##T p_eai_graph_start_iterator_to_##T(EaiGraph_##T *g,                        \
                                                           EaiGraphNode_##T from);                 \
    void p_eai_graph_next_from_##T(EaiGraph_##T *g,                                                \
                                   EaiGraphEdgeLoop_##T *l,                                        \
                                   EaiGraphNode_##T from);                                         \
    void p_eai_graph_next_to_##T(EaiGraph_##T *g, EaiGraphEdgeLoop_##T *l, EaiGraphNode_##T to);   \
    void p_eai_graph_next_##T(EaiGraph_##T *g, EaiGraphEdgeLoop_##T *l);                           \
    bool p_eai_graph_has_next_##T(EaiGraph_##T *g, EaiGraphEdgeLoop_##T *l);

/**
 * @brief Implement a graph of type T
 *
 * @param T the type name of the graph
 * @param node_eq a function that takes as argument two nodes and return true if they are equal
 */
#define EAI_GRAPH_IMPL(T, node_eq)                                                                 \
    bool eai_graph_edge_eq_##T(EaiGraphEdgeEntry_##T a, EaiGraphEdgeEntry_##T b)                   \
    {                                                                                              \
        return a.to == b.to;                                                                       \
    }                                                                                              \
    bool eai_edge_cmp_##T(EaiGraphEdgeEntry_##T a, EaiGraphEdgeEntry_##T b)                        \
    {                                                                                              \
        return a.to < b.to;                                                                        \
    }                                                                                              \
                                                                                                   \
    bool eai_graph_nodes_are_equal_##T(EaiGraphNode_##T a, EaiGraphNode_##T b) { return node_eq(a, b); } \
                                                                                                   \
    UVEC_IMPL_COMPARABLE(EaiGraphEdgeEntry_##T, eai_graph_edge_eq_##T, eai_edge_cmp_##T)           \
    UVEC_IMPL_EQUATABLE(EaiGraphNode_##T, eai_graph_nodes_are_equal_##T)                                 \
    UVEC_IMPL(EaiGraphEdgeList_##T)                                                                \
                                                                                                   \
    EaiGraph_##T eai_graph_##T(void)                                                               \
    {                                                                                              \
        EaiGraph_##T result;                                                                       \
        result.nodes = uvec(EaiGraphNode_##T);                                                     \
        result.edges = uvec(EaiGraphEdgeList_##T);                                                 \
        result.flags = ubit_none(8);                                                               \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    void eai_graph_deinit_##T(EaiGraph_##T *g)                                                     \
    {                                                                                              \
        uvec_foreach(EaiGraphEdgeList_##T, &g->edges, edge) {                                      \
            uvec_deinit(EaiGraphEdgeEntry_##T, edge.item);                                         \
        }                                                                                          \
        uvec_deinit(EaiGraphNode_##T, &g->nodes);                                                  \
        uvec_deinit(EaiGraphEdgeList_##T, &g->edges);                                              \
    }                                                                                              \
                                                                                                   \
    bool eai_graph_add_node_##T(EaiGraph_##T *g, EaiGraphNode_##T n)                               \
    {                                                                                              \
        g->flags = ubit_none(8);                                                                   \
                                                                                                   \
        if(uvec_contains(EaiGraphNode_##T, &g->nodes, n)) {                                        \
            g->flags = ubit_set(8, EAI_GRAPH_DUPLICATE, g->flags);                                 \
            return false;                                                                          \
        }                                                                                          \
        uvec_ret retA = uvec_push(EaiGraphNode_##T, &g->nodes, n);                                 \
                                                                                                   \
        if(retA != UVEC_OK) {                                                                      \
            g->flags = ubit_set(8, EAI_GRAPH_MEM_ERROR, g->flags);                                 \
            return false;                                                                          \
        }                                                                                          \
                                                                                                   \
        UVec(EaiGraphEdgeEntry_##T) x = uvec(EaiGraphEdgeEntry_##T);                               \
        uvec_ret retB = uvec_push(EaiGraphEdgeList_##T, &g->edges, x);                             \
        if(retB != UVEC_OK) {                                                                      \
            uvec_remove(EaiGraphNode_##T, &g->nodes, n);                                           \
            uvec_deinit(EaiGraphEdgeEntry_##T, &x);                                                \
            g->flags = ubit_set(8, EAI_GRAPH_MEM_ERROR, g->flags);                                 \
            return false;                                                                          \
        }                                                                                          \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    bool eai_graph_add_edge_##T(EaiGraph_##T *g,                                                   \
                                EaiGraphNode_##T start,                                            \
                                EaiGraphNode_##T end,                                              \
                                EaiGraphEdge_##T val)                                              \
    {                                                                                              \
        g->flags = ubit_none(8);                                                                   \
                                                                                                   \
        if(!uvec_contains(EaiGraphNode_##T, &g->nodes, start) ||                                   \
           !uvec_contains(EaiGraphNode_##T, &g->nodes, end)) {                                     \
            g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);                                 \
            return false;                                                                          \
        }                                                                                          \
                                                                                                   \
        ulib_uint idx = uvec_index_of(EaiGraphNode_##T, &g->nodes, start);                         \
        EaiGraphEdgeEntry_##T edge;                                                                \
        edge.val = val;                                                                            \
        edge.to = uvec_index_of(EaiGraphNode_##T, &g->nodes, end);                                 \
                                                                                                   \
        uvec_ret ret = uvec_insert_sorted(EaiGraphEdgeEntry_##T,                                   \
                                          &uvec_data(EaiGraphEdgeList_##T, &g->edges)[idx],        \
                                          edge,                                                    \
                                          NULL);                                                   \
        if(ret == UVEC_OK)                                                                         \
            return true;                                                                           \
        g->flags = ubit_set(8, EAI_GRAPH_MEM_ERROR, g->flags);                                     \
        return false;                                                                              \
    }                                                                                              \
                                                                                                   \
    bool eai_graph_set_node_##T(EaiGraph_##T *g, EaiGraphNode_##T old, EaiGraphNode_##T new)       \
    {                                                                                              \
        g->flags = ubit_none(8);                                                                   \
        ulib_uint idx = uvec_index_of(EaiGraphNode_##T, &g->nodes, old);                           \
                                                                                                   \
        if(!uvec_index_is_valid(EaiGraphNode_##T, &g->nodes, idx)) {                               \
            g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);                                 \
            return false;                                                                          \
        }                                                                                          \
                                                                                                   \
        uvec_data(EaiGraphNode_##T, &g->nodes)[idx] = new;                                         \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    EaiGraphEdge_##T eai_graph_set_edge_##T(EaiGraph_##T *g,                                       \
                                            EaiGraphNode_##T start,                                \
                                            EaiGraphNode_##T end,                                  \
                                            EaiGraphEdge_##T e)                                    \
    {                                                                                              \
        EaiGraphEdge_##T old_val;                                                                  \
        memset(&old_val, 0, sizeof(EaiGraphEdge_##T));                                             \
        g->flags = ubit_none(8);                                                                   \
                                                                                                   \
        EaiGraphEdgeEntry_##T new;                                                                 \
        new.to = uvec_index_of(EaiGraphNode_##T, &g->nodes, end);                                  \
        new.val = e;                                                                               \
                                                                                                   \
        if(!uvec_contains(EaiGraphNode_##T, &g->nodes, start) ||                                   \
           !uvec_contains(EaiGraphNode_##T, &g->nodes, end)) {                                     \
            g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);                                 \
            return old_val;                                                                        \
        }                                                                                          \
                                                                                                   \
        ulib_uint start_idx = uvec_index_of(EaiGraphNode_##T, &g->nodes, start);                   \
                                                                                                   \
        if(!uvec_index_is_valid(EaiGraphNode_##T, &g->nodes, start_idx)) {                         \
            g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);                                 \
            return old_val;                                                                        \
        }                                                                                          \
                                                                                                   \
        UVec(EaiGraphEdgeEntry_##T) *edges = &uvec_data(EaiGraphEdgeList_##T,                      \
                                                        &g->edges)[start_idx];                     \
        ulib_uint edge_idx = uvec_index_of_sorted(EaiGraphEdgeEntry_##T, edges, new);              \
                                                                                                   \
        if(!uvec_index_is_valid(EaiGraphEdgeEntry_##T, edges, edge_idx)) {                         \
            g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);                                 \
            return old_val;                                                                        \
        }                                                                                          \
                                                                                                   \
        old_val = uvec_data(EaiGraphEdgeEntry_##T, edges)[edge_idx].val;                           \
        uvec_data(EaiGraphEdgeEntry_##T, edges)[edge_idx] = new;                                   \
        return old_val;                                                                            \
    }                                                                                              \
                                                                                                   \
    EaiGraphEdge_##T eai_graph_get_edge_##T(EaiGraph_##T *g,                                       \
                                            EaiGraphNode_##T start,                                \
                                            EaiGraphNode_##T end)                                  \
    {                                                                                              \
        EaiGraphEdge_##T old_val;                                                                  \
        memset(&old_val, 0, sizeof(EaiGraphEdge_##T));                                             \
        g->flags = ubit_none(8);                                                                   \
                                                                                                   \
        EaiGraphEdgeEntry_##T dummy;                                                               \
        dummy.to = uvec_index_of(EaiGraphNode_##T, &g->nodes, end);                                \
                                                                                                   \
        ulib_uint start_idx = uvec_index_of(EaiGraphNode_##T, &g->nodes, start);                   \
                                                                                                   \
        if(!uvec_index_is_valid(EaiGraphNode_##T, &g->nodes, start_idx)) {                         \
            g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);                                 \
            return old_val;                                                                        \
        }                                                                                          \
                                                                                                   \
        UVec(EaiGraphEdgeEntry_##T) *edges = &uvec_data(EaiGraphEdgeList_##T,                      \
                                                        &g->edges)[start_idx];                     \
        ulib_uint edge_idx = uvec_index_of_sorted(EaiGraphEdgeEntry_##T, edges, dummy);            \
                                                                                                   \
        if(!uvec_index_is_valid(EaiGraphEdgeEntry_##T, edges, edge_idx)) {                         \
            g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);                                 \
            return old_val;                                                                        \
        }                                                                                          \
                                                                                                   \
        return uvec_data(EaiGraphEdgeEntry_##T, edges)[edge_idx].val;                              \
    }                                                                                              \
                                                                                                   \
    bool eai_graph_del_node_##T(EaiGraph_##T *g, EaiGraphNode_##T node)                            \
    {                                                                                              \
        g->flags = ubit_none(8);                                                                   \
                                                                                                   \
        if(!uvec_contains(EaiGraphNode_##T, &g->nodes, node)) {                                    \
            g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);                                 \
            return false;                                                                          \
        }                                                                                          \
                                                                                                   \
        ulib_uint idx = uvec_index_of(EaiGraphNode_##T, &g->nodes, node);                          \
                                                                                                   \
        if(!uvec_index_is_valid(EaiGraphNode_##T, &g->nodes, idx)) {                               \
            g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);                                 \
            return false;                                                                          \
        }                                                                                          \
                                                                                                   \
        EaiGraphEdgeEntry_##T dummy;                                                               \
        dummy.to = uvec_index_of(EaiGraphNode_##T, &g->nodes, node);                               \
                                                                                                   \
        uvec_remove_at(EaiGraphNode_##T, &g->nodes, idx);                                          \
        uvec_deinit(EaiGraphEdgeEntry_##T, &uvec_data(EaiGraphEdgeList_##T, &g->edges)[idx]);      \
        uvec_remove_at(EaiGraphEdgeList_##T, &g->edges, idx);                                      \
                                                                                                   \
        uvec_foreach(EaiGraphEdgeList_##T, &g->edges, edges) {                                     \
            uvec_remove(EaiGraphEdgeEntry_##T, edges.item, dummy);                                 \
        }                                                                                          \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    EaiGraphEdge_##T eai_graph_del_edge_##T(EaiGraph_##T *g,                                       \
                                            EaiGraphNode_##T start,                                \
                                            EaiGraphNode_##T end)                                  \
    {                                                                                              \
        EaiGraphEdge_##T old_val;                                                                  \
        memset(&old_val, 0, sizeof(EaiGraphEdge_##T));                                             \
        g->flags = ubit_none(8);                                                                   \
                                                                                                   \
        if(!uvec_contains(EaiGraphNode_##T, &g->nodes, start) ||                                   \
           !uvec_contains(EaiGraphNode_##T, &g->nodes, end)) {                                     \
            g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);                                 \
            return old_val;                                                                        \
        }                                                                                          \
                                                                                                   \
        EaiGraphEdgeEntry_##T dummy;                                                               \
        dummy.to = uvec_index_of(EaiGraphNode_##T, &g->nodes, end);                                \
        ulib_uint start_idx = uvec_index_of(EaiGraphNode_##T, &g->nodes, start);                   \
                                                                                                   \
        if(!uvec_index_is_valid(EaiGraphNode_##T, &g->nodes, start_idx)) {                         \
            g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);                                 \
            return old_val;                                                                        \
        }                                                                                          \
                                                                                                   \
        UVec(EaiGraphEdgeEntry_##T) *edges = &uvec_data(EaiGraphEdgeList_##T,                      \
                                                        &g->edges)[start_idx];                     \
        ulib_uint edge_idx = uvec_index_of_sorted(EaiGraphEdgeEntry_##T, edges, dummy);            \
                                                                                                   \
        if(!uvec_index_is_valid(EaiGraphEdgeEntry_##T, edges, edge_idx)) {                         \
            g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);                                 \
            return old_val;                                                                        \
        }                                                                                          \
                                                                                                   \
        old_val = uvec_data(EaiGraphEdgeEntry_##T, edges)[edge_idx].val;                           \
        uvec_remove_at(EaiGraphEdgeEntry_##T, edges, edge_idx);                                    \
        return old_val;                                                                            \
    }                                                                                              \
                                                                                                   \
    bool eai_graph_has_node_##T(EaiGraph_##T *g, EaiGraphNode_##T node)                            \
    {                                                                                              \
        return uvec_contains(EaiGraphNode_##T, &g->nodes, node);                                   \
    }                                                                                              \
                                                                                                   \
    bool eai_graph_has_edge_##T(EaiGraph_##T *g, EaiGraphNode_##T start, EaiGraphNode_##T end)     \
    {                                                                                              \
        EaiGraphEdgeEntry_##T dummy;                                                               \
        dummy.to = uvec_index_of(EaiGraphNode_##T, &g->nodes, end);                                \
                                                                                                   \
        ulib_uint start_idx = uvec_index_of(EaiGraphNode_##T, &g->nodes, start);                   \
                                                                                                   \
        if(!uvec_index_is_valid(EaiGraphNode_##T, &g->nodes, start_idx)) {                         \
            return false;                                                                          \
        }                                                                                          \
                                                                                                   \
        UVec(EaiGraphEdgeEntry_##T) *edges = &uvec_data(EaiGraphEdgeList_##T,                      \
                                                        &g->edges)[start_idx];                     \
        ulib_uint edge_idx = uvec_index_of_sorted(EaiGraphEdgeEntry_##T, edges, dummy);            \
        return uvec_index_is_valid(EaiGraphEdgeEntry_##T, edges, edge_idx);                        \
    }                                                                                              \
                                                                                                   \
    EaiGraphEdgeLoop_##T p_eai_graph_start_iterator_##T(EaiGraph_##T *g)                           \
    {                                                                                              \
        EaiGraphEdgeLoop_##T r;                                                                    \
        EaiGraphEdgeEntry_##T first = uvec_first(EaiGraphEdgeEntry_##T,                            \
                                                 &uvec_first(EaiGraphEdgeList_##T, &g->edges));    \
        r.start = uvec_first(EaiGraphNode_##T, &g->nodes);                                         \
        r.end = uvec_get(EaiGraphNode_##T, &g->nodes, first.to);                                   \
        r.val = first.val;                                                                         \
        r.__a = 0;                                                                                 \
        r.__b = 0;                                                                                 \
        return r;                                                                                  \
    }                                                                                              \
                                                                                                   \
    EaiGraphEdgeLoop_##T p_eai_graph_start_iterator_to_##T(EaiGraph_##T *g, EaiGraphNode_##T end)  \
    {                                                                                              \
        EaiGraphEdgeLoop_##T r = p_eai_graph_start_iterator_##T(g);                                \
        if(!eai_graph_nodes_are_equal_##T(r.end, end) && p_eai_graph_has_next_##T(g, &r)) {              \
            p_eai_graph_next_to_##T(g, &r, end);                                                   \
        }                                                                                          \
        return r;                                                                                  \
    }                                                                                              \
                                                                                                   \
    EaiGraphEdgeLoop_##T p_eai_graph_start_iterator_from_##T(EaiGraph_##T *g,                      \
                                                             EaiGraphNode_##T start)               \
    {                                                                                              \
        EaiGraphEdgeLoop_##T r = p_eai_graph_start_iterator_##T(g);                                \
        if(!eai_graph_nodes_are_equal_##T(r.start, start) && p_eai_graph_has_next_##T(g, &r)) {          \
            p_eai_graph_next_from_##T(g, &r, start);                                               \
        }                                                                                          \
        return r;                                                                                  \
    }                                                                                              \
                                                                                                   \
    void p_eai_graph_next_from_##T(EaiGraph_##T *g,                                                \
                                   EaiGraphEdgeLoop_##T *loop,                                     \
                                   EaiGraphNode_##T from)                                          \
    {                                                                                              \
        do {                                                                                       \
            p_eai_graph_next_##T(g, loop);                                                         \
        } while(!eai_graph_nodes_are_equal_##T(loop->start, from) && p_eai_graph_has_next_##T(g, loop)); \
    }                                                                                              \
                                                                                                   \
    void p_eai_graph_next_to_##T(EaiGraph_##T *g, EaiGraphEdgeLoop_##T *loop, EaiGraphNode_##T to) \
    {                                                                                              \
        do {                                                                                       \
            p_eai_graph_next_##T(g, loop);                                                         \
        } while(!eai_graph_nodes_are_equal_##T(loop->end, to) && p_eai_graph_has_next_##T(g, loop));     \
    }                                                                                              \
                                                                                                   \
    void p_eai_graph_next_##T(EaiGraph_##T *g, EaiGraphEdgeLoop_##T *loop)                         \
    {                                                                                              \
        if(!uvec_index_is_valid(EaiGraphEdgeList_##T, &g->edges, loop->__b)) {                     \
            return;                                                                                \
        }                                                                                          \
                                                                                                   \
        do {                                                                                       \
            loop->__a += 1;                                                                        \
            if(!uvec_index_is_valid(EaiGraphEdgeEntry_##T,                                         \
                                    &uvec_get(EaiGraphEdgeList_##T, &g->edges, loop->__b),         \
                                    loop->__a)) {                                                  \
                loop->__b += 1;                                                                    \
                loop->__a = 0;                                                                     \
            }                                                                                      \
            if(!uvec_index_is_valid(EaiGraphEdgeList_##T, &g->edges, loop->__b)) {                 \
                return;                                                                            \
            }                                                                                      \
        } while(!uvec_index_is_valid(EaiGraphEdgeEntry_##T,                                        \
                                     &uvec_get(EaiGraphEdgeList_##T, &g->edges, loop->__b),        \
                                     loop->__a));                                                  \
                                                                                                   \
        if(uvec_index_is_valid(EaiGraphEdgeList_##T, &g->edges, loop->__b)) {                      \
            loop->start = uvec_get(EaiGraphNode_##T, &g->nodes, loop->__b);                        \
            ulib_uint end_idx = uvec_get(EaiGraphEdgeEntry_##T,                                    \
                                         &uvec_get(EaiGraphEdgeList_##T, &g->edges, loop->__b),    \
                                         loop->__a)                                                \
                                    .to;                                                           \
                                                                                                   \
            loop->end = uvec_get(EaiGraphNode_##T, &g->nodes, end_idx);                            \
            loop->val = uvec_get(EaiGraphEdgeEntry_##T,                                            \
                                 &uvec_get(EaiGraphEdgeList_##T, &g->edges, loop->__b),            \
                                 loop->__a)                                                        \
                            .val;                                                                  \
        }                                                                                          \
    }                                                                                              \
                                                                                                   \
    bool p_eai_graph_has_next_##T(EaiGraph_##T *g, EaiGraphEdgeLoop_##T *loop)                     \
    {                                                                                              \
        return uvec_index_is_valid(EaiGraphEdgeList_##T, &g->edges, loop->__b);                    \
    }

/**
 * Declares a new graph variable.
 *
 * @param T graph type.
 *
 */
#define EaiGraph(T) EaiGraph_##T

/**
 * Loop over all nodes in a graph
 * @param graph the graph
 * @param node the node structure iterator name
 */
#define eai_graph_foreach_node(T, graph, node) uvec_foreach(EaiGraphNode_##T, &(graph)->nodes, node)

/**
 * Loop over all edges in a graph
 * @param graph the graph
 * @param edge the edge structure iterator name
 */
#define eai_graph_foreach_edge(T, graph, edge)                                                     \
    for(EaiGraphEdgeLoop_##T edge = p_eai_graph_start_iterator_##T((graph));                       \
        p_eai_graph_has_next_##T((graph), &(edge));                                                \
        p_eai_graph_next_##T((graph), &(edge)))

/**
 * Loop over all edges starting from the specified node
 * @param graph the graph
 * @param start the node starting node
 * @param edge iterator name
 */
#define eai_graph_foreach_edge_from(T, graph, start, edge)                                         \
    for(EaiGraphEdgeLoop_##T edge = p_eai_graph_start_iterator_from_##T((graph), (start));         \
        p_eai_graph_has_next_##T((graph), &(edge));                                                \
        p_eai_graph_next_from_##T((graph), &(edge), start))

/**
 * Loop over all edges ending into the specified node
 * @param graph the graph
 * @param end the node starting node
 * @param edge iterator name
 */
#define eai_graph_foreach_edge_to(T, graph, end, edge)                                             \
    for(EaiGraphEdgeLoop_##T edge = p_eai_graph_start_iterator_to_##T((graph), (end));             \
        p_eai_graph_has_next_##T((graph), &(edge));                                                \
        p_eai_graph_next_to_##T((graph), &(edge), end))

/**
 * @brief create a graph of type T
 *
 * @param T the type of graph
 * @return graph instance of type T
 */
#define eai_graph(T) eai_graph_##T()

/**
 * Destroy the graph and free memory
 * @param T the type of graph
 * @param graph the graph
 */
#define eai_graph_deinit(T, graph) eai_graph_deinit_##T(graph)

/**
 * Add a node to the graph
 * @param T the type of graph
 * @param graph the graph
 * @param node the node
 * @return true if element has been added. If not, the function will return false and
 *         graph->flags will be set to EAI_GRAPH_MEM_ERROR (memory errors) or EAI_GRAPH_DUPLICATE
 * if node exists
 */
#define eai_graph_add_node(T, graph, node) eai_graph_add_node_##T(graph, node)

/**
 * @brief Add a node to the graph

 * @param T the type of graph
 * @param graph the graph
 * @param start the starting node
 * @param end the ending node
 * @param val the associated edge value
 * @return true if element has been added. If not, the function will return false and
 *         graph->flags will be set to EAI_GRAPH_MEM_ERROR (memory errors), EAI_GRAPH_DUPLICATE if
 *         edge exists or EAI_GRAPH_NOT_FOUND if nodes do not exists
 */
#define eai_graph_add_edge(T, graph, start, end, val) eai_graph_add_edge_##T(graph, start, end, val)

/**
 * @brief Replace a node value in the graph
 *
 * @param T the type of graph
 * @param graph the graph
 * @param old the existing node
 * @param new the new node value
 * @return true if the old node has been removed and replaced successfully.
 *         If it does not exists graph->flags will be set to EAI_GRAPH_NOT_FOUND
 *         If memory error occurs, graph->flags will be set to EAI_GRAPH_MEM_ERROR
 */
#define eai_graph_set_node(T, graph, old, new) eai_graph_set_node_##T(graph, old, new)

/**
 * @brief Replace the value on an existing graph edge
 *
 * @param T the type of graph
 * @param graph the graph
 * @param start the initial node
 * @param end the ending node
 * @return the value of the old old edge. NULL on error
 *         If it does not exists graph->flags will be set to EAI_GRAPH_NOT_FOUND and the graph will
 *         be not altered If memory error occurs, graph->flags will be set
 *         to EAI_GRAPH_MEM_ERROR and the graph will be not altered
 */
#define eai_graph_set_edge(T, graph, start, end, val) eai_graph_set_edge_##T(graph, start, end, val)

/**
 * @brief Check if a node exist in the graph
 *
 * @param T the type of graph
 * @param graph the graph
 * @param node the node
 * @return true if node exists
 */
#define eai_graph_has_node(T, graph, node) eai_graph_has_node_##T(graph, node)

/**
 * @brief Return true if a edge between start and end exists
 *
 * @param T the type of graph
 * @param graph the graph
 * @param start the starting node
 * @param end the ending node
 * @return true if the node exists
 */
#define eai_graph_has_edge(T, graph, start, end) eai_graph_has_edge_##T(graph, start, end)

/**
 * @brief Return the edge value between two nodes
 *
 * @param T the type of graph
 * @param graph the graph
 * @param start the initial node
 * @param end the ending node
 * @return the edge annotation/weight. If it does not exists an non-initialized value
 *         will be returned and graph->flags will be set to EAI_GRAPH_NOT_FOUND
 */
#define eai_graph_get_edge(T, graph, start, end) eai_graph_get_edge_##T(graph, start, end)

/**
 * @brief Delete a node to the graph and all edges connected to the node
 *
 * @param T the type of graph
 * @param graph the graph
 * @param node the node
 * @return true if element has been removed. If no element can be found, graph->flags will be set to
 * EAI_GRAPH_NOT_FOUND
 */
#define eai_graph_del_node(T, graph, node) eai_graph_del_node_##T(graph, node)

/**
 * @brief Remove an existing node from the graph
 *
 * @param T the type of graph
 * @param graph the graph
 * @param start the starting node
 * @param end the ending node
 * @param val the associated edge value
 * @return the value stored on the edge. If no element can be found, it will return NULL and flag
 *         will be set to EAI_GRAPH_NOT_FOUND
 */
#define eai_graph_del_edge(T, graph, start, end) eai_graph_del_edge_##T(graph, start, end)

// PRIVATE =========================================================================================

/**
 * @warning PRIVATE function, dont use this
 * @brief Create a graph iterator data structure
 * @param graph the graph
 * @return the iterator struct
 */
#define p_eai_graph_start_iterator(T, graph) p_eai_graph_start_iterator_##T(graph)

/**
 * @warning PRIVATE function, dont use this
 * @brief Create a graph iterator data structure for a iterator that starts from a given node
 * @param graph the graph
 * @param from the starting node
 * @return the iterator struct
 */
#define p_eai_graph_start_iterator_from(T, graph) p_eai_graph_start_iterator_from_##T(graph)

/**
 * @warning PRIVATE function, dont use this
 * @brief Create a graph iterator data structure for a iterator that starts from a given node
 * @param graph the graph
 * @param to the ending node
 * @return the iterator struct
 */
#define p_eai_graph_start_iterator_to(T, graph) p_eai_graph_start_iterator_to_##T(graph)

/**
 * @warning PRIVATE function, dont use this
 * @brief Update iterator with the next edge information
 * @param graph the graph
 * @param loop the loop structure
 * @param from the starting node
 */
#define p_eai_graph_next_from(T, graph, loop, from) p_eai_graph_next_from_##T(graph, loop, from)

/**
 * @warning PRIVATE function, dont use this
 * @brief Update iterator with the next edge information
 * @param graph the graph
 * @param loop the loop structure
 * @param to the ending node
 */
#define p_eai_graph_next_to(T, graph, loop, to) p_eai_graph_next_to_##T(graph, loop, from)

/**
 * @warning PRIVATE function, dont use this
 * @brief Update iterator with the next edge information
 * @param graph the graph
 * @param loop the loop structure
 */
#define p_eai_graph_next(T, graph, loop) p_eai_graph_next_##T(graph, loop)

/**
 * @warning PRIVATE function, dont use this
 * @brief Update iterator with the next edge information
 * @param graph the graph
 * @param loop the loop structure
 * @param true if the next edge exists
 */
#define p_eai_graph_has_next(T, graph, loop) p_eai_graph_has_next_##T(graph, loop)

#endif // EAI_TOOLBOX_EAI_GRAPH_H
