/**
 * @file task_managers/main_process_task_manager.h
 * @brief declaration of main_process_task_manager class.
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
     * @brief Sends needed ingredients to secondary processes.
     */
    void send_ingredients_to_precesses() const;

private:
    /**
     * @brief Sends needed ingredients to the secondary process.
     * @param p_id Id of process to send ingredients.
     * @param mus Mu list for which secondary process must made
     *        calculations.
     */
    void send_ingredients(unsigned p_id, 
        const tag_to_mu_map& mus) const;

private:
    /**
     * @brief Colculate mu count per process.
     */
    void colculate_process_to_mu_count();

private:
    /**
     * @brief Recieves results from secondary processes.
     */
    void revieve_results_from_processes();

private:
    process_to_mu_count m_process_to_mu_count;

public:
    /**
     * @brief Constructor
     * @param world World of mpi
     */
    main_process_task_manager(boost::mpi::communicator& world);
};
