/**
 * @file common/types.h
 * @brief common typedefs enums e.t.c
 */

#pragma once

#include <map>
#include <set>
#include <list>
#include <string>
#include <boost/any.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>

/**
 * @brief Map of process arguments name to value.
 */
typedef std::map<std::string, boost::any> arg_name_to_value_map;

/**
 * @enum randomization_type
 * @brief This type shows how to randomize graphs.
 * @note INVALID_RT value notifies that
 *       invalid name of type was red.
 */
enum randomization_type {
    RANDOM_SWITCH,
    FIXED_DEGREE,
    INVALID_RT
};

/**
 * @brief Gets randomization type value by name.
 * @param t_n Randomization type name.
 * @return Returns appropriate randomization type 
 *         if it is valid, otherwise returns INVALID_RT enum value.
 */
randomization_type get_randomization_type_by_name(
    const std::string& t_n);

/**
 * @brief Gets randomization type name by value.
 * @param t Randomization type.
 */
std::string get_randomization_name_by_type(
    randomization_type t);

/**
 * @enum alternate_property_type
 * @brief This type shows what property to compute from graphs.
 * @note INVALID_APT value notifies that
 *       invalid name of type was red.
 */
enum alternate_property_type {
    TRIANGLE_COUNT,
    QUADRANGLE_COUNT,
    INVALID_APT
};

/**
 * @brief Gets alternate property type value by name.
 * @param t_n Alternate property type name.
 * @return Returns appropriate alternate property type 
 *         if it is valid, otherwise returns INVALID_APT enum value.
 */
alternate_property_type get_alternate_property_type_by_name(
    const std::string& t_n);

/**
 * @brief Gets alternate property type name by value.
 * @param t Aternate property type.
 */
std::string get_alternate_property_name_by_type(
    alternate_property_type t);

static int s_status_step = 500;

/**
 * @brief Persent of work which was done to mu
 */
typedef std::pair<double, double> persent_to_mu;

/**
 * @brief mu values list.
 */
typedef std::vector<double> mu_list;

/**
 * @brief process id to mu count map.
 */
typedef std::vector<std::pair<unsigned, unsigned>> process_to_mu_count;

/**
 * @brief mpi tag to mu vector.
 */
typedef std::vector<std::pair<unsigned, double>> tag_to_mu_map;

/**
 * @brief Work status tags for process.
 */
typedef std::vector<unsigned> status_tags;

/**
 * @brief vector of pairs: step number to setp property count.
 */
typedef std::vector<std::pair<unsigned, double>> single_results_list;

/**
 * @brief map from mu value to appropriate vector of pairs:
 *        step number to step property count.
 */
typedef std::vector<std::pair<double, single_results_list>>
    calculation_results;

namespace graph_types
{

// undirected graph
typedef boost::adjacency_list<
    boost::setS, boost::listS, boost::undirectedS> undirected_graph;

// random generator
typedef std::mt19937 random_generator;

// Erdos-Renyi generator
typedef boost::sorted_erdos_renyi_iterator<
    random_generator, undirected_graph> erdos_renyi_iterator;

// graph vertex
typedef boost::graph_traits<undirected_graph>::vertex_descriptor 
    vertex;

// graph vertex pair
typedef std::pair<vertex, vertex> vertex_pair;

// graph nonexisting edges container.
typedef std::set<vertex_pair> null_edges;

// graph nonexisting edges sequent container
typedef std::vector<vertex_pair> sequent_null_edges;

// graph edge
typedef boost::graph_traits<undirected_graph>::edge_descriptor edge;

// graph out edge
typedef boost::graph_traits<undirected_graph>::out_edge_iterator 
    out_edge_iterator;

// graph edge
typedef boost::graph_traits<undirected_graph>::edge_iterator 
    edge_iterator;

// vertex iterator
typedef boost::graph_traits<undirected_graph>::vertex_iterator 
    vertex_iterator;

// adjacency list iterator
typedef boost::graph_traits<undirected_graph>::adjacency_iterator 
    adjacency_iterator;

}

typedef std::pair<graph_types::null_edges,
    graph_types::null_edges> remove_add_edges_pair;
