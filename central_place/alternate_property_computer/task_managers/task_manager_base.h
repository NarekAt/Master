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
    void calculate_for_single_mu(single_results_list& c_r,
        double mu, bool is_first_pass);
protected:
    enum mpi_tag {
        PROCESS_IS_NEEDED = 1,
        GRAPH = 2,
        STEP_COUNT = 3,
        PASS_COUNT = 4,
        RANDOMIZATION_TYPE = 5,
        ALTERNATE_PROPERTY_TYPE = 6,
        MUS = 7,
        MU_START = 100
    };

protected:
    bool m_inited;
    boost::mpi::communicator& m_world;
    graph_types::undirected_graph m_initial_graph;
    graph_types::undirected_graph m_current_graph;
    mu_list m_mu_list;
    unsigned m_step_count;
    // TODO: pass count must be got from program arguments.
    //       for now it will just set to 10.
    unsigned m_pass_count; 
    randomization_type m_randomizator_type;
    randomizator_base* m_randomizator;
    alternate_property_type m_alternate_property_type;
    property_counter_base* m_counter;
    unsigned m_current_property_count;
    calculation_results m_results;

public:
    /**
     * @brief Constructor
     * @param world World of mpi
     */
    task_manager_base(boost::mpi::communicator& world);

public:
    /**
     * @brief Destructor
     */
    virtual ~task_manager_base();
};
