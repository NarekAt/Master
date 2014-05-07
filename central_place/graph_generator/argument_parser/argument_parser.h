/**
 * @file argument_parser/argument_parser.h
 * @brief declaration of class argument_parser.
 */

#pragma once

#include "types.h"
#include <boost/program_options.hpp>

/**
 * @class argument_parser
 * @brief this class is used for parsing argument of program and
 *        converting them to the appropriate types.
 * @note Singleton class.
 */
class argument_parser
{
    /// @name parsing interface
    /// @{
public:
    /**
     * @brief Parses the arguments and returns map of name->value.
     * @param argc Argument cound of the process.
     * @param argv Pointer to the argument list of the process.
     */
    arg_name_to_value_map parse_and_get_args(
        int argc, char* argv[]) const;
private:
    boost::program_options::options_description m_options_description;
    /// @}
    
    /// @name singleton management
    /// @{
public:
    /**
     * @brief Gets singletone object.
     */
    static argument_parser& get_instance();

public:
    /**
     * @brief Intstantiates singletone object.
     */
    static void instantiate();

public:
    /**
     * @brief Destroys singletone object.
     */
    static void destroy();

private:
    static argument_parser* s_instance;
    /// @}

    /// @name Special member functions.
    /// @{
public:
    /**
     * @brief Constructor
     */
    argument_parser();

public:
    /**
     * @brief Destructor.
     */
    ~argument_parser() = default;
private:
    argument_parser(const argument_parser&) = delete;
    argument_parser& operator=(const argument_parser&) = delete;
    /// @}
};
