/**
 * @file task_managers/task_manager_base.h
 * @description declaration of task_manager_base class.
 */

#pragma once

#include "types.h"
#include <boost/mpi.hpp>

class randomizator_base;
class property_counter_base;

/**
 * @class task_manager_base
 * @brief base class for task managers.
 */
class task_manager_base
{
public:
    /**
     * @brief Inits task manager.
     * @param g Undirected graph.
     * @param m Mu list
     * @param s_c Step count
     * @param r Randomization type
     * @param p Alternate property type.
     */
    void init(const graph_types::undirected_graph& g,
        const mu_list& m, unsigned s_c, const randomization_type r,
        const alternate_property_type p);

public:
    /**
     * @brief Runs task manager
     */
    virtual void run() = 0;

public:
    /**
     * @brief Gets results.
     */
    const calculation_results& get_results() const;

protected:
    bool check_to_assume_step(int delta, double mu) const;

protected:
    boost::mpi::communicator& m_world;
    graph_types::undirected_graph m_graph;
    mu_list m_mu_list;
    unsigned m_step_count;
    randomizator_base* m_randomizator;
    property_counter_base* m_counter;
    unsigned m_current_property_count;
    calculation_results m_results;

public:
    /**
     * @brief Constructor
     * @param world World of mpi
     */
    task_manager_base(boost::mpi::communicator& world);
};
