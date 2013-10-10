/**
 * @file task_managers/single_process_task_manager.cpp
 * @description definition of single_process_task_manager class.
 */

#include "single_process_task_manager.h"
#include "randomizator_base.h"
#include "property_counter_base.h"

void single_process_task_manager::run()
{
    assert(m_inited);
    assert(nullptr != m_randomizator);
    for (auto& mu : m_mu_list) {
        single_results_list c_r;
        for (int p_c = 0; p_c < m_pass_count; ++p_c) { 
            m_current_property_count = m_counter->
                compute_initial_count();
            if (0 == p_c) {
                c_r.push_back(std::make_pair(0, 
                    static_cast<double>(m_current_property_count)));
            }
            calculate_for_single_mu(c_r, mu, 0 == p_c);
        }
        for (auto r : c_r) {
            r.second /= m_pass_count;
        }
        m_results.push_back(std::make_pair(mu, c_r));
    }
}

single_process_task_manager::single_process_task_manager(
    boost::mpi::communicator& world) :
    task_manager_base(world)
{}
