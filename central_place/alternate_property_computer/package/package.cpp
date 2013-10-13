/**
 * @file package/package.cpp
 * @brief definition of package class.
 */

#include "package.h"
#include "mediator.h"
#include "argument_parser.h"

void package::init()
{
    argument_parser::instantiate();
    mediator::instantiate();
}

void package::uninit()
{
    mediator::destroy();
    argument_parser::destroy();
}
