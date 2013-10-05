/**
 * @file argument_parser/argument_parser.cpp
 * @description definition of class argument_parser.
 */

#include "argument_parser.h"
#include <assert.h>

argument_parser* argument_parser::s_instance = nullptr;

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
