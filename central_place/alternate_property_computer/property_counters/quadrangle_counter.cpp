/**
 * @file property_counters/quadrangle_counter.cpp
 * @brief definition of quadrangle_counter class.
 */

#include "quadrangle_counter.h"
#include "utility.h"
#include <vector>

// TODO: try to make faster.
unsigned quadrangle_counter::compute_initial_count() const
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
            boost::tie(vti, vti_end) = 
                boost::adjacent_vertices(vt, m_graph);
            for(; vti != vti_end; ++vti) {
                if(*vsi != *vti && *vsi != vt && *vti != vs && 
                    boost::edge(*vsi, *vti, m_graph).second)
                {
                    ++num;
                }
            }
        }
    }
    // every quadrangle is counted four times.
    num = num / 4;
    return num;
}

unsigned quadrangle_counter::compute_decrease_after_remove(
    const graph_types::null_edges& e) const
{

    unsigned d_c = 0;
    for (const auto& n_e : e) {
        graph_types::vertex vs = n_e.first;
        graph_types::vertex vt = n_e.second;
        boost::remove_edge(vs, vt, m_graph);
        graph_types::adjacency_iterator vsi, vsi_end, vti, vti_end;
        boost::tie(vsi, vsi_end) = 
            boost::adjacent_vertices(vs, m_graph);
        for(; vsi != vsi_end; ++vsi) {
            boost::tie(vti, vti_end) = 
                boost::adjacent_vertices(vt, m_graph);
            for(; vti != vti_end; ++vti) {
                if(*vsi != *vti &&
                    boost::edge(*vsi, *vti, m_graph).second)
                {
                    ++d_c;
                }
            }
        }
    }
    return d_c;
}

unsigned quadrangle_counter::compute_increase_after_add(
    const graph_types::null_edges& e) const
{
    // The same code for both: increase and decrease :)
    return compute_decrease_after_remove(e);
}

alternate_property_type quadrangle_counter::get_type() const
{
    return QUADRANGLE_COUNT;
}

quadrangle_counter::quadrangle_counter(
    graph_types::undirected_graph& graph)
    : property_counter_base(graph)
{}
