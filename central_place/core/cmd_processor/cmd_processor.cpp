#include "cmd_processor.h"

/**
 * @file cmd_processor.cpp
 * @brief definition of class 
 *              cmd_processor
 * @author Hayk Khachatryan
 */

#include "cmd_base.h"
#include "cmd_exception.h"
#include <assert.h>

namespace cmd
{

cmd_processor* cmd_processor::s_instance = nullptr;

void cmd_processor::add_command(cmd_base* cmd)
{
    assert(nullptr != cmd);
    assert(m_commands.end() == m_commands.find(cmd->get_name()));
    m_commands.insert(std::make_pair(cmd->get_name(), cmd));
}

void cmd_processor::parse_and_execute_command(const std::string& c_l)
{
    unsigned cmd_name_start = c_l.find_first_not_of(" ");
    unsigned cmd_name_end = c_l.find_first_of(" ", cmd_name_start);
    std::string cmd_name = "";
    std::string cmd_args = "";
    if (std::string::npos != cmd_name_end) {
        cmd_name = c_l.substr(cmd_name_start
            , cmd_name_end - cmd_name_start);
        cmd_args = c_l.substr(cmd_name_end);
    } else {
        cmd_name = c_l.substr(cmd_name_start);
    }
    auto it = m_commands.find(cmd_name);
    if (m_commands.end() == it) {
        std::string message = 
            std::string("There is no command with name '")
            + cmd_name + std::string("'");
        throw exception(message);
    }
    parse_arg_list(it->second, cmd_args);
    (it->second)->execute();
    (it->second)->reset();
}

void cmd_processor::parse_arg_list(cmd_base* cmd
    , const std::string& a_l)
{
    assert(nullptr != cmd);
    std::vector<std::string> args;
    split_arg_line(a_l, args);
    if (0 != args.size()%2) {
        std::string message("Invalid argument list");
        throw exception(message);
    }
    for (auto it = args.begin(); it != args.end(); it+=2) {
        if ('-' != (*it)[0] || 1 == it->size()) {
            std::string message = 
                std::string("Invalid argument name '")
                + *it + std::string("'");
            throw exception(message);
        }
        cmd->set_arg_value((*it).substr(1), *(it + 1));
    }
}

void cmd_processor::split_arg_line(const std::string& a_l
    , std::vector<std::string>& args)
{
    int s_word_start = a_l.find_first_not_of(" ");
    int s_word_end = 0;
    while(std::string::npos != s_word_start) {
            s_word_end = a_l.find_first_of(" ", s_word_start);
            args.push_back(a_l.substr(s_word_start, 
                    s_word_end - s_word_start));
            s_word_start = a_l.find_first_not_of(" ", s_word_end);
    }
}

cmd_processor& cmd_processor::get_instance()
{
    assert(s_instance != nullptr);
    return *s_instance;
}

void cmd_processor::instantiate()
{
    assert(s_instance == nullptr);
    s_instance = new cmd_processor();
}

void cmd_processor::destroy()
{
    assert(s_instance != nullptr);
    delete s_instance;
    s_instance = nullptr;
}

cmd_processor::~cmd_processor()
{
    for (auto& v : m_commands) {
        delete v.second;
    }
    m_commands.clear();
}

}
