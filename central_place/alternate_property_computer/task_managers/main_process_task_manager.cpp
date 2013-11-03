/**
 * @file task_managers/main_process_task_manager.cpp
 * @brief definition of main_process_task_manager class.
 */

#include "main_process_task_manager.h"
#include "randomizator_base.h"
#include "property_counter_base.h"
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <iostream>

void main_process_task_manager::run()
{
    assert(m_inited);
    colculate_process_to_mu_count();
    send_ingredients_to_precesses();
    revieve_results_from_processes();
}

void main_process_task_manager::send_ingredients_to_precesses() const
{
    unsigned next_tag = MU_START;
    unsigned mu_i = 0;
    for (auto& p_to_c : m_process_to_mu_count) {
        tag_to_mu_map c_t_m;
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
        send_ingredients(p_to_c.first, c_t_m);
    }
    assert(m_mu_list.size() == mu_i);
}

void main_process_task_manager::send_ingredients(
    unsigned p_id, const tag_to_mu_map& mus) const
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

void main_process_task_manager::revieve_results_from_processes()
{
    m_results.reserve(m_mu_list.size());
    std::vector<std::pair<boost::mpi::request, double>> requests;
    unsigned next_tag = MU_START;
    unsigned mu_i = 0;
    for (auto& p_to_c : m_process_to_mu_count) {
        if (0 != p_to_c.second) {
            int e = mu_i + p_to_c.second;
            assert(m_mu_list.size() >= e);
            while (mu_i < e) {
                m_results.push_back(std::make_pair(
                    m_mu_list[mu_i], single_results_list()));
                requests.push_back(std::make_pair(
                    boost::mpi::request(), m_mu_list[mu_i]));
                requests.back().first = m_world.irecv(
                    p_to_c.first, next_tag, m_results.back()); 
                ++next_tag;
                ++mu_i;
            }
        }
    }
    while (!requests.empty()) {
        for (int i = requests.size() - 1; i >= 0; --i) {
            if (false == !requests[i].first.test()) {
                // TODO: change cout to log.
                std::cout << "\nCalculation for mu: " <<
                    requests[i].second << " finished.\n";
                requests.erase(requests.begin() + i);
            }
        }
        usleep(100);
    }
}

main_process_task_manager::main_process_task_manager(
    boost::mpi::communicator& world) :
    task_manager_base(world)
{}
