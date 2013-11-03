/**
 * @file task_managers/single_process_task_manager.cpp
 * @brief definition of single_process_task_manager class.
 */

#include "single_process_task_manager.h"
#include "randomizator_factory.h"
#include <iostream>

void single_process_task_manager::run()
{
    assert(m_inited);
    for (auto& mu : m_mu_list) {
        single_results_list c_r;
        calculate_for_single_mu_by_pass_count(c_r, mu);
        // TODO: change cout to log.
        std::cout << "\nCalculation for mu: " << mu <<
            " finished.\n";
        m_results.push_back(std::make_pair(mu, c_r));
    }
}

single_process_task_manager::single_process_task_manager(
    boost::mpi::communicator& world) :
    task_manager_base(world)
{}
