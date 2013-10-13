/**
 * @file io/results_writer.cpp
 * @brief definition of results_writer class
 */

#include "results_writer.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>

void results_writer::write(const calculation_results& r,
    unsigned n, double p)
{
    if (!prepare_output_directory()) {
        // TODO: throw exception.
        return; // dont forget to remove, 
                // after throw part will be implemented.
    }
    for (auto& s_r : r) {
        write_single_results_list(s_r.second, n, p, s_r.first);
    }
}

namespace {

bool if_dir_dont_exists_then_create(const std::string& d_n)
{
    auto f = boost::filesystem::status(d_n);
    if (!boost::filesystem::exists(f)) {
        boost::filesystem::create_directory(d_n);
    } else if (!boost::filesystem::is_directory(f)) {
        return false;
    }
    return true;
}

std::string get_time_name_of_folder()
{
    auto tt = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now());
    auto ptm = std::localtime(&tt);
    char buf[100];
    strftime(buf, sizeof(buf), "%Y.%m.%d_%H.%M.%S", ptm);
    std::string name = std::string("results_") + std::string(buf);
    return name;
}

}

bool results_writer::prepare_output_directory()
{
    m_directory_name = "global_results";
    if (!if_dir_dont_exists_then_create(m_directory_name)) {
        return false;
    }
    m_directory_name += std::string("/") + 
        "alternate_property_computer_results";
    if (!if_dir_dont_exists_then_create(m_directory_name)) {
        return false;
    }
    m_directory_name += std::string("/") +
        get_time_name_of_folder();
    if (!if_dir_dont_exists_then_create(m_directory_name)) {
        return false;
    }
    return true;
}

void results_writer::write_single_results_list(
    const single_results_list& r,
    unsigned n, double p, double mu) const
{
    std::string file_name = m_directory_name + "/" + "N" + std::to_string(n)
        + "_p" + std::to_string(p) + 
        "_mu" + std::to_string(mu) + ".txt";
    auto f = boost::filesystem::status(file_name);
    if (boost::filesystem::exists(f)) {
        if (!boost::filesystem::is_regular_file(f)) {
            // TODO: write error message.
            return;
        }
        // TODO: write info message.
        return;
    }
    std::ofstream output;
    output.open(file_name);
    if (!output.is_open()) {
        // TODO: write error message.
        return;
    }
    output << n << " " << p << " " << mu << std::endl;
    for (auto n_r : r) {
        output << n_r.first << " " << n_r.second << std::endl;
    }
    output.close();
}
