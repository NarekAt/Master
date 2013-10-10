/**
 * @file property_counters/property_counter_factory.cpp
 * @description definition of property_counter_factory class
 */

#include "property_counter_factory.h"
#include <assert.h>
#include <functional>

typedef std::map<alternate_property_type, std::function<
    property_counter_base* (const graph_types::undirected_graph& g)>>
        type_to_property_counter;

static type_to_property_counter s_type_to_property_counter = {
    std::make_pair(TRIANGLE_COUNT,
        [] (const graph_types::undirected_graph& g) -> 
            property_counter_base* {
            return new triangle_counter(g);
        })
};

property_counter_base* property_counter_factory::get_counter(
    const graph_types::undirected_graph& graph,
    alternate_property_type t)
{
    auto it = s_type_to_property_counter.find(t);
    assert(s_type_to_property_counter.end() != it);
    return (it->second)(graph);
}
