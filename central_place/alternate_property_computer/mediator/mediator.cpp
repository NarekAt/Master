/**
 * @file mediator/mediator.cpp
 * @description definition of class mediator.
 */

#include "mediator.h"
#include "erdos_renyi_reader.h"
#include "single_process_task_manager.h"
#include "main_process_task_manager.h"
#include "secondary_process_task_manager.h"
#include <assert.h>

mediator* mediator::s_instance = nullptr;

void mediator::init(const arg_name_to_value_map& a_n_v)
{
    assert(!m_inited);
    m_inited = true;
    // if argument list is empty
    // it means that this is secondary process.
    if (a_n_v.empty()) {
        return;
    }
    try {
        erdos_renyi_reader r;
        auto mu_it = a_n_v.find("mu_file");
        assert(a_n_v.end() != mu_it);
        r.get_mus_from_file(
            boost::any_cast<std::string>(mu_it->second), m_mu_list);
        auto gr_it = a_n_v.find("graph_file");
        assert(a_n_v.end() != gr_it);
        r.get_graph_and_properties_from_file(
            boost::any_cast<std::string>(gr_it->second),
            m_graph, m_vertex_count, m_probability);
        auto sc_it = a_n_v.find("step_count");
        assert(a_n_v.end() != sc_it);
        m_step_count = boost::any_cast<unsigned>(sc_it->second);
        auto rt_it = a_n_v.find("randomization_type");
        assert(a_n_v.end() != rt_it);
        m_randomization_type = boost::any_cast<randomization_type>(
            rt_it->second);
        assert(INVALID_RT != m_randomization_type);
        auto apt_it = a_n_v.find("alternate_property_type");
        assert(a_n_v.end() != apt_it);
        m_alternate_property_type = boost::any_cast<
            alternate_property_type>(apt_it->second);
        assert(INVALID_APT != m_alternate_property_type);
    } catch (const boost::bad_any_cast&) {
        assert(!"bad any cast");
    }
}

void mediator::run(boost::mpi::communicator& world)
{
    assert(m_inited);
    if (1 == world.size()) {
        single_process_task_manager t_m(world);
        run_task_manager_and_send_to_output(t_m);
    } else {
        if (0 == world.rank()) {
            main_process_task_manager t_m(world);
            run_task_manager_and_send_to_output(t_m);
        } else {
            secondary_process_task_manager t_m(world);
            t_m.run();
        }
    }
}

void mediator::run_task_manager_and_send_to_output(
    task_manager_base& t_m)
{
    t_m.init(m_graph, m_mu_list, m_step_count,
        m_randomization_type, m_alternate_property_type);
    t_m.run();
    // TODO: Send to output results t_m.get_results()
}

mediator& mediator::get_instance()
{
    assert(s_instance != nullptr);
    return *s_instance;
}

void mediator::instantiate()
{
    assert(s_instance == nullptr);
    s_instance = new mediator();
}

void mediator::destroy()
{
    assert(s_instance != nullptr);
    delete s_instance;
    s_instance = nullptr;
}

mediator::mediator()
    : m_inited(false)
{
}
