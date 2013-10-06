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
    const std::set<graph_types::edge>& e) const
{
    unsigned d_c = 0;
    std::map<graph_types::vertex, std::vector<graph_types::edge>>
        vertex_to_edges;
    for (auto& n_e : e) {
        vertex_to_edges[boost::source(n_e, m_graph)].push_back(n_e);
        vertex_to_edges[boost::target(n_e, m_graph)].push_back(n_e);
    }
    for (auto& v_e : vertex_to_edges) {
        if (2 == v_e.second.size()) {
            graph_types::vertex an_v1 = utility::get_another_vertex(
                v_e.first, v_e.second[0], m_graph); 
            graph_types::vertex an_v2 = utility::get_another_vertex(
                v_e.first, v_e.second[1], m_graph);
            auto edge_info_pair = boost::edge(an_v1, an_v2, m_graph);
            if (true == edge_info_pair.second
                || e.end() != e.find(edge_info_pair.first)) {
                ++d_c;
            }
        }
        if (2 < v_e.second.size()) {
            for(int i = 0; i < v_e.second.size() - 1; ++i) {
                for (int j = i + 1; j < v_e.second.size(); ++j) {
                    graph_types::vertex an_v1 = utility::
                        get_another_vertex(v_e.first, v_e.second[i],
                            m_graph); 
                    graph_types::vertex an_v2 = utility::
                        get_another_vertex(v_e.first, v_e.second[j],
                            m_graph); 
                    auto edge_info_pair = boost::edge(
                        an_v1, an_v2, m_graph);
                    if (true == edge_info_pair.second
                        || e.end() != e.find(edge_info_pair.first)) {
                        ++d_c;
                    }
                }
            }
        }
    }
    for (const auto& n_e : e) {
        graph_types::vertex vs = boost::source(n_e, m_graph);
        graph_types::vertex vt = boost::target(n_e, m_graph);
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
    const std::set<graph_types::edge>& e) const
{
    // The same code for both: increase and decrease :)
    return compute_decrease_after_remove(e);
}

triangle_counter::triangle_counter(
    const graph_types::undirected_graph& graph)
    : property_counter_base(graph)
{}
