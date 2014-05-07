/**
 * @file parallel_property_counters/parallel_triangle_counter.cpp
 * @brief definition of class parallel_triangle_counter.
 */

#include "parallel_triangle_counter.h"
#include "erdos_renyi_reader.h"

#include <iostream>
#include <ctime>
#include <assert.h>

//////////////////////////////////////////////////
//#include <boost/graph/erdos_renyi_generator.hpp>
//#include <boost/random/linear_congruential.hpp>
//////////////////////////////////////////////////

parallel_triangle_counter::parallel_triangle_counter(boost::mpi::communicator& comm,
    const std::string& infile) :
    m_comm(comm),
    m_count(-1),
    m_partition_begin(-1),
    m_partition_end(-1)
{

   ////////////////////////////////////////////////////////////////////////////////////////////////
   //typedef boost::erdos_renyi_iterator<boost::minstd_rand, graph_types::undirected_graph> ERGen;
   //boost::minstd_rand gen;
   //m_graph = graph_types::undirected_graph(ERGen(gen, 2000, 0.3), ERGen(), 2000);
   //m_vertex_count = num_vertices(m_graph);
   ////////////////////////////////////////////////////////////////////////////////////////////////
    erdos_renyi_reader r;
    r.get_graph_and_properties_from_file(infile, m_graph, m_vertex_count, m_probability);
    assert(m_vertex_count == num_vertices(m_graph));
}

void parallel_triangle_counter::count()
{
    init_current_partition();
    count_on_partition();
    if (0 == m_comm.rank()) {
        accumulate_results();
    } else {
        send_result();
    }
}

void parallel_triangle_counter::init_current_partition()
{
    int partition_size = m_vertex_count / m_comm.size();
    m_partition_begin = m_comm.rank() * partition_size;
    m_partition_end = (m_comm.rank() + 1) * partition_size; 
    if(m_comm.rank() == m_comm.size() - 1) {
        m_partition_end = m_vertex_count;
    }
    /////////////////////////////////////////////////////////////////////  
    //  std::cout << "Initializing graph partition for process: #" 
    //            << m_comm.rank() << " interval [" 
    //            << m_partition_begin << " "<< m_partition_end << ")\n";
    /////////////////////////////////////////////////////////////////////
}

void parallel_triangle_counter::count_on_partition()
{
    int vert_index = 0;
    graph_types::vertex_iterator vi, vi_end;
    for (tie(vi, vi_end) = boost::vertices(m_graph); vi != vi_end; ++vi, ++vert_index) {
        if(m_partition_begin <= vert_index && m_partition_end > vert_index) {
            int curr_vert_count = 0;
            graph_types::adjacency_iterator first_adj_vi, first_adj_vi_end;
            for(tie(first_adj_vi, first_adj_vi_end) = boost::adjacent_vertices(*vi, m_graph); 
                first_adj_vi != first_adj_vi_end; ++first_adj_vi) {
                graph_types::adjacency_iterator second_adj_vi, second_adj_vi_end;
                for(tie(second_adj_vi, second_adj_vi_end) = boost::adjacent_vertices(*vi, m_graph); 
                    second_adj_vi != second_adj_vi_end; ++second_adj_vi) {
                    if(*first_adj_vi != *second_adj_vi && true == boost::edge(*first_adj_vi,
                        *second_adj_vi, m_graph).second) {
                        ++curr_vert_count;
                    }
                }
            }
            m_count += curr_vert_count / 2;
        }
    }
}

void parallel_triangle_counter::send_result()
{
    assert(0 != m_comm.rank());
    m_comm.send(0, m_comm.rank(), m_count);
}

void parallel_triangle_counter::accumulate_results()
{
    assert(0 == m_comm.rank());
    int received = 0;
    for(int i = 1; i < m_comm.size(); ++i) {
        m_comm.recv(i, i, received);
        m_count += received;
    }
    m_count /= 3;
}

void parallel_triangle_counter::write_results() const
{
    if(0 == m_comm.rank())
    {
        std::cout << "Parallel computing is done. " << std::endl 
            << "Parallel processes count: " << m_comm.size() << std::endl
            << "Graph vertices count: " << m_vertex_count << std::endl
            << "Graph probability: " << m_probability << std::endl
            << "Triangles count: " << m_count << std::endl;
    }
}

