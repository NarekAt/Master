/**
 * @file randomizators/randomizator_base
 * @description declaration of class randomizator_base
 */

#pragma once

#include "types.h"

/**
 * @class randomizator_base
 * @brief class for performing randomization step.
 */
class randomizator_base
{
public:
    /**
     * @brief Gets step of randomizator.
     */
    virtual remove_add_edges_pair get_step() = 0;

public:
    /**
     * @brief Constructor
     */
    randomizator_base(graph_types::undirected_graph& graph)
        : m_graph(graph)
    {}

protected:
    graph_types::undirected_graph& m_graph;
};
