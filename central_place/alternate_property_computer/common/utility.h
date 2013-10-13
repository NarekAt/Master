/**
 * @file common/utility.h
 * @brief declarations of utility functions
 */

#pragma once

#include "types.h"

namespace utility
{

/**
 * @brief Returns another vertex of the edge.
 * @param v Initial vertex
 * @param e Edge
 * @param graph Graph
 * @return Another vertex of v
 * @pre Given edge must exists
 */
graph_types::vertex get_another_vertex(
    graph_types::vertex v, graph_types::edge e,
    const graph_types::undirected_graph& graph);

/**
 * @brief Returns another vertex of the vertex pair.
 * @param v Initial vertex
 * @param v_e Vertex pair
 * @return Another vertex of v
 */
graph_types::vertex get_another_vertex(
    graph_types::vertex v, graph_types::vertex_pair& v_e);

}
