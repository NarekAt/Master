#include "main_app.h"
#include "task_factory.h"
#include "defs.h"
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <system_error>
#include <boost/program_options.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/utility.hpp>

enum message_tag
{
  vertex_count_tag = 1,
  probability_tag,
  graph_tag,
  mu_count_per_process_tag,
  mu_array_per_process_tag,
  pass_count_tag,
  results_base_tag = 1000
};

//validator for type field
struct randomization_type
{
  randomization_type(const std::string& value)
    : value_(value)
  {}
  std::string value_;
};

void validate(boost::any& v, std::vector<std::string> const& values, randomization_type* /* target_type */, int)
{
  // Make sure no previous assignment to 'v' was made.
  po::validators::check_first_occurrence(v);

  // Extract the first string from 'values'. If there is more than
  // one string, it's an error, and exception will be thrown.
  const std::string& s = po::validators::get_single_string(values);

  if (s == "random_switch" || s == "fixed_degree")
  {
    v = boost::any(randomization_type(s));
  } 
  else 
  {
    throw po::validation_error(po::validation_error::invalid_option_value);
  }
}

main_app::main_app(int argc, char* argv[])
  : argc_(argc)
  , argv_(argv)
  , type_(std::string())
  , env_(argc, argv)
  , world_()
  , output_directory_(std::string())
  , graph_file_name_(std::string())
  , mu_file_name_(std::string())
  , step_count_(1000)
  , pass_count_(1)
{
  rank_ = world_.rank();
  size_ = world_.size();
}

main_app::~main_app()
{
  finalize();
}

bool main_app::parse_command_line()
{
  po::options_description desc("Options");
  desc.add_options()
    ("help,h", "Print help message")
    ("graph_file,g", po::value<std::string>()->required(), "File which contains graph")
    ("mu_file,m", po::value<std::string>()->required(), "File which contains mu sequence or mu range with step")
    ("step_count,c", po::value<unsigned>(), "Step count per generation.")
    ("pass_count,p", po::value<unsigned>(), "Pass count for each mu value.")
    ("type,t", po::value<randomization_type>(), "Randomization type.");

  po::variables_map vm;
  try
  {
    po::store(po::parse_command_line(argc_, argv_, desc), vm);
    if(vm.count("help"))
    {
      std::cout << "Graph Randomizer v1.0" << std::endl << desc << std::endl;
    }
    if(vm.count("graph_file"))
    {
      std::cout << "graph file name = " << vm["graph_file"].as<std::string>() << std::endl;
      graph_file_name_ = vm["graph_file"].as<std::string>();
    }
    if(vm.count("mu_file"))
    {
      std::cout << "mu file name = " << vm["mu_file"].as<std::string>() << std::endl;
      mu_file_name_ = vm["mu_file"].as<std::string>();
    }
    if(vm.count("step_count"))
    {
      std::cout << "step count = " << vm["step_count"].as<unsigned>() << std::endl;
      step_count_ = vm["step_count"].as<unsigned>();
    }
    if(vm.count("pass_count"))
    {
      std::cout << "pass count = " << vm["pass"].as<unsigned>() << std::endl;
      pass_count_ = vm["pass_count"].as<unsigned>();
    }
    if(vm.count("type"))
    {
      std::cout << "type = " << vm["type"].as<randomization_type>().value_ << std::endl;
      type_ = vm["type"].as<randomization_type>().value_;
    }
    po::notify(vm);
  }
  catch(po::error& e)
  {
    std::cerr << "\nError parsing command line: " << e.what() << std::endl << std::endl;
    std::cerr << desc << std::endl;
  }
}

void main_app::execute()
{
  assert(1 <= size_);
  parse_command_line();
  if(1 == size_)
  {
    execute_single_threaded();
  }
  else
  {
    execute_multi_threaded();
  }
}

void main_app::execute_single_threaded()
{
  graph_randomization::undirected_graph graph;
  std::ifstream graph_file;
  graph_file.open(graph_file_name_);
  if(false == graph_file.is_open())
  {
    std::cerr << "Failed to open file containing graph." << std::endl;
    finalize();
    env_.abort(-1);
  }
  archive::text_iarchive ia(graph_file);
  int vertex_count = 0;
  double probability = 0.0;
  ia >> vertex_count >> probability;
  serialization::load(ia, graph, 0);
  std::ifstream mu_file;
  mu_file.open(mu_file_name_);
  if(false == mu_file.is_open())
  {
    std::cerr << "Failed to open file containing mu values." << std::endl;
    finalize();
    env_.abort(-1);
  }
  std::vector<double> mu_values;
  //optimize for vector allocation
  while(false == mu_file.eof())
  {
    double mu = 0.0;
    mu_file >> mu;
    mu_values.push_back(mu);
  }
  int mu_count = mu_values.size();
  std::cerr << "NOT IMPLEMENTED YET." << std::endl;
}

void main_app::execute_multi_threaded()
{
  if(0 == rank_)
  {
    execute_main_process();
  }
  else
  {
    execute_secondary_process();
  }
  world_.barrier();
  if(0 == rank_)
  {
    collect_results();
  }
}

void main_app::finalize()
{
  // finalizing code here
}

void main_app::execute_main_process()
{
  // input -- graph file, mu file, step count
  // read both files and broadcast to all processes
  graph_randomization::undirected_graph graph;
  std::ifstream graph_file;
  graph_file.open(graph_file_name_);
  if(false == graph_file.is_open())
  {
    std::cerr << "Failed to open file containing graph." << std::endl;
    finalize();
    env_.abort(-1);
  }
  archive::text_iarchive ia(graph_file);
  int vertex_count = 0;
  double probability = 0.0;
  ia >> vertex_count >> probability;
  serialization::load(ia, graph, 0);
  std::ifstream mu_file;
  mu_file.open(mu_file_name_);
  if(false == mu_file.is_open())
  {
    std::cerr << "Failed to open file containing mu values." << std::endl;
    finalize();
    env_.abort(-1);
  }
  std::vector<double> mu_values;
  //optimize for vector allocation
  {
    double mu = 0.0;
    while(mu_file >> mu)
    {
      mu_values.push_back(mu);
    }
  }
  int mu_count = mu_values.size();

//  world_.barrier();
  for(int i = 1; i < size_; ++i)
  {
    world_.send(i, vertex_count_tag, vertex_count);
  }
  for(int i = 1; i < size_; ++i)
  {
    world_.send(i, probability_tag, probability);
  }
  for(int i = 1; i < size_; ++i)
  {
    world_.send(i, graph_tag, graph);
  }
  /*for(int i = 1; i < size_; ++i)
  {
    world_.send(i, pass_count_tag, pass_count_);
  }*/

  std::cout << "hardware concurrency " << std::thread::hardware_concurrency() << std::endl;

  assert(size_ > 1);
  int mu_per_process = mu_count / (size_ - 1);
  if(0 == mu_per_process)
  {
    mu_per_process = 1;
  }
  else
  {
    int remainder = mu_count - mu_per_process * (size_ - 1);
    std::cout << "remainder = " << remainder << std::endl;
  }
  for(int i = 1; i < size_; ++i)
  {
    double* mu_array = new double[mu_per_process];
    for(int j = 0; j < mu_per_process; ++j)
    {
      mu_array[j] = mu_values[(i-1)*mu_per_process + j];
    }
    world_.send(i, mu_count_per_process_tag, mu_per_process);
    world_.send(i, mu_array_per_process_tag, mu_array, mu_per_process);
    delete[] mu_array;
  }
  std::vector<std::pair<mpi::request, int>> reqs;
  reqs.resize((size_-1)*mu_per_process);
  std::vector<std::pair<int, double>>* results = new std::vector<std::pair<int, double>>[(size_-1)*mu_per_process];
  for(int i = 1; i < size_; ++i)
  {
    for(int j = 0; j < mu_per_process; ++j)
    {
      reqs[(i-1)*mu_per_process + j].first = world_.irecv(i, results_base_tag*i+j, results[(i-1)*mu_per_process + j]);
      reqs[(i-1)*mu_per_process + j].second = (i-1)*mu_per_process + j;
    }
  }
  prepare_output_directory();
  while(false == reqs.empty())
  {
    for(int i = reqs.size() - 1; i >= 0; --i)
    {
      if(false == !reqs[i].first.test())
      {
        std::cout << "received for " << mu_values[reqs[i].second] << std::endl;
        write_output(vertex_count, probability, mu_values[reqs[i].second], results[reqs[i].second]);
        reqs.erase(reqs.begin() + i);
      }
    }
    usleep(100);
  }
  delete[] results;
  std::cout << "received all information" << std::endl;
}

void main_app::execute_secondary_process()
{
  // receive graph file and mus
  // process them
  //world_.barrier();
  int vertex_count = 0;
  world_.recv(0, vertex_count_tag, vertex_count);
  double probability = 0.0;
  world_.recv(0, probability_tag, probability);
  graph_randomization::undirected_graph graph;
  world_.recv(0, graph_tag, graph);
  int mu_count = 0;
  world_.recv(0, mu_count_per_process_tag, mu_count);
  double* mu_array = new double[mu_count];
  world_.recv(0, mu_array_per_process_tag, mu_array, mu_count);
  //world_.recv(0, pass_count_tag, pass_count_);

  mpi::request* reqs = new mpi::request[mu_count];
  for(int i = 0; i < mu_count; ++i)
  {
    // create generating object and execute
    // send results to main process
    std::cout << "starting: steps " << step_count_ << " mu value " << mu_array[i] << std::endl;
    std::vector<std::pair<int, double>> results(step_count_ + 1);
    for(int k = 0; k < results.size(); ++k)
    {
      results[k].first = k;
    }
    unsigned pass_count = 10;
    for(int p = 0; p < pass_count; ++p)
    {
      graph_randomization::base_task* task = graph_randomization::get_task(graph, mu_array[i], step_count_, type_); 
      task->perform_randomization();
      std::vector<std::pair<int, int>> results_for_i = task->results();
      assert(step_count_ + 1 == results_for_i.size());
      for(int j = 0; j < results_for_i.size(); ++j)
      {
        assert(results[j].first == results_for_i[j].first);
        results[j].second += static_cast<double>(results_for_i[j].second);
      }
      delete task;
    }
      std::cout << "finished: steps " << step_count_ << " mu value " << mu_array[i] << std::endl;
    for(int j = 0; j < results.size(); ++j)
    {
      results[j].second /= pass_count;
    }
    reqs[i] = world_.isend(0, results_base_tag*rank_ + i, results);
  }
  mpi::wait_all(reqs, reqs + mu_count);
  delete[] reqs;
  delete[] mu_array;
}

void main_app::collect_results()
{
}

void main_app::prepare_output_directory()
{
  using std::chrono::system_clock;
  std::stringstream output_folder;
  std::time_t tt = system_clock::to_time_t(system_clock::now());
  struct std::tm* ptm = std::localtime(&tt);
  char buf[100];
  strftime(buf, sizeof(buf), "%Y.%m.%d_%H.%M.%S", ptm);
  output_folder << "results_" << buf;//std::put_time(ptm, "%F_%T");
  output_directory_ = output_folder.str();
  fs::create_directory(output_directory_);
}

void main_app::write_output(int N, double p, double mu, const std::vector<std::pair<int, double>>& result) const
{
  std::stringstream file_name;
  file_name << (output_directory_.empty() ? "" : output_directory_ + "/") <<"N" << N << "_p" << p << "_u" << mu << "_T";
  //try
  //{
    fs::create_directory(file_name.str());
  //}
  //catch(fs::basic_filesystem_error&)
  //{
  //  std::cerr << "Directory already exists." << std::endl;
  //  return;
  //}
  //file_name << "/N" << N << "_p" << p << "_u" << mu << ".txt";
  file_name << "/" << 0 << ".txt";
  std::ofstream output;
  output.open(file_name.str());
  if(false == output.is_open())
  {
    std::cerr << "Failed to open/create output file." << std::endl;
    return;
  }
  output << N << " " << p << " " << mu << std::endl;
  for(int i = 0; i < result.size(); ++i)
  {
    output << result[i].first << " " << result[i].second << std::endl;
  }
  output.close();
}
