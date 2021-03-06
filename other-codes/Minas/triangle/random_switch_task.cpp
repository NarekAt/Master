#include "random_switch_task.h"
#include <system_error>

namespace graph_randomization
{

random_switch_task::random_switch_task(const undirected_graph& graph, double mu, int step_count)
  : base_task(graph, mu, step_count)
{
  int vertices_count = boost::num_vertices(graph_);
  int all_edges_count = vertices_count * (vertices_count - 1) / 2;
  int non_existing_edges_count = all_edges_count - boost::num_edges(graph_);
  non_existing_edges_.reserve(non_existing_edges_count);
  vertex_iterator v, v_end;
  boost::tie(v, v_end) = boost::vertices(graph_);
  int index = 0;
  for(; v != v_end; ++v)
  {
    vertex_iterator v1 = v;
    for(++v1; v1 != v_end; ++v1)
    {
      if(false == boost::edge(*v, *v1, graph_).second)
      {
        non_existing_edges_.push_back(std::make_pair(*v, *v1));
      }
    }
  }
  std::uniform_int_distribution<> distrib(0, non_existing_edges_.size() - 1);
  var_generator_ = new boost::variate_generator<random_generator, std::uniform_int_distribution<>>(rand_generator_, distrib);
}

random_switch_task::~random_switch_task()
{
  delete var_generator_;
}

void random_switch_task::make_randomization_step()
{
  unsigned removed = 0;
  unsigned added = 0;

  edge edge_to_remove;
  vertex source1, target1;
  std::thread* thread1 = 0;
  std::thread* thread2 = 0;
  try
  {
    thread1 = new std::thread([&]()
    {
      edge_to_remove = boost::random_edge(graph_, rand_generator_);
      source1 = boost::source(edge_to_remove, graph_);
      target1 = boost::target(edge_to_remove, graph_);
      adjacency_iterator v, v_end;
      for(boost::tie(v, v_end) = boost::adjacent_vertices(source1, graph_); v != v_end; ++v)
      {
        if(*v != target1 && true == boost::edge(*v, target1, graph_).second)
        {
          ++removed;
        }
      }
    });
  }
  catch(std::system_error&)
  {
    //std::cout << "No resources to create new thread #1" << std::endl;
    edge_to_remove = boost::random_edge(graph_, rand_generator_);
    source1 = boost::source(edge_to_remove, graph_);
    target1 = boost::target(edge_to_remove, graph_);
    adjacency_iterator v, v_end;
    for(boost::tie(v, v_end) = boost::adjacent_vertices(source1, graph_); v != v_end; ++v)
    {
      if(*v != target1 && true == boost::edge(*v, target1, graph_).second)
      {
        ++removed;
      }
    }
  }

  boost::graph_traits<undirected_graph>::edges_size_type added_edge_num = 0;
  std::pair<vertex, vertex> edge_to_add;
  try
  {
    thread2 = new std::thread([&]()
    {
      assert(non_existing_edges_.size() > 0);
      added_edge_num = (*var_generator_)();
      edge_to_add = non_existing_edges_[added_edge_num];
      assert(false == boost::edge(edge_to_add.first, edge_to_add.second, graph_).second);
      
      adjacency_iterator v, v_end;
      for(boost::tie(v, v_end) = boost::adjacent_vertices(edge_to_add.first, graph_); v != v_end; ++v)
      {
        if(true == boost::edge(*v, edge_to_add.second, graph_).second)
        {
          ++added;
        }
      }
    });
  }
  catch(std::system_error&)
  {
    //std::cout << "No resources to create new thread #2" << std::endl;
    assert(non_existing_edges_.size() > 0);
    added_edge_num = (*var_generator_)();
    edge_to_add = non_existing_edges_[added_edge_num];
    assert(false == boost::edge(edge_to_add.first, edge_to_add.second, graph_).second);

    adjacency_iterator v, v_end;
    for(boost::tie(v, v_end) = boost::adjacent_vertices(edge_to_add.first, graph_); v != v_end; ++v)
    {
      if(true == boost::edge(*v, edge_to_add.second, graph_).second)
      {
        ++added;
      }
    }
  }

  if(0 != thread1)
  {
    thread1->join();
    delete thread1;
  }
  if(0 != thread2)
  {
    thread2->join();
    delete thread2;
  }

  if(edge_to_add.first == source1 && true == boost::edge(edge_to_add.second, target1, graph_).second ||
     edge_to_add.first == target1 && true == boost::edge(edge_to_add.second, source1, graph_).second ||
     edge_to_add.second == source1 && true == boost::edge(edge_to_add.first, target1, graph_).second ||
     edge_to_add.second == target1 && true == boost::edge(edge_to_add.first, source1, graph_).second)
  {
    --added;
  }

  int delta = added - removed;
  if(false == check_step(delta))
  {
    return;
  }
  boost::remove_edge(edge_to_remove, graph_);
  //std::pair<vertex, vertex> edge_to_add = non_existing_edges_[added_edge_num];
  boost::add_edge(edge_to_add.first, edge_to_add.second, graph_);
  non_existing_edges_[added_edge_num] = std::make_pair(source1, target1);
  num_triangles_ += delta;
  //non_existing_edges_.erase(non_existing_edges_.begin() + added_edge_num);
  //non_existing_edges_.push_back(std::make_pair(source1, target1));
}

}
