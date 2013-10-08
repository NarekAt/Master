/**
 * @file property_counters/property_counter_factory.h
 * @description declaration of property counter_factory class
 */

#pragma once

#include "types.h"
#include "triangle_counter.h"

/**
 * @class property_counter_factory
 * @brief creater of property_counters.
 */
class property_counter_factory
{
public:
    /**
     * @brief Creates and gets property counter
     *        by specialized alternate_property_type.
     * @note Got pointer must be deleted after use.
     */
    template <alternate_property_type>
    static property_counter_base* get_counter(
        const graph_types::undirected_graph& graph);
};


template <>
property_counter_base* property_counter_factory::
    get_counter<TRIANGLE_COUNT>(
        const graph_types::undirected_graph& graph)
{
    return new triangle_counter(graph);
}
