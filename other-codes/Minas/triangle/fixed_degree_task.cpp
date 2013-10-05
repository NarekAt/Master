#include "fixed_degree_task.h"
#include <system_error>

namespace graph_randomization
{

fixed_degree_task::fixed_degree_task(const undirected_graph& graph, double mu, int step_count)
  : base_task(graph, mu, step_count)
{
}

void fixed_degree_task::make_randomization_step()
{
  edge e1 = boost::random_edge(graph_, rand_generator_);
  edge e2 = boost::random_edge(graph_, rand_generator_);
  vertex vs1 = boost::source(e1, graph_);
  vertex vt1 = boost::target(e1, graph_);
  vertex vs2 = boost::source(e2, graph_);
  vertex vt2 = boost::target(e2, graph_);
  while(e1 == e2 || vs1 == vs2 || vt1 == vt2 ||
        true == boost::edge(vs1, vs2, graph_).second ||
        true == boost::edge(vt1, vt2, graph_).second)
  {
    e2 = boost::random_edge(graph_, rand_generator_);
    vs2 = boost::source(e2, graph_);
    vt2 = boost::target(e2, graph_);
  }
  assert(e1 != e2 && vs1 != vs2 && vt1 != vt2 &&
         false == boost::edge(vs1, vs2, graph_).second &&
         false == boost::edge(vt1, vt2, graph_).second);

  //int bvs1 =  boost::degree(vs1, graph_);
  //int bvt1 = boost::degree(vt1, graph_);
  //int bvs2 = boost::degree(vs2, graph_);
  //int bvt2 = boost::degree(vt2, graph_);
  // removing edges
  boost::remove_edge(e1, graph_);
  boost::remove_edge(e2, graph_);
  assert(false == boost::edge(vs1, vt1, graph_).second &&
         false == boost::edge(vs2, vt2, graph_).second);
  
  int removed = 0;
  int added = 0;
  std::thread* thread1 = 0;
  std::thread* thread2 = 0;
  try
  {
    thread1 = new std::thread([&]()
    {
      adjacency_iterator v, v_end;
      for(boost::tie(v, v_end) = boost::adjacent_vertices(vs1, graph_); v != v_end; ++v)
      {
        if(*v != vt1 && true == boost::edge(*v, vt1, graph_).second)
        {
          ++removed;
        }
      }
      for(boost::tie(v, v_end) = boost::adjacent_vertices(vs2, graph_); v != v_end; ++v)
      {
        if(*v != vt2  && true == boost::edge(*v, vt2, graph_).second)
        {
          ++removed;
        }
      }
    });
  }
  catch(std::system_error&)
  {
//    std::cout << "No resources to create new thread #1" << std::endl;
    adjacency_iterator v, v_end;
    for(boost::tie(v, v_end) = boost::adjacent_vertices(vs1, graph_); v != v_end; ++v)
    {
      if(*v != vt1 && true == boost::edge(*v, vt1, graph_).second)
      {
        ++removed;
      }
    }
    for(boost::tie(v, v_end) = boost::adjacent_vertices(vs2, graph_); v != v_end; ++v)
    {
      if(*v != vt2  && true == boost::edge(*v, vt2, graph_).second)
      {
        ++removed;
      }
    }
  }
  // adding new edges
  try
  {
    thread2 = new std::thread([&]()
    {
      adjacency_iterator v, v_end;
      for(boost::tie(v, v_end) = boost::adjacent_vertices(vs1, graph_); v != v_end; ++v)
      {
        if(*v != vs2 && true == boost::edge(*v, vs2, graph_).second)
        {
          ++added;
        }
      }
      for(boost::tie(v, v_end) = boost::adjacent_vertices(vt1, graph_); v != v_end; ++v)
      {
        if(*v != vt2 && true == boost::edge(*v, vt2, graph_).second)
        {
          ++added;
        }
      }
    });
  }
  catch(std::system_error&)
  {
//    std::cout << "No resources to create new thread #2" << std::endl;
    adjacency_iterator v, v_end;
    for(boost::tie(v, v_end) = boost::adjacent_vertices(vs1, graph_); v != v_end; ++v)
    {
      if(*v != vs2 && true == boost::edge(*v, vs2, graph_).second)
      {
        ++added;
      }
    }
    for(boost::tie(v, v_end) = boost::adjacent_vertices(vt1, graph_); v != v_end; ++v)
    {
      if(*v != vt2 && true == boost::edge(*v, vt2, graph_).second)
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
  boost::add_edge(vs1, vs2, graph_);
  boost::add_edge(vt1, vt2, graph_);
  int delta  = added - removed;
  if(true == check_step(delta))
  {
//  std::cout << "vs1 " << bvs1 << " " << boost::degree(vs1, graph_) << 
//    " vt1 " << bvt1 << " " << boost::degree(vt1, graph_) << 
//    " vs2 " << bvs2 << " " << boost::degree(vs2, graph_) << 
//    " vt2 " << bvt2 << " " << boost::degree(vt2, graph_) << "\n";
    num_triangles_ += delta;
    return; 
  }
  boost::add_edge(vs1, vt1, graph_);
  boost::add_edge(vs2, vt2, graph_);
  boost::remove_edge(vs1, vs2, graph_);
  boost::remove_edge(vt1, vt2, graph_);
}

}
