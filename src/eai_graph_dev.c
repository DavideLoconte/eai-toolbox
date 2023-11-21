//
// Created by DavideLoconte on 20-Nov-23.
//

#include "eai_graph_dev.h"

static inline bool edge_eq(EaiGraphEdge_dev a, EaiGraphEdge_dev b) { return a.to == b.to; }
static inline bool edge_cmp(EaiGraphEdge_dev a, EaiGraphEdge_dev b) { return a.to < b.to; }
static inline bool node_eq(EaiGraphNode_dev a, EaiGraphNode_dev b) { return a == b; }

UVEC_IMPL_EQUATABLE(EaiGraphNode_dev, node_eq)
UVEC_IMPL_COMPARABLE(EaiGraphEdge_dev, edge_eq, edge_cmp)

EaiGraph_dev eai_graph_dev(void)
{
    EaiGraph_dev result;
    result.nodes = uvec(EaiGraphNode_dev);
    result.edges = uvec(EaiGraphEdge_dev);
    result.limits = uvec(ulib_uint);
    result.flags = ubit_none(8);
    return result;
}

void eai_graph_deinit_dev(EaiGraph_dev *g)
{
    uvec_deinit(ulib_uint, &g->limits);
    uvec_deinit(EaiGraphEdge_dev, &g->edges);
    uvec_deinit(EaiGraphNode_dev, &g->nodes);
}

bool eai_graph_add_node_dev(EaiGraph_dev *g, EaiGraphNode_dev n)
{
    g->flags = ubit_none(8);

    if(uvec_contains(EaiGraphNode_dev, &g->nodes, n)) {
        g->flags = ubit_set(8, EAI_GRAPH_DUPLICATE, g->flags);
        return false;
    }

    uvec_ret ret = uvec_push(EaiGraphNode_dev, &g->nodes, n);

    if(ret != UVEC_OK) {
        g->flags = ubit_set(8, EAI_GRAPH_MEM_ERROR, g->flags);
        return false;
    }

    ret = uvec_push(ulib_uint, &g->limits, uvec_count(EaiGraphEdge_dev, &g->edges));

    if(ret != UVEC_OK) {
        uvec_remove(EaiGraphNode_dev, &g->nodes, n);
        g->flags = ubit_set(8, EAI_GRAPH_MEM_ERROR, g->flags);
        return false;
    }

    return true;
}

bool eai_graph_add_edge_dev(EaiGraph_dev *g,
                            EaiGraphNode_dev start,
                            EaiGraphNode_dev end,
                            ulib_uint val)
{
    g->flags = ubit_none(8);
    ulib_uint start_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, start);
    ulib_uint end_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, end);

    if(!uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, start_idx) ||
       !uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, end_idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return false;
    }

    EaiGraphEdge_dev edge;
    edge.to = end_idx;
    edge.val = val;

    ulib_uint offset_start = 0;
    ulib_uint offset_end = uvec_get(ulib_uint, &g->limits, start_idx);
    if (start_idx > 0) {
        offset_start = uvec_get(ulib_uint, &g->limits, start_idx - 1);
    }

    UVec(EaiGraphEdge_dev) edges  = uvec_view(EaiGraphEdge_dev, &g->edges, offset_start, offset_end - offset_start);
    ulib_uint insert_idx = uvec_insertion_index_sorted(EaiGraphEdge_dev, &edges, edge);
    uvec_ret ret = uvec_insert_at(EaiGraphEdge_dev, &g->edges, offset_start + insert_idx, edge);

    if(ret != UVEC_OK) {
        g->flags = ubit_set(8, EAI_GRAPH_MEM_ERROR, g->flags);
        return false;
    }

    for (ulib_uint i = start_idx; uvec_index_is_valid(ulib_uint, &g->limits, i); i++) {
        ulib_uint old_val = uvec_get(ulib_uint, &g->limits, i);
        uvec_set(ulib_uint, &g->limits, i, old_val + 1);
    }

    return true;
}

bool eai_graph_set_node_dev(EaiGraph_dev *g, EaiGraphNode_dev old, EaiGraphNode_dev new)
{
    g->flags = ubit_none(8);
    ulib_uint idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, old);

    if(!uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return false;
    }

    uvec_data(EaiGraphNode_dev, &g->nodes)[idx] = new;

    return true;
}

ulib_uint eai_graph_set_edge_dev(EaiGraph_dev *g,
                                        EaiGraphNode_dev start,
                                        EaiGraphNode_dev end,
                                        ulib_uint e)
{
    ulib_uint old_val;
    memset(&old_val, 0, sizeof(ulib_uint));

    g->flags = ubit_none(8);
    ulib_uint start_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, start);
    ulib_uint end_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, end);

    if(!uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, start_idx) ||
       !uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, end_idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return old_val;
    }

    EaiGraphEdge_dev new;
    new.to = end_idx;
    new.val = e;

    ulib_uint offset_start = 0;
    ulib_uint offset_end = uvec_get(ulib_uint, &g->limits, start_idx);
    if (start_idx > 0) {
        offset_start = uvec_get(ulib_uint, &g->limits, start_idx - 1);
    }

    UVec(EaiGraphEdge_dev) edges  = uvec_view(EaiGraphEdge_dev, &g->edges, offset_start, offset_end - offset_start);
    ulib_uint edge_idx = uvec_index_of_sorted(EaiGraphEdge_dev, &edges, new);
    if(!uvec_index_is_valid(EaiGraphEdge_dev, &edges, edge_idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return old_val;
    }

    old_val = uvec_get(EaiGraphEdge_dev, &edges, edge_idx).val;
    uvec_set(EaiGraphEdge_dev, &edges, edge_idx, new);
    return old_val;
}

ulib_uint eai_graph_get_edge_dev(EaiGraph_dev *g,
                                 EaiGraphNode_dev start,
                                 EaiGraphNode_dev end)
{
    ulib_uint old_val;
    memset(&old_val, 0, sizeof(ulib_uint));

    g->flags = ubit_none(8);
    ulib_uint start_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, start);
    ulib_uint end_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, end);

    if(!uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, start_idx) ||
       !uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, end_idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return old_val;
    }

    EaiGraphEdge_dev dummy;
    dummy.to = end_idx;

    ulib_uint offset_start = 0;
    ulib_uint offset_end = uvec_get(ulib_uint, &g->limits, start_idx);
    if (start_idx > 0) {
        offset_start = uvec_get(ulib_uint, &g->limits, start_idx - 1);
    }

    UVec(EaiGraphEdge_dev) edges  = uvec_view(EaiGraphEdge_dev, &g->edges, offset_start, offset_end - offset_start);
    ulib_uint edge_idx = uvec_index_of_sorted(EaiGraphEdge_dev, &edges, dummy);

    if(!uvec_index_is_valid(EaiGraphEdge_dev, &edges, edge_idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return old_val;
    }

    return uvec_get(EaiGraphEdge_dev, &edges, edge_idx).val;
}

bool eai_graph_del_node_dev(EaiGraph_dev *g, EaiGraphNode_dev node)
{
    g->flags = ubit_none(8);
    ulib_uint idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, node);

    if(!uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return false;
    }

    ulib_uint offset_start = 0;
    ulib_uint offset_end = uvec_get(ulib_uint, &g->limits, idx);
    if (idx > 0) {
        offset_start = uvec_get(ulib_uint, &g->limits, idx - 1);
    }
    ulib_uint edge_count = offset_end - offset_start;

    for (ulib_uint i = offset_start; i < offset_end; i++) {
        uvec_remove_at(EaiGraphEdge_dev, &g->edges, i);
    }

    for (ulib_uint i = idx; uvec_index_is_valid(ulib_uint, &g->limits, i); i++) {
        ulib_uint old_val = uvec_get(ulib_uint, &g->limits, i);
        uvec_set(ulib_uint, &g->limits, i, old_val - edge_count);
    }

    uvec_foreach_reverse(EaiGraphEdge_dev, &g->edges, e) {
        if (e.item->to == node) {
            uvec_remove_at(EaiGraphEdge_dev, &g->edges, e.i);
            uvec_foreach_reverse(ulib_uint, &g->limits, l) {
                if (*l.item >= e.i) {
                    *l.item -= 1;
                } else {
                    break;
                }
            }
        }
    }

    uvec_remove_at(EaiGraphNode_dev, &g->nodes, idx);
    uvec_remove_at(ulib_uint, &g->limits, idx);
    return true;
}

ulib_uint eai_graph_del_edge_dev(EaiGraph_dev *g, EaiGraphNode_dev start, EaiGraphNode_dev end)
{
    ulib_uint old_val;
    memset(&old_val, 0, sizeof(ulib_uint));

    g->flags = ubit_none(8);
    ulib_uint start_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, start);
    ulib_uint end_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, end);

    if(!uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, start_idx) ||
       !uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, end_idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return old_val;
    }

    ulib_uint offset_start = 0;
    if (start_idx > 0) {
        offset_start = uvec_get(ulib_uint, &g->limits, start_idx - 1);
    }

    EaiGraphEdge_dev dummy;
    dummy.to = end_idx;

    ulib_uint idx = uvec_index_of_sorted(EaiGraphEdge_dev, &g->edges, dummy) + offset_start;
    old_val = uvec_get(EaiGraphEdge_dev, &g->edges, idx).val;
    uvec_remove_at(EaiGraphEdge_dev, &g->edges, idx);

    uvec_foreach_reverse(ulib_uint, &g->limits, l) {
        if (*l.item >= idx) {
            *l.item -= 1;
        } else {
            break;
        }
    }

    return old_val;
}

bool eai_graph_has_node_dev(EaiGraph_dev *g, EaiGraphNode_dev node)
{
    return uvec_contains(EaiGraphNode_dev, &g->nodes, node);
}

bool eai_graph_has_edge_dev(EaiGraph_dev *g, EaiGraphNode_dev start, EaiGraphNode_dev end)
{
    g->flags = ubit_none(8);
    ulib_uint start_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, start);
    ulib_uint end_idx = uvec_index_of(EaiGraphNode_dev, &g->nodes, end);

    if(!uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, start_idx) ||
       !uvec_index_is_valid(EaiGraphNode_dev, &g->nodes, end_idx)) {
        g->flags = ubit_set(8, EAI_GRAPH_NOT_FOUND, g->flags);
        return false;
    }

    ulib_uint offset_start = 0;
    ulib_uint offset_end = uvec_get(ulib_uint, &g->limits, start_idx);
    if (start_idx > 0) {
        offset_start = uvec_get(ulib_uint, &g->limits, start_idx - 1);
    }

    UVec(EaiGraphEdge_dev) edges  = uvec_view(EaiGraphEdge_dev, &g->edges, offset_start, offset_end - offset_start);
    EaiGraphEdge_dev dummy;
    dummy.to = end_idx;

    return uvec_contains_sorted(EaiGraphEdge_dev, &edges, dummy);
}

EaiGraphEdgeLoop_dev p_eai_graph_start_iterator_dev(EaiGraph_dev *g)
{
    EaiGraphEdgeLoop_dev loop;
    EaiGraphEdge_dev edge = uvec_first(EaiGraphEdge_dev, &g->edges);
    loop.count = uvec_count(EaiGraphEdge_dev, &g->edges);
    loop.i = 0;

    uvec_foreach_reverse(ulib_uint, &g->limits, i) {
        if (0 < *i.item) {
            loop.start_i = i.i;
        }
    }

    loop.start = uvec_get(EaiGraphNode_dev, &g->nodes, loop.start_i);
    loop.end = uvec_get(EaiGraphNode_dev, &g->nodes, edge.to);
    loop.end_i = edge.to;
    return loop;
}

EaiGraphEdgeLoop_dev p_eai_graph_start_iterator_from_dev(EaiGraph_dev *g, EaiGraphNode_dev from)
{
    EaiGraphEdgeLoop_dev loop = p_eai_graph_start_iterator_dev(g);
    if (loop.start != from) {
        p_eai_graph_next_from_dev(g, &loop, from);
    }
    return loop;
}

EaiGraphEdgeLoop_dev p_eai_graph_start_iterator_to_dev(EaiGraph_dev *g, EaiGraphNode_dev to)
{
    EaiGraphEdgeLoop_dev loop = p_eai_graph_start_iterator_dev(g);
    if (loop.end != to) {
        p_eai_graph_next_to_dev(g, &loop, to);
    }
    return loop;
}

void p_eai_graph_next_from_dev(EaiGraph_dev *g, EaiGraphEdgeLoop_dev *l, EaiGraphNode_dev from)
{
    do {
        p_eai_graph_next_dev(g, l);
    } while (l->i < l->count && l->start != from);
}

void p_eai_graph_next_to_dev(EaiGraph_dev *g, EaiGraphEdgeLoop_dev *l, EaiGraphNode_dev to)
{
    do {
        p_eai_graph_next_dev(g, l);
    } while (l->i < l->count && l->end != to);
}

void p_eai_graph_next_dev(EaiGraph_dev *g, EaiGraphEdgeLoop_dev *l)
{
    l->i += 1;
    if (l->i >= l->count) {
        return;
    }

    EaiGraphEdge_dev edge = uvec_get(EaiGraphEdge_dev, &g->edges, l->i);
    uvec_foreach_reverse(ulib_uint, &g->limits, i) {
        if (l->i < *i.item) {
            l->start_i = i.i;
        }
    }

    l->start =  uvec_get(EaiGraphNode_dev, &g->nodes, l->start_i);
    l->end = uvec_get(EaiGraphNode_dev, &g->nodes, edge.to);
    l->end_i = edge.to;
}