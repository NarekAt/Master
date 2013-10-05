#include "central_engine.h"

/**
 * @file central_engine.cpp
 * @brief definition of class 
 *              central_engine
 * @author Hayk Khachatryan
 */

#include "cmd_processor.h"
#include "cmd_exception.h"
#include "commands.h"
#include <iostream>
#include <assert.h>

central_engine* central_engine::s_instance = nullptr;

void central_engine::run()
{
    std::string command_line;
    cmd::cmd_processor& cmd_proc = cmd::cmd_processor::get_instance();
    while (true) {
        command_line.clear();
        std::cout << ">>> ";
        std::getline(std::cin, command_line);
        if ("" == command_line || std::string::npos 
            == command_line.find_first_not_of(" ")) {
            continue;
        }
        try {
            cmd_proc.parse_and_execute_command(command_line);
        } catch (cmd::exception& ex) {
            std::cout << "command ERROR: " << ex.get_message() 
                << std::endl;
        }
    }
    // TODO: write body.
}

void central_engine::init()
{
    init_cmd_processor();
}

void central_engine::init_cmd_processor()
{
    cmd::cmd_processor::instantiate();
    cmd::cmd_processor& cmd_proc = cmd::cmd_processor::get_instance();
    cmd_proc.add_command(new cmd_graph_converter());
    // TODO: add commands to cmd processor.
}

void central_engine::uninit()
{
    uninit_cmd_processor();
}

void central_engine::uninit_cmd_processor()
{
    cmd::cmd_processor::destroy();
}

central_engine& central_engine::get_instance()
{
    assert(s_instance != nullptr);
    return *s_instance;
}

void central_engine::instantiate()
{
    assert(s_instance == nullptr);
    s_instance = new central_engine();
}

void central_engine::destroy()
{
    assert(s_instance != nullptr);
    delete s_instance;
    s_instance = nullptr;
}

central_engine::central_engine()
{
    init();
}

central_engine::~central_engine()
{
    uninit();
}
