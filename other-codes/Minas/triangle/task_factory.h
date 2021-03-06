#ifndef _TASK_FACTORY_H_
#define _TASK_FACTORY_H_

#include "base_task.h"
#include "fixed_degree_task.h"
#include "random_switch_task.h"

namespace graph_randomization
{

base_task* get_task(const undirected_graph& graph, double mu, int step_count, const std::string& type);

}

#endif //_TASK_FACTORY_H_
