/**
 * @file randomizers/random_switch_randomizer.h
 * @brief definition of class random_switch_randomizer
 */

#include "random_switch_randomizer.h"
#include <boost/graph/random.hpp>
#include <assert.h>

remove_add_edges_pair random_switch_randomizer::get_step()
{
    remove_add_edges_pair r_a_p;
    std::uniform_int_distribution<> distrib(0, 
        m_non_existing_edges.size() - 1);
    auto variate_generator = new boost::variate_generator<
        graph_types::random_generator,
        std::uniform_int_distribution<>>(m_random_generator, distrib);
    graph_types::edge e1 = boost::random_edge(m_graph,
        m_random_generator);
    graph_types::vertex vs1 = boost::source(e1, m_graph);
    graph_types::vertex vt1 = boost::target(e1, m_graph);
    r_a_p.first.insert(graph_types::vertex_pair(vs1, vt1));
    unsigned n_e_e_index = (*variate_generator)();
    r_a_p.second.insert(m_non_existing_edges[n_e_e_index]);
    m_non_existing_edges.erase(m_non_existing_edges.begin() + n_e_e_index);
    delete variate_generator;
    return r_a_p;
}

randomization_type random_switch_randomizer::get_type() const
{
    return RANDOM_SWITCH;
}

random_switch_randomizer::random_switch_randomizer(
    graph_types::undirected_graph& graph,
    graph_types::sequent_null_edges& non_existing_edges)
    : randomizator_base(graph, non_existing_edges)
{}
