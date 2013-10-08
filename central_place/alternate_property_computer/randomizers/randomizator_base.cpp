/**
 * @file randomizators/randomizator_base.cpp
 * @description definition of class randomizator_base
 */

#include "randomizator_base.h"

randomizator_base::randomizator_base(
    graph_types::undirected_graph& graph)
    : m_random_generator(time(NULL)),
    m_graph(graph)
{}
