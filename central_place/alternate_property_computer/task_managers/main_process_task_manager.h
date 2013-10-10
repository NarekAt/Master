/**
 * @file task_managers/main_process_task_manager.h
 * @description declaration of main_process_task_manager class.
 */

#pragma once

#include "task_manager_base.h"

/**
 * @class main_process_task_manager
 * @brief main process task manager
 *        rules secondary processes.
 */
class main_process_task_manager : public task_manager_base
{
public:
    /**
     * @brief Runs task manager
     */
    virtual void run();

private:
    /**
     * @brief Sends needed ingredients to the secondary process.
     * @param p_id Id of process to send ingredients.
     * @param mus Mu list for which secondary process must made
     *        calculations.
     */
    void send_ingredients(unsigned p_id, const mu_list& mus) const;

public:
    /**
     * @brief Constructor
     * @param world World of mpi
     */
    main_process_task_manager(boost::mpi::communicator& world);
};
