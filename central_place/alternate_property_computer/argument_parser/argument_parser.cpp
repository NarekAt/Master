/**
 * @file argument_parser/argument_parser.cpp
 * @description definition of class argument_parser.
 */

#include "argument_parser.h"
#include <assert.h>

argument_parser* argument_parser::s_instance = nullptr;

arg_name_to_value_map argument_parser::parse_and_get_args(
    int argc, char* argv[], int rank_of_process) const
{
    return arg_name_to_value_map();
    // TODO: fill body.
}

argument_parser& argument_parser::get_instance()
{
    assert(s_instance != nullptr);
    return *s_instance;
}

void argument_parser::instantiate()
{
    assert(s_instance == nullptr);
    s_instance = new argument_parser();
}

void argument_parser::destroy()
{
    assert(s_instance != nullptr);
    delete s_instance;
    s_instance = nullptr;
}
