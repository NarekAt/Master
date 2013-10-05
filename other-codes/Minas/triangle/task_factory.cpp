#include "task_factory.h"


namespace graph_randomization
{

base_task* get_task(const undirected_graph& graph, double mu, int step_count, const std::string& type)
{
  if(std::string("random_switch") == type)
  {
    return new random_switch_task(graph, mu, step_count);
  }
  else if(std::string("fixed_degree") == type)
  {
    return new fixed_degree_task(graph, mu, step_count);
  }
  else
  {
    std::cerr << "Warning: returning null task." << std::endl;
    return 0;
  }
}

}
