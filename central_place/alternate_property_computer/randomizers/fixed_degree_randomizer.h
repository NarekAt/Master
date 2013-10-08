/**
 * @file randomizers/fixed_degree_randomizer.h
 * @description declaration of class fixed_degree_randomizer
 */

#pragma once

#include "randomizator_base.h"

/**
 * @class fixed_degree_randomizer
 * @brief Makes randomization by keeping degree of random
 *        selected vertices.
 */
class fixed_degree_randomizer : public randomizator_base
{
public:
    /**
     * @brief Gets step of randomizator.
     */
    virtual remove_add_edges_pair get_step();

public:
    /**
     * @brief Constructor
     * @param graph Graph to perform randomization.
     */
    fixed_degree_randomizer(graph_types::undirected_graph& graph);
};
