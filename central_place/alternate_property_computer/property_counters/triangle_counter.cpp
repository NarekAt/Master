/**
 * @file property_counters/triangle_counter.cpp
 * @description definition of triangle_counter class.
 */

#include "triangle_counter.h"
#include "utility.h"
#include <vector>

unsigned triangle_counter::compute_initial_count() const
{
    unsigned num = 0;
    graph_types::edge_iterator ei, ei_end;
    // iterate over edges
    boost::tie(ei, ei_end) = boost::edges(m_graph);
    for (; ei != ei_end; ++ei) {
        // get edge vertices
        graph_types::vertex vs = boost::source(*ei, m_graph);
        graph_types::vertex vt = boost::target(*ei, m_graph);
        // get adjacent vertices for them
        graph_types::adjacency_iterator vsi, vsi_end, vti, vti_end;
        boost::tie(vsi, vsi_end) = 
            boost::adjacent_vertices(vs, m_graph);
        for(; vsi != vsi_end; ++vsi) {
            /**
             * TODO: make faster, by not iterating on vertices of
             *       vt, just check, 
             *       boost::edge(vsi, vt, m_graph).second
             */
            boost::tie(vti, vti_end) = 
                boost::adjacent_vertices(vt, m_graph);
            for(; vti != vti_end; ++vti) {
                if(*vsi == *vti)
                {
                    ++num;
                }
            }
        }
    }
    // every triangle is counted triple
    num = num / 3;
    return num;
}

unsigned triangle_counter::compute_decrease_after_remove(
    const graph_types::null_edges& e) const
{
    unsigned d_c = 0;
    for (const auto& n_e : e) {
        graph_types::vertex vs = n_e.first;
        graph_types::vertex vt = n_e.second;
        boost::remove_edge(vs, vt, m_graph);
        graph_types::adjacency_iterator v, v_e;
        boost::tie(v, v_e) = boost::adjacent_vertices(vs, m_graph);
        while (v != v_e) {
            if (*v != vt &&
                true == boost::edge(*v, vt, m_graph).second) {
                ++d_c;
            }
            ++v;
        }
    }
    return d_c;
}

unsigned triangle_counter::compute_increase_after_add(
    const graph_types::null_edges& e) const
{
    // The same code for both: increase and decrease :)
    return compute_decrease_after_remove(e);
}

alternate_property_type triangle_counter::get_type() const
{
    return TRIANGLE_COUNT;
}

triangle_counter::triangle_counter(
    graph_types::undirected_graph& graph)
    : property_counter_base(graph)
{}
