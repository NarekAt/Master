#ifndef _BASE_TASK_H_
#define _BASE_TASK_H_

#include "defs.h"
#include <fstream>
#include <random>
#include <thread>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace graph_randomization
{

class base_task
{
public:
  base_task(const undirected_graph& graph, double mu, int step_count);
  virtual void perform_randomization();
  const std::vector<std::pair<int, int>>& results() const;

private:
  void load_graph_from_file(const std::string& file_name);

protected:
  virtual void make_randomization_step() = 0;
  virtual bool is_stabilized();
  virtual bool check_step(int delta);
  unsigned count_triangles();

protected:
  undirected_graph graph_;
  unsigned num_triangles_;
  random_generator rand_generator_;
  unsigned num_vertices_;
  double mu_;
  double probability_;
  std::vector<std::pair<int, int>> results_;
  int step_count_;
};

} // end namespace graph_randomization

#endif // _BASE_TASK_H_
