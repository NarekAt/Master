/**
 * @file io/erdos_renyi_reader.cpp
 * @description definition of erdos_renyi_reader class
 */

#include "erdos_renyi_reader.h"
#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/graph/adj_list_serialize.hpp>


void erdos_renyi_reader::get_mus_from_file(const std::string& f_n,
    mu_list& mus) const
{
    std::ifstream mu_file;
    mu_file.open(f_n);
    if (!mu_file.is_open()) {
        // TODO: throw exception.
    }
    while (!mu_file.eof()) {
        double mu = 0.0;
        mu_file >> mu;
        mus.push_back(mu);
    }
    mu_file.close();
}

void erdos_renyi_reader::get_graph_and_properties_from_file(
    const std::string& f_n,
    graph_types::undirected_graph& graph, int& v_c, double& p)
{
    std::ifstream graph_file;
    graph_file.open(f_n);
    if (!graph_file.is_open()) {
        // TODO: throw exception.
    }
    boost::archive::text_iarchive ia(graph_file);
    ia >> v_c >> p;
    boost::serialization::load(ia, graph, 0);
    graph_file.close();
}
