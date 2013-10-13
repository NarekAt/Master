/**
 * @file main/main.cpp
 * @brief Contains main(...) function call
#include "package.h"
#include "argument_parser.h"
#include "mediator.h"
#include "exception_base.h"
#include <iostream>

int main(int argc, char* argv[])
{
    package::init();
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    auto& a_p = argument_parser::get_instance();
    auto& m = mediator::get_instance();
    try {
        m.init(a_p.parse_and_get_args(argc, argv, world.rank()));
        m.run(world);
    } catch (const exception_base& e) {
        // Only main process can abort environment
        // and write info, warning and error messages.
        if (0 == world.rank()) {
            // TODO: Change cout to log.
            std::cout << std::endl << "ERROR: "
                << e.get_message() << std::endl;
            package::uninit();
            env.abort(-1);
        }
    }
    package::uninit();
    return 0;
}
