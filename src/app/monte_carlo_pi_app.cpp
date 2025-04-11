#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <string>
#include "monte_carlo_pi.h"
#include "MainForm.h"

using namespace monte_carlo_pi_app;
using namespace System;
using namespace System::Windows::Forms;

// Test different thread counts (0 = sequential, 1 = default threads, 2,4,8,16 threads)
const std::vector<int> thread_counts = {0, 1, 2, 4, 8, 16};

// Function to measure execution time with multiple runs
template<typename Func>
std::vector<double> measure_time_multiple_runs(Func f, int num_runs = 5) {
  std::vector<double> times;
  times.reserve(num_runs);

  for (int i = 0; i < num_runs; ++i) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    times.push_back(std::chrono::duration<double>(end - start).count());
  }

  return times;
}

// Function to calculate statistics
struct TimeStats {
  double mean;
  double std_dev;
  double min;
  double max;
};

TimeStats calculate_stats(const std::vector<double> &times) {
  TimeStats stats;
  stats.mean = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
  double sq_sum = std::inner_product(times.begin(), times.end(), times.begin(), 0.0);
  stats.std_dev = std::sqrt(sq_sum / times.size() - stats.mean * stats.mean);
  auto [min_it, max_it] = std::minmax_element(times.begin(), times.end());
  stats.min = *min_it;
  stats.max = *max_it;
  return stats;
}

// Function to save results to CSV file
void save_results_to_csv(const std::string &filename,
                         const std::vector<long long> &point_counts,
                         const std::vector<std::vector<TimeStats>> &seq_stats,
                         const std::vector<std::vector<TimeStats>> &par_stats) {
  std::ofstream file(filename);
  file << "Points,Threads,Sequential Mean (s),Sequential StdDev (s),Sequential Min (s),Sequential Max (s),"
       << "Parallel Mean (s),Parallel StdDev (s),Parallel Min (s),Parallel Max (s),Speedup\n";

  for (size_t i = 0; i < point_counts.size(); ++i) {
    for (size_t j = 0; j < seq_stats[i].size(); ++j) {
      const auto &seq = seq_stats[i][j];
      const auto &par = par_stats[i][j];
      double speedup = seq.mean / par.mean;
      file << point_counts[i] << ","
           << (j == 0 ? "sequential" : std::to_string(thread_counts[j])) << ","
           << std::fixed << std::setprecision(6)
           << seq.mean << "," << seq.std_dev << "," << seq.min << "," << seq.max << ","
           << par.mean << "," << par.std_dev << "," << par.min << "," << par.max << ","
           << speedup << "\n";
    }
  }
}

[STAThread]
int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
                   LPSTR /*lpCmdLine*/, int /*nCmdShow*/) {
  Application::EnableVisualStyles();
  Application::SetCompatibleTextRenderingDefault(false);
  Application::Run(gcnew MainForm());
  return 0;
}
