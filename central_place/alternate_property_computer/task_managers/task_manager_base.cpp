/**
 * @file task_managers/task_manager_base.cpp
 * @brief definition of task_manager_base class.
 */

#include "task_manager_base.h"
#include "randomizator_factory.h"
#include "property_counter_factory.h"
#include <system_error>
#include <assert.h>
#include <iostream>
#include <time.h>

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
    calculate_initial_alternate_property_count();
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

void task_manager_base::calculate_for_single_mu_by_pass_count(
    single_results_list& c_r, double mu)
{
    for (int p_c = 0; p_c < m_pass_count; ++p_c) { 
        m_current_graph = m_initial_graph;
        m_current_non_existing_edges = m_initial_non_existing_edges;
        assert(nullptr == m_randomizator);
        assert(nullptr == m_counter);
        m_randomizator = randomizator_factory::get_randomizator(
            m_current_graph, m_current_non_existing_edges, m_randomizator_type);
        m_counter = property_counter_factory::get_counter(
            m_current_graph, m_alternate_property_type);
        m_current_property_count = m_initial_property_count; 
        if (0 == p_c) {
            c_r.push_back(std::make_pair(0, 
                static_cast<double>(m_current_property_count)));
        }
        calculate_for_single_mu(c_r, mu, 0 == p_c);
        delete m_randomizator;
        m_randomizator = nullptr;
        delete m_counter;
        m_counter = nullptr;
    }
    for (auto r : c_r) {
        r.second /= m_pass_count;
    }
}

void task_manager_base::calculate_for_single_mu(
    single_results_list& c_r, double mu, bool is_first_pass)
{
    assert(nullptr != m_counter);
    assert(nullptr != m_randomizator);
    for (int s_i = 1; s_i <= m_step_count; ++s_i) {
        auto step = m_randomizator->get_step();
        unsigned d = m_counter->compute_decrease_after_remove(
            step.first);
        // edges must be added befor calling function:
        // compute_increase_after_add(<added_edges>).
        for (auto& e : step.second) {
            boost::add_edge(e.first, e.second, m_current_graph);
        }
        unsigned i =
            m_counter->compute_increase_after_add(step.second);
        unsigned delta = i - d;
        if (check_to_assume_step(delta, mu)) {
            m_current_property_count += delta;
            for (auto& e : step.second) {
                boost::add_edge(e.first, e.second, m_current_graph);
            }
            for (auto& e : step.first) {
                m_current_non_existing_edges.push_back(e);
            }
        } else {
            for (auto& e : step.first) {
                boost::add_edge(e.first, e.second, m_current_graph);
            }
            for (auto& e : step.second) {
                m_current_non_existing_edges.push_back(e);
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

void task_manager_base::calculate_initial_non_existing_edges()
{
    graph_types::vertex_iterator v, v_end;
    boost::tie(v, v_end) = boost::vertices(m_initial_graph);
    for(; v != v_end; ++v)
    {
        graph_types::vertex_iterator v1 = v;
        for(++v1; v1 != v_end; ++v1)
        {
            if(false == boost::edge(*v, *v1, m_initial_graph).second)
            {
                m_initial_non_existing_edges.push_back(std::make_pair(*v, *v1));
            }
        }
    }
    assert(!m_initial_non_existing_edges.empty());
}

void task_manager_base::calculate_initial_alternate_property_count()
{
    assert(nullptr == m_counter);
    clock_t t = clock();
    m_counter = property_counter_factory::get_counter(
        m_initial_graph, m_alternate_property_type);
    m_initial_property_count = m_counter->compute_initial_count();
    t = clock() - t;
    // TODO: change cout to log.
    std::cout << "\nGraph initial " << 
        get_alternate_property_name_by_type(m_alternate_property_type) <<
        " computed: " << m_initial_property_count << 
        " time: " << static_cast<float>(t)/CLOCKS_PER_SEC << std::endl;
    delete m_counter;
    m_counter = nullptr;
}

task_manager_base::task_manager_base(boost::mpi::communicator& world)
    : m_inited(false), m_world(world), m_pass_count(1),
    m_randomizator(nullptr), m_counter(nullptr)
{}

task_manager_base::~task_manager_base()
{
    assert(nullptr == m_randomizator);
    assert(nullptr == m_counter);
}
