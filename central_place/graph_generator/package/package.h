/**
 * @file package/package.h
 * @brief declaration of package class.
 */

#pragma once

/**
 * @class package
 * @brief package class is needed for initialization
 *        and uninitialization singleton objects.
 *   
 *        list of singletons:
 *            argument_parser
 *            coordinator
 * @note No object can be created for this class,
 *       this class needed for its static functions.
 */
class package
{
public:
    /**
     * @brief initialize objects.
     */
    static void init();

public:
    /**
     * @brief uninitialize objects.
     */
    static void uninit();

    /// @name no-object creation interface.
    /// @{
private:
    package() = default;
    ~package() = default;
    package(const package&) = delete;
    package& operator=(const package&) = delete;
    /// @}
};
