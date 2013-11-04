/**
 * @file randomizers/fixed_degree_randomizer.h
 * @brief definition of class fixed_degree_randomizer
 */

#include "fixed_degree_randomizer.h"
#include <boost/graph/random.hpp>
#include <assert.h>

remove_add_edges_pair fixed_degree_randomizer::get_step()
{
    remove_add_edges_pair r_a_p;
    graph_types::edge e1 = boost::random_edge(m_graph,
        m_random_generator);
    graph_types::edge e2 = boost::random_edge(m_graph,
        m_random_generator);
    graph_types::vertex vs1 = boost::source(e1, m_graph);
    graph_types::vertex vt1 = boost::target(e1, m_graph);
    graph_types::vertex vs2 = boost::source(e2, m_graph);
    graph_types::vertex vt2 = boost::target(e2, m_graph);
    while(e1 == e2 || vs1 == vs2 || vt1 == vt2 ||
        true == boost::edge(vs1, vs2, m_graph).second ||
        true == boost::edge(vt1, vt2, m_graph).second)
    {
        e2 = boost::random_edge(m_graph, m_random_generator);
        vs2 = boost::source(e2, m_graph);
        vt2 = boost::target(e2, m_graph);
    }
    assert(e1 != e2 && vs1 != vs2 && vt1 != vt2 &&
        false == boost::edge(vs1, vs2, m_graph).second &&
        false == boost::edge(vt1, vt2, m_graph).second);
    r_a_p.first.insert(graph_types::vertex_pair(vs1, vt1));
    r_a_p.first.insert(graph_types::vertex_pair(vs2, vt2));
    r_a_p.second.insert(graph_types::vertex_pair(vs1, vs2));
    r_a_p.second.insert(graph_types::vertex_pair(vt1, vt2));
    return r_a_p;
}

randomization_type fixed_degree_randomizer::get_type() const
{
    return FIXED_DEGREE;
}

fixed_degree_randomizer::fixed_degree_randomizer(
    graph_types::undirected_graph& graph,
    graph_types::sequent_null_edges& non_existing_edges)
    : randomizator_base(graph, non_existing_edges)
{}
