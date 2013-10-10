/**
 * @file task_managers/task_manager_base.cpp
 * @description definition of task_manager_base class.
 */

#include "task_manager_base.h"
#include "randomizator_factory.h"
#include "property_counter_factory.h"

void task_manager_base::init(const graph_types::undirected_graph& g,
    const mu_list& m, unsigned s_c, const randomization_type r,
    const alternate_property_type p)
{
    m_graph = g;
    m_mu_list = m;
    m_step_count = s_c;
    m_randomizator = randomizator_factory::get_randomizator(
        m_graph, r);
    m_counter = property_counter_factory::get_counter(
        m_graph, p);
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

task_manager_base::task_manager_base(boost::mpi::communicator& world)
    : m_world(world)
{}
