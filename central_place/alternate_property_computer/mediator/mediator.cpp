/**
 * @file mediator/mediator.cpp
 * @description definition of class mediator.
 */

#include "mediator.h"
#include "erdos_renyi_reader.h"
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
    // TODO: fill body.
}

void mediator::run(const boost::mpi::communicator& world)
{
    assert(m_inited);
    // TODO: fill body.
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
