/**
 * @file property_counters/property_counter_base.h
 * @brief delcaration of property_counter_base class.
 */

#pragma once

#include "types.h"
#include <set>

/**
 * @class property_counter_base
 * @brief This class is needed for basic functionality
 *        of counting different properties of graph.
 */
class property_counter_base
{
public:
    /**
     * @brief Computes initial count of property of graph.
     */
    virtual unsigned compute_initial_count() const = 0;

public:
    /**
     * @brief Gets property decrease value after removing edge.
     * @param e Edges to remove.
     */
    virtual unsigned compute_decrease_after_remove(
        const graph_types::null_edges& e) const = 0;

public:
    /**
     * @brief Gets property increase value after adding edge.
     * @param e Edges to add.
     */
    virtual unsigned compute_increase_after_add(
        const graph_types::null_edges& e) const = 0;

public:
    /**
     * @brief Gets property counter type
     */
    virtual alternate_property_type get_type() const = 0;

protected:
    graph_types::undirected_graph& m_graph;

public:
    /**
     * @brief Constructor
     * @param graph Graph for counting property.
     */
    property_counter_base(graph_types::undirected_graph& graph)
        : m_graph(graph)
    {}
};
