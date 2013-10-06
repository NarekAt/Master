/**
 * @file common/utility.h
 * @description declarations of utility functions
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
 */
graph_types::vertex get_another_vertex(
    graph_types::vertex v, graph_types::edge e,
    const graph_types::undirected_graph& graph);

}
