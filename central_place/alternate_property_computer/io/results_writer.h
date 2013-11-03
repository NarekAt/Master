/**
 * @file io/results_writer.h
 * @brief declaration of results_writer class
 */

#pragma once

#include "types.h"

/**
 * TODO: 1) make base class for writers to
 *          write different kinds of results.
 */

/**
 * @class results_writer
 * @brief writes results
 */
class results_writer
{
public:
    /**
     * @brief Writes results
     * @param r Calculation results.
     * @param n Vertex count of graph
     * @param p Probability of Erdos-Renyi graph
     */
    void write(const calculation_results& r, unsigned n, double p);

private:
    /**
     * @brief Prepares output directory.
     * @param n Vertex count of graph
     * @param p Probability of Erdos-Renyi graph
     * @return Returns false if praparing faild.
     */
    bool prepare_output_directory(unsigned n, double p);

private:
    /**
     * @brief Writes single result list.
     * @param r Single result list.
     * @param n Vertex count of graph.
     * @param p Probability of Erdos-Renyi graph
     * @param mu Mu of calculation.
     */
    void write_single_results_list(const single_results_list& r,
        unsigned n, double p, double mu) const;

private:
    std::string m_directory_name;
};
