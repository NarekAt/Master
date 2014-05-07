/**
 * @file main/main.cpp
 * @brief Contains main(...) function call
 */

#include "package.h"
#include "argument_parser.h"
#include "coordinator.h"
#include <iostream>

int main(int argc, char* argv[])
{
    package::init();
    auto& a_p = argument_parser::get_instance();
    auto& c = coordinator::get_instance();
    c.init(a_p.parse_and_get_args(argc, argv));
    c.run();
    package::uninit();
    return 0;
}
