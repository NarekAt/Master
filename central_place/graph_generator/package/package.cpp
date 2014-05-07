/**
 * @file package/package.cpp
 * @brief definition of package class.
 */

#include "package.h"
#include "coordinator.h"
#include "argument_parser.h"

void package::init()
{
    argument_parser::instantiate();
    coordinator::instantiate();
}

void package::uninit()
{
    coordinator::destroy();
    argument_parser::destroy();
}
