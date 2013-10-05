/**
 * @file argument_parser/argument_parser.h
 * @description declaration of class argument_parser.
 */

#pragma once

/**
 * @class argument_parser
 * @brief this class is used for parsing argument of program and
 *        converting them to the appropriate types.
 * @note Singleton class.
 */
class argument_parser
{
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
    argument_parser() = default;

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
