/**
 * @file randomizers/randomizator_base.cpp
 * @brief definition of class randomizator_base
 */

#include "randomizator_base.h"

randomizator_base::randomizator_base(
    graph_types::undirected_graph& graph)
    : m_random_generator(time(0)),
    m_graph(graph)
{}
