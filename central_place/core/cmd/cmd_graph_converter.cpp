#include "cmd_graph_converter.h"
#include "cmd_arg_string.h"
#include "cmd_exception.h"
#include <unistd.h>

void cmd_graph_converter::execute()
{
    int pid = fork();
    if (-1 == pid) {
        std::string message("cannot fork program");
        throw cmd::exception(message);
    } else if (0 == pid) {
        std::string executable = std::string("graph_converter.exe")
            + std::string(" ") + m_flag->get_value()
            + std::string(" ") + m_file_from->get_value()
            + std::string(" ") + m_file_to->get_value();
        execvp(executable.c_str(), NULL);  
    }
}

cmd_graph_converter::cmd_graph_converter() 
    : cmd_base("graph_converter")
{
    m_flag = new cmd_arg_string("flag");
    add_argument(m_flag);
    m_file_from = new cmd_arg_string("file_from");
    add_argument(m_file_from);
    m_file_to = new cmd_arg_string("file_to");
    add_argument(m_file_to);
}
