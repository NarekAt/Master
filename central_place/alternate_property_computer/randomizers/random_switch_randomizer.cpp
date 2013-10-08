/**
 * @file randomizers/random_switch_randomizer.h
 * @description definition of class random_switch_randomizer
 */

#include "random_switch_randomizer.h"
#include <boost/graph/random.hpp>
#include <assert.h>

remove_add_edges_pair random_switch_randomizer::get_step()
{
    remove_add_edges_pair r_a_p;
    int vertices_count = boost::num_vertices(m_graph);
    int all_edges_count = vertices_count * (vertices_count - 1) / 2;
    int existing_edges_count = boost::num_edges(m_graph);
    int non_existing_edges_count = 
        all_edges_count - existing_edges_count;
    if (0 == non_existing_edges_count || 0 == existing_edges_count) {
        return r_a_p;
    }
    graph_types::sequent_null_edges non_existing_edges;
    non_existing_edges.reserve(non_existing_edges_count);
    graph_types::vertex_iterator v, v_end;
    boost::tie(v, v_end) = boost::vertices(m_graph);
    for(; v != v_end; ++v)
    {
        graph_types::vertex_iterator v1 = v;
        for(++v1; v1 != v_end; ++v1)
        {
            if(false == boost::edge(*v, *v1, m_graph).second)
            {
                non_existing_edges.push_back(std::make_pair(*v, *v1));
            }
        }
    }
    assert(!non_existing_edges.empty());
    std::uniform_int_distribution<> distrib(0, 
        non_existing_edges.size() - 1);
    auto variate_generator = new boost::variate_generator<
        graph_types::random_generator,
        std::uniform_int_distribution<>>(m_random_generator, distrib);
    graph_types::edge e1 = boost::random_edge(m_graph,
        m_random_generator);
    graph_types::vertex vs1 = boost::source(e1, m_graph);
    graph_types::vertex vt1 = boost::target(e1, m_graph);
    r_a_p.first.insert(graph_types::vertex_pair(vs1, vt1));
    r_a_p.second.insert(non_existing_edges[(*variate_generator)()]);
    delete variate_generator;
    return r_a_p;
}

random_switch_randomizer::random_switch_randomizer(
    graph_types::undirected_graph& graph)
    : randomizator_base(graph)
{}
