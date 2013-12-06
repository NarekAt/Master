/**
 * @file package/package.cpp
 * @brief definition of package class.
 */

#include "package.h"
#include "mediator.h"
#include "argument_parser.h"
#include "results_writer.h"

void package::init()
{
    argument_parser::instantiate();
    mediator::instantiate();
    results_writer::instantiate();
}

void package::uninit()
{
    results_writer::destroy();
    mediator::destroy();
    argument_parser::destroy();
}
