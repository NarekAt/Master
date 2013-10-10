/**
 * @file task_managers/task_manager_base.cpp
 * @description definition of task_manager_base class.
 */

#include "task_manager_base.h"
#include "randomizator_factory.h"
#include "property_counter_factory.h"
#include <assert.h>

void task_manager_base::init(const graph_types::undirected_graph& g,
    const mu_list& m, unsigned s_c, const randomization_type r,
    const alternate_property_type p)
{
    assert(!m_inited);
    m_inited = true;
    m_initial_graph = g;
    m_mu_list = m;
    m_step_count = s_c;
    m_randomizator_type = r;
    m_alternate_property_type = p;
    //m_randomizator = randomizator_factory::get_randomizator(
    //    m_current_graph, r);
    //m_counter = property_counter_factory::get_counter(
    //    m_current_graph, p);
}

const calculation_results& task_manager_base::get_results() const
{
    return m_results;
}

bool task_manager_base::check_to_assume_step(int delta
    , double mu) const
{
    if(0 >= delta) {
        std::array<long double, 2> probabilities;
        probabilities[0] = exp(mu * delta);
        if(std::isinf(probabilities[0])) {
            probabilities[0] = 0;
        }
        probabilities[1] = 1.e0 - probabilities[0];
        std::discrete_distribution<int> distribution(
            probabilities.begin(), probabilities.end());
        graph_types::random_generator r_g(time(0));
        if(1 == distribution(r_g)) {
            return false;
        }
    }
    return true;
}

void task_manager_base::calculate_for_single_mu(
    single_results_list& c_r, double mu, bool is_first_pass)
{
    assert(nullptr != m_counter);
    assert(nullptr != m_randomizator);
    for (int s_i = 1; s_i <= m_step_count; ++s_i) {
        auto step = m_randomizator->get_step();
        for (auto& e : step.first) {
            boost::remove_edge(e.first, e.second, m_current_graph);
        }
        unsigned delta = 
            m_counter->compute_increase_after_add(step.second)
            - m_counter->compute_decrease_after_remove(step.first);
        if (check_to_assume_step(delta, mu)) {
            m_current_property_count += delta;
            for (auto& e : step.second) {
                boost::add_edge(e.first, e.second, m_current_graph);
            }
        } else {
            for (auto& e : step.first) {
                boost::add_edge(e.first, e.second, m_current_graph);
            }
        }
        if (is_first_pass) {
            c_r.push_back(std::make_pair(s_i, 
                static_cast<double>(m_current_property_count)));
        } else {
            c_r[s_i].second += 
                static_cast<double>(m_current_property_count);
        }
    }
}

task_manager_base::task_manager_base(boost::mpi::communicator& world)
    : m_inited(false), m_world(world), m_pass_count(10),
    m_randomizator(nullptr), m_counter(nullptr)
{}

task_manager_base::~task_manager_base()
{
    assert(nullptr == m_randomizator);
    assert(nullptr == m_counter);
}
