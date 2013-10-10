/**
 * @file task_managers/single_process_task_manager.h
 * @description declaration of single_process_task_manager class.
 */

#pragma once

#include "task_manager_base.h"

/**
 * @class single_process_task_manager
 * @brief single process task manager
 */
class single_process_task_manager : public task_manager_base
{
public:
    /**
     * @brief Runs task manager
     */
    virtual void run();

public:
    /**
     * @brief Constructor
     * @param world World of mpi
     */
    single_process_task_manager(boost::mpi::communicator& world);
};
