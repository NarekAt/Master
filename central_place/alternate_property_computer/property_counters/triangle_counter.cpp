/**
 * @file property_counters/triangle_counter.cpp
 * @brief definition of triangle_counter class.
 */

#include "triangle_counter.h"
#include "utility.h"
#include "graph.h"
#include <vector>

unsigned triangle_counter::compute_initial_count() const
{
    unsigned num = 0;
    const auto edges = m_graph.edges_sequence();
    for (const auto& e : edges) {
        const graph_types::vertex vs = e.first;
        const graph_types::vertex vt = e.second;
        const auto vs_n = m_graph.neighbors_sequence(vs);
        for (const auto& vsi : vs_n) {
            if (vsi != vt && m_graph.edge_exists(vsi, vt)) {
                ++num;
            }
        }
    }
    return num / 3;
}

unsigned triangle_counter::compute_decrease_after_remove(
    const graph_types::sequent_null_edges& e) const
{
    unsigned d_c = 0;
    for (const auto& n_e : e) {
        const graph_types::vertex vs = n_e.first;
        const graph_types::vertex vt = n_e.second;
        m_graph.remove_edge(vs, vt);
        const auto vs_n = m_graph.neighbors_sequence(vs);
        for (const auto& vsi : vs_n) {
            if (m_graph.edge_exists(vsi, vt)) {
                ++d_c;
            }
        }
    }
    return d_c;
}

unsigned triangle_counter::compute_increase_after_add(
    const graph_types::sequent_null_edges& e) const
{
    // The same code for both: increase and decrease :)
    return compute_decrease_after_remove(e);
}

alternate_property_type triangle_counter::get_type() const
{
    return TRIANGLE_COUNT;
}

triangle_counter::triangle_counter(
    graph_types::graph& graph)
    : property_counter_base(graph)
{}
