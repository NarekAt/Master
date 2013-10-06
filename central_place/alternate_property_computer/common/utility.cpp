/**
 * @file common/utility.cpp
 * @description definitions of utility functions
 */

#include "utility.h"
#include <assert.h>

namespace utility
{

graph_types::vertex get_another_vertex(
    graph_types::vertex v, graph_types::edge e,
    const graph_types::undirected_graph& graph)
{
    graph_types::vertex s = boost::source(e, graph);
    graph_types::vertex t = boost::target(e, graph);
    if (v == s) {
        return t;
    }
    assert(v == t);
    return s;
}

}
