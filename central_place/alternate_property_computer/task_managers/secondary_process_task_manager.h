/**
 * @file task_managers/secondary_process_task_manager.h
 * @description declaration of secondary_process_task_manager class.
 */

#pragma once

#include "task_manager_base.h"

/**
 * @class secondary_process_task_manager
 * @brief secondary process task manager 
 *        main process's helper process.
 */
class secondary_process_task_manager : public task_manager_base
{
public:
    /**
     * @brief Runs task manager
     */
    virtual void run();

private:
    /**
     * @brief receives needed ingredients from main process.
     * @return false if main process don't need this process.
     */
    bool receive_ingredients();

private:
    tag_to_mu_map m_taged_mus;

public:
    /**
     * @brief Constructor
     * @param world World of mpi
     */
    secondary_process_task_manager(boost::mpi::communicator& world);
};
