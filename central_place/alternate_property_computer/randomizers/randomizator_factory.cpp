/**
 * @file randomizers/randomizator_factory.cpp 
 * @brief definition of randomizator_factory class
 */

#include "randomizator_factory.h"
#include <assert.h>
#include <functional>

typedef std::map<randomization_type, std::function<randomizator_base* 
    (graph_types::undirected_graph& graph)>> type_to_randomizator;

static type_to_randomizator s_type_to_randomizator = {
    std::make_pair(FIXED_DEGREE,
        [] (graph_types::undirected_graph& g) -> randomizator_base* {
            return new fixed_degree_randomizer(g);
        }),
    std::make_pair(RANDOM_SWITCH,
        [] (graph_types::undirected_graph& g) -> randomizator_base* {
            return new random_switch_randomizer(g);
        })
};

randomizator_base* randomizator_factory::get_randomizator(
    graph_types::undirected_graph& graph, randomization_type t)
{
    auto it = s_type_to_randomizator.find(t);
    assert(s_type_to_randomizator.end() != it);
    return (it->second)(graph);
}
