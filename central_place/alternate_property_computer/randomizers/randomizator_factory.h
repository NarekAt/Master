/**
 * @file randomizators/randomizator_factory.h
 * @description declaration of property counter_factory class
 */

#pragma once

#include "types.h"
#include "random_switch_randomizer.h"
#include "fixed_degree_randomizer.h"

/**
 * @class randomizator_factory
 * @brief creater of randomizators.
 */
class randomizator_factory
{
public:
    /**
     * @brief Creates and gets randomizator
     *        by specialized randomization_type.
     * @note Got pointer must be deleted after use.
     */
    template <randomization_type>
    static randomizator_base* get_randomizator(
        graph_types::undirected_graph& graph);
};


template <>
randomizator_base* randomizator_factory::
    get_randomizator<FIXED_DEGREE>(
        graph_types::undirected_graph& graph)
{
    return new fixed_degree_randomizer(graph);
}

template <>
randomizator_base* randomizator_factory::
    get_randomizator<RANDOM_SWITCH>(
        graph_types::undirected_graph& graph)
{
    return new random_switch_randomizer(graph);
}
