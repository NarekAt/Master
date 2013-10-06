/**
 * @file mediator/mediator.h
 * @description declaration of class mediator.
 */

#pragma once

#include "types.h"
#include <boost/mpi.hpp>

/**
 * @class mediator
 * @brief this class is used for managing interaction
 *        between input, task_manager and output interfaces.
 * @note Singleton class.
 */
class mediator
{
    /// @name mediator base public interface
    /// @{
public:
    /**
     * @brief Inits mediator
     * @param a_n_v process arguments map.
     * @note must be called before run() function, and only one time.
     */
    void init(const arg_name_to_value_map& a_n_v);

public:
    /**
     * @brief Runs mediator
     * @param world mpi communicator.
     * @note Before call of this function init() must be called.
     */
    void run(const boost::mpi::communicator& world);

private:
    bool m_inited;
    graph_types::undirected_graph m_graph;
    int m_vertex_count;
    double m_probability;
    mu_list m_mu_list;
    unsigned m_step_count;
    randomization_type m_randomization_type;
    alternate_property_type m_alternate_property_type;
    /// @}

    /// @name singleton management
    /// @{
public:
    /**
     * @brief Gets singletone object.
     */
    static mediator& get_instance();

public:
    /**
     * @brief Intstantiates singletone object.
     */
    static void instantiate();

public:
    /**
     * @brief Destroys singletone object.
     */
    static void destroy();

private:
    static mediator* s_instance;
    /// @}

    /// @name Special member functions.
    /// @{
public:
    /**
     * @brief Constructor
     */
    mediator();

public:
    /**
     * @brief Destructor.
     */
    ~mediator() = default;
private:
    mediator(const mediator&) = delete;
    mediator& operator=(const mediator&) = delete;
    /// @}
};
