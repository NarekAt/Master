/**
 * @file mediator/mediator.h
 * @description declaration of class mediator.
 */

#pragma once

#include <boost/mpi.hpp>

/**
 * @class mediator
 * @brief this class is used for managing interaction
 *        between input, task_manager and output interfaces.
 * @note Singleton class.
 */
class mediator
{
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
    mediator() = default;

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
