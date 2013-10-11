/**
 * @file task_managers/secondary_process_task_manager.cpp
 * @description definition of secondary_process_task_manager class.
 */

#include "secondary_process_task_manager.h"
#include "randomizator_base.h"
#include "property_counter_base.h"
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/utility.hpp>

void secondary_process_task_manager::run()
{
    // TODO: fill body.
}

bool secondary_process_task_manager::receive_ingredients()
{
    bool process_is_needed = false;
    m_world.recv(0, mpi_tag::PROCESS_IS_NEEDED, process_is_needed);
    if (!process_is_needed) {
        return false;
    }
    m_world.recv(0, GRAPH, m_initial_graph);
    m_world.recv(0, STEP_COUNT, m_step_count);
    m_world.recv(0, PASS_COUNT, m_pass_count);
    int r_type = 0;
    m_world.recv(0, RANDOMIZATION_TYPE, r_type);
    m_randomizator_type = static_cast<randomization_type>(r_type);
    int ap_type = 0;
    m_world.recv(0, ALTERNATE_PROPERTY_TYPE, ap_type);
    m_alternate_property_type = static_cast<alternate_property_type>(
        ap_type);
    m_world.recv(0, MUS, m_taged_mus);
    return true;
}

secondary_process_task_manager::secondary_process_task_manager(
    boost::mpi::communicator& world) :
    task_manager_base(world)
{}
