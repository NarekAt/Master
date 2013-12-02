/**
 * @file task_managers/main_process_task_manager.cpp
 * @brief definition of main_process_task_manager class.
 */

#include "main_process_task_manager.h"
#include "randomizator_base.h"
#include "property_counter_base.h"
#include "property_counter_factory.h"
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <iostream>

void main_process_task_manager::run()
{
    assert(m_inited);
    colculate_process_to_mu_count();
    send_ingredients_to_precesses();
    receive_results_from_processes();
}

void main_process_task_manager::treat_status_information(const persent_to_mu& info)
{
    // Means That whole calculation was done for came mu.
    if (0 == info.first) {
        std::cout << "*** Calculation was done for mu: " << info.second
            << " ***\n";
        return;
    }
    m_counter = property_counter_factory::get_counter(
        m_initial_graph, m_alternate_property_type);
    std::string p_t_n = get_alternate_property_name_by_type(
        m_counter->get_type());
    std::string info_message = std::string("calculation by ") + p_t_n + 
        std::string(" was done by ") + std::to_string(info.first) + 
        std::string("% for mu ") + std::to_string(info.second);
    std::cout << info_message << std::endl;
    delete m_counter;
    m_counter = nullptr;
}

void main_process_task_manager::send_ingredients_to_precesses()
{
    unsigned next_tag = MU_START;
    unsigned next_status_tag = STATUS_START;
    unsigned mu_i = 0;
    for (auto& p_to_c : m_process_to_mu_count) {
        tag_to_mu_map c_t_m;
        status_tags s_t;
        int s_per_process = p_to_c.second*m_pass_count*(m_step_count / s_status_step) +
            p_to_c.second;
        for (int i = 0; i < s_per_process; ++i) {
            s_t.push_back(next_status_tag++);
        }
        m_process_to_status_tags.insert(std::make_pair(p_to_c.first, s_t));
        if (0 != p_to_c.second) {
            int e = mu_i + p_to_c.second;
            assert(m_mu_list.size() >= e);
            while (mu_i < e) {
                c_t_m.push_back(std::make_pair(next_tag,
                    m_mu_list[mu_i]));
                ++next_tag;
                ++mu_i;
            }
        }
        send_ingredients(p_to_c.first, c_t_m, s_t);
    }
    assert(m_mu_list.size() == mu_i);
}

void main_process_task_manager::send_ingredients(
    unsigned p_id, const tag_to_mu_map& mus, const status_tags& s_t) const
{
    assert(0 < p_id);
    if (mus.empty()) {
        m_world.send(p_id, mpi_tag::PROCESS_IS_NEEDED, false);
    }
    m_world.send(p_id, PROCESS_IS_NEEDED, true);
    m_world.send(p_id, GRAPH, m_initial_graph);
    m_world.send(p_id, ALTERNATE_PROPERTY_INITIAL_COUNT,
        m_initial_property_count);
    m_world.send(p_id, STEP_COUNT, m_step_count);
    m_world.send(p_id, PASS_COUNT, m_pass_count);
    m_world.send(p_id, RANDOMIZATION_TYPE,
        static_cast<int>(m_randomizator_type));
    m_world.send(p_id, ALTERNATE_PROPERTY_TYPE,
        static_cast<int>(m_alternate_property_type));
    m_world.send(p_id, MUS, mus);
    m_world.send(p_id, STATUS_TAGS, s_t);
}

void main_process_task_manager::colculate_process_to_mu_count()
{
    unsigned mu_per_process = m_mu_list.size() / (m_world.size() - 1);
    if (0 == mu_per_process) {
        int c_p = 1;
        while (c_p <= m_mu_list.size()) {
            m_process_to_mu_count.push_back(std::make_pair(
                c_p, 1));
            ++c_p;
        }
        while (c_p < m_world.size()) {
            m_process_to_mu_count.push_back(std::make_pair(
                c_p, 0));
            ++c_p;
        }
    } else {
        unsigned last_mus_count = m_mu_list.size() %
            (m_world.size() - 1);
        int c_p = 1;
        while (c_p < m_world.size()) {
            m_process_to_mu_count.push_back(std::make_pair(
                c_p, mu_per_process));
            ++c_p;
        }
        for (int i = 0; i < last_mus_count; ++i) {
            ++m_process_to_mu_count[i].second;
        }
    }
}

namespace {

void process_requests_of_results(
    std::vector<std::pair<boost::mpi::request, double>>& r_requests)
{
    while (!r_requests.empty()) {
        for (int i = r_requests.size() - 1; i >= 0; --i) {
            if (false == !r_requests[i].first.test()) {
                r_requests.erase(r_requests.begin() + i);
            }
        }
        usleep(100);
    }
}

}

void main_process_task_manager::process_requests_of_statuses()
{
    unsigned statuses_count = m_mu_list.size() *
        m_pass_count * (m_step_count / s_status_step) + m_mu_list.size();
    std::vector<std::pair<unsigned, unsigned>> tag_sequent(statuses_count);
    unsigned i = 0;
    while (i < statuses_count) {
        for (int p = 1; p < m_world.size(); ++p) {
            auto it = m_process_to_status_tags.find(p);
            assert(m_process_to_status_tags.end() != it);
            auto& tags = it->second;
            if (!tags.empty()) {
                tag_sequent[i].first = p;
                tag_sequent[i].second = tags.front();
                ++i;
                tags.erase(tags.begin());
            }
        }
    }
    m_process_to_status_tags.clear();
    for (const auto& t : tag_sequent) {
        persent_to_mu upcoming_info;
        m_world.recv(t.first, t.second, upcoming_info);
        treat_status_information(upcoming_info);
    }
}

void main_process_task_manager::receive_results_from_processes()
{
    m_results.reserve(m_mu_list.size());
    std::vector<std::pair<boost::mpi::request, double>> r_requests;
    unsigned next_tag = MU_START;
    unsigned mu_i = 0;
    for (auto& p_to_c : m_process_to_mu_count) {
        if (0 != p_to_c.second) {
            int e = mu_i + p_to_c.second;
            assert(m_mu_list.size() >= e);
            while (mu_i < e) {
                m_results.push_back(std::make_pair(
                    m_mu_list[mu_i], single_results_list()));
                r_requests.push_back(std::make_pair(
                    boost::mpi::request(), m_mu_list[mu_i]));
                r_requests.back().first = m_world.irecv(
                    p_to_c.first, next_tag, m_results.back()); 
                ++next_tag;
                ++mu_i;
            }
        }
    }
    process_requests_of_statuses();
    process_requests_of_results(r_requests);
}

main_process_task_manager::main_process_task_manager(
    boost::mpi::communicator& world) :
    task_manager_base(world)
{}
