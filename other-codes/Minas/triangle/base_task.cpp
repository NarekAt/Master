#include "base_task.h"

namespace graph_randomization
{

base_task::base_task(const undirected_graph& graph, double mu, int step_count)
  : graph_(graph)
  , rand_generator_(time(NULL))
  , mu_(mu)
  , step_count_(step_count)
{
  num_triangles_ = count_triangles();
}

inline void base_task::load_graph_from_file(const std::string& file_name)
{
  std::ifstream file;
  file.open(file_name);
  if(false == file.is_open())
  {
    std::cerr << "Failed to open file containing graph." << std::endl;
    return;
  }
  boost::archive::text_iarchive ia(file);
  ia >> num_vertices_ >> probability_;
  boost::serialization::load(ia, graph_, 0);
}

const std::vector<std::pair<int, int>>& base_task::results() const
{
  return results_;
}

unsigned base_task::count_triangles()
{
  unsigned num = 0;
  edge_iterator ei, ei_end;
  // iterate over edges
  for(tie(ei, ei_end) = boost::edges(graph_); ei != ei_end; ++ei)
  {
    // get edge vertices
    vertex vs = boost::source(*ei, graph_);
    vertex vt = boost::target(*ei, graph_);
    // get adjacent vertices for them
    adjacency_iterator vsi, vsi_end, vti, vti_end;
    for(tie(vsi, vsi_end) = boost::adjacent_vertices(vs, graph_); vsi != vsi_end; ++vsi)
    {
      for(tie(vti, vti_end) = boost::adjacent_vertices(vt, graph_); vti != vti_end; ++vti)
      {
        if(*vsi == *vti)
        {
          ++num;
        }
      }
    }
  }
  // every triangle is counted triple
  num = num / 3;
  return num;
}

void base_task::perform_randomization()
{
  results_.push_back(std::make_pair(0, num_triangles_));
  int step = 1;
  while(step <= step_count_ && false == is_stabilized())
  {
    make_randomization_step();
    results_.push_back(std::make_pair(step, num_triangles_));
    ++step;
  }
}

void base_task::make_randomization_step()
{
  //throw new not_implemented_exception(__FUNCTION__" is not implemented.");
  std::cout << "NOT IMPLEMENTED" << std::endl;
}

bool base_task::is_stabilized()
{
  return false;
}

bool base_task::check_step(int delta)
{
  if(delta <= 0)
  {
    std::array<long double, 2> probabilities;
    probabilities[0] = exp(mu_ * delta);
    if(true == std::isinf(probabilities[0]))
    {
      probabilities[0] = 0;
    }
    probabilities[1] = 1.e0 - probabilities[0];
    std::discrete_distribution<int> distribution(probabilities.begin(), probabilities.end());
    if(1 == distribution(rand_generator_))
    {
      return false;
    }
  }
  return true;
}

} // end namespace graph_randomization
