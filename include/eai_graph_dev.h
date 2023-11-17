//
// Created by DavideLoconte on 20-Nov-23.
//

#ifndef EAI_TOOLBOX_EAI_GRAPH_DEV_H
#define EAI_TOOLBOX_EAI_GRAPH_DEV_H
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

#include <ulib.h>

#define EAI_GRAPH_MEM_ERROR ubit_bit(8, 1)
#define EAI_GRAPH_NOT_FOUND ubit_bit(8, 2)
#define EAI_GRAPH_DUPLICATE ubit_bit(8, 3)


typedef ulib_uint EaiGraphNode_dev;

typedef struct EaiGraphEdge_s_dev {
    ulib_uint to;
    ulib_uint val;
} EaiGraphEdge_dev;

UVEC_DECL_EQUATABLE(EaiGraphNode_dev)
UVEC_DECL_COMPARABLE(EaiGraphEdge_dev)

typedef struct EaiGraph_s_dev {
    UVec(EaiGraphNode_dev) nodes;
    UVec(EaiGraphEdge_dev) edges;
    UVec(ulib_uint) limits;
    UBit(8) flags;
} EaiGraph_dev;

typedef struct EaiGraphEdgeLoop_s_dev {
    EaiGraphNode_dev start, end;
    ulib_uint value;
    ulib_uint start_i;
    ulib_uint end_i;
    ulib_uint i;
    ulib_uint count;
} EaiGraphEdgeLoop_dev;

EaiGraph_dev eai_graph_dev(void);

void eai_graph_deinit_dev(EaiGraph_dev *g);
bool eai_graph_add_node_dev(EaiGraph_dev *g, EaiGraphNode_dev n);

bool eai_graph_add_edge_dev(EaiGraph_dev *g,
                            EaiGraphNode_dev u,
                            EaiGraphNode_dev v,
                            ulib_uint e);

bool eai_graph_set_node_dev(EaiGraph_dev *g, EaiGraphNode_dev start, EaiGraphNode_dev end);
ulib_uint eai_graph_set_edge_dev(EaiGraph_dev *g,
                         EaiGraphNode_dev u,
                         EaiGraphNode_dev v,
                         ulib_uint e);

bool eai_graph_has_node_dev(EaiGraph_dev *g, EaiGraphNode_dev n);
bool eai_graph_has_edge_dev(EaiGraph_dev *g, EaiGraphNode_dev u, EaiGraphNode_dev v);
ulib_uint eai_graph_get_edge_dev(EaiGraph_dev *g,
                                        EaiGraphNode_dev u,
                                        EaiGraphNode_dev v);
bool eai_graph_del_node_dev(EaiGraph_dev *g, EaiGraphNode_dev node);
ulib_uint eai_graph_del_edge_dev(EaiGraph_dev *g,
                                        EaiGraphNode_dev start,
                                        EaiGraphNode_dev end);


EaiGraphEdgeLoop_dev p_eai_graph_start_iterator_dev(EaiGraph_dev *g);
void p_eai_graph_next_dev(EaiGraph_dev *g, EaiGraphEdgeLoop_dev *l);

void p_eai_graph_next_to_dev(EaiGraph_dev *g, EaiGraphEdgeLoop_dev *l, EaiGraphNode_dev to);
EaiGraphEdgeLoop_dev p_eai_graph_start_iterator_to_dev(EaiGraph_dev *g, EaiGraphNode_dev to);

EaiGraphEdgeLoop_dev p_eai_graph_start_iterator_from_dev(EaiGraph_dev *g, EaiGraphNode_dev from);
void p_eai_graph_next_from_dev(EaiGraph_dev *g, EaiGraphEdgeLoop_dev *l, EaiGraphNode_dev from);




/**
 * Declares a new graph variable.
 *
 * @param T graph type.
 *
 */
#define EaiGraph(T) EaiGraph_dev

/**
 * Loop over all nodes in a graph
 * @param graph the graph
 * @param node the node structure iterator name
 */
#define eai_graph_foreach_node_dev(graph, node) uvec_foreach(EaiGraphNode_dev, &(graph)->nodes, node)

/**
 * Loop over all edges in a graph
 * @param graph the graph
 * @param edge the edge structure iterator name
 */
#define eai_graph_foreach_edge_dev(graph, edge)                                                        \
    for(EaiGraphEdgeLoop_dev edge = p_eai_graph_start_iterator_dev((graph));                       \
        edge.i < edge.count;                                                \
        p_eai_graph_next_dev((graph), &(edge)))

/**
 * Loop over all edges starting from the specified node
 * @param graph the graph
 * @param start the node starting node
 * @param edge iterator name
 */
#define eai_graph_foreach_edge_from_dev(graph, start, edge)                                     \
    for(EaiGraphEdgeLoop_dev edge = p_eai_graph_start_iterator_from_dev((graph), (start));         \
        edge.i < edge.count;                                                \
        p_eai_graph_next_from_dev((graph), &(edge), start))

/**
 * Loop over all edges ending into the specified node
 * @param graph the graph
 * @param end the node starting node
 * @param edge iterator name
 */
#define eai_graph_foreach_edge_to_dev(graph, end, edge)                                             \
    for(EaiGraphEdgeLoop_dev edge = p_eai_graph_start_iterator_to_dev((graph), (end));             \
        edge.i < edge.count;                                                \
        p_eai_graph_next_to_dev((graph), &(edge), end))


#endif // EAI_TOOLBOX_EAI_GRAPH_DEV_H
