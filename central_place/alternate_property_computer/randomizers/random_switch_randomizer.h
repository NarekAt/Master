/**
 * @file randomizers/random_switch_randomizer.h
 * @description declaration of class random_switch_randomizer
 */

#pragma once

#include "randomizator_base.h"

/**
 * @class random_switch_randomizer
 * @brief Makes randomization random edge to remove and random
 *        edge to add.
 */
class random_switch_randomizer : public randomizator_base
{
public:
    /**
     * @brief Gets step of randomizator.
     */
    virtual remove_add_edges_pair get_step();

public:
    /**
     * @brief Gets type of randomizator (RANDOM_SWITCH).
     */
    virtual randomization_type get_type() const;

public:
    /**
     * @brief Constructor
     * @param graph Graph to perform randomization.
     */
    random_switch_randomizer(graph_types::undirected_graph& graph);
};
