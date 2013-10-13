/**
 * @file randomizers/randomizator_factory.h 
 * @brief declaration of randomizator_factory class
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
     *        by given randomization_type.
     * @param graph Graph for randomizator.
     * @param t Randomization type.
     * @note Got pointer must be deleted after use.
     */
    static randomizator_base* get_randomizator(
        graph_types::undirected_graph& graph, randomization_type t);
};
