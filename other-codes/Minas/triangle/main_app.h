#include <boost/mpi.hpp>
#include <string>

namespace mpi = boost::mpi;

class main_app
{
  public:
    main_app(int argc, char* argv[]);
    ~main_app();

    void execute();

  private:
    bool parse_command_line();
    void execute_single_threaded();
    void execute_multi_threaded();
    void execute_main_process();
    void execute_secondary_process();
    void collect_results();
    void finalize();
    void read_config_file(); // for future
    void prepare_output_directory();
    void write_output(int N, double p, double mu, const std::vector<std::pair<int, double>>& result) const;

  private:
    int argc_;
    char** argv_;
    std::string type_;
    mpi::environment env_;
    mpi::communicator world_;
    int rank_;
    int size_;
    std::string output_directory_;
    std::string graph_file_name_;
    std::string mu_file_name_;
    unsigned step_count_;
    unsigned pass_count_;
};
