/**
 * @file task_managers/main_process_task_manager.cpp
 * @description definition of main_process_task_manager class.
 */

#include "main_process_task_manager.h"
#include "randomizator_base.h"
#include "property_counter_base.h"

void main_process_task_manager::run()
{
    assert(m_inited);
}

void main_process_task_manager::send_ingredients(
    unsigned p_id, const mu_list& mus) const
{
    return;
}

main_process_task_manager::main_process_task_manager(
    boost::mpi::communicator& world) :
    task_manager_base(world)
{}
