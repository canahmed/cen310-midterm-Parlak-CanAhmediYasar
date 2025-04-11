#include <gtest/gtest.h>
#include "monte_carlo_pi.h"
#include <cmath>
#include <thread>
#include <vector>
#include <future>
#include <chrono>
#include <numeric>
#include <algorithm>

// Define M_PI if not defined (for Windows)
#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

namespace {

// Helper function to measure execution time
template<typename Func>
double measure_time(Func f) {
  auto start = std::chrono::high_resolution_clock::now();
  f();
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double>(end - start).count();
}

// Basic functionality tests
TEST(MonteCarloPiTest, SequentialCalculation) {
  const long long num_points = 1000000;
  double pi = monte_carlo_pi::calculate_pi_sequential(num_points);
  EXPECT_NEAR(pi, M_PI, 0.01);
}

TEST(MonteCarloPiTest, ParallelCalculation) {
  const long long num_points = 1000000;
  double pi = monte_carlo_pi::calculate_pi_parallel(num_points);
  EXPECT_NEAR(pi, M_PI, 0.01);
}

// Thread count tests
TEST(MonteCarloPiTest, DifferentThreadCounts) {
  const long long num_points = 1000000;

  // Test with different thread counts
  for (int threads = 1; threads <= 16; threads *= 2) {
    double pi = monte_carlo_pi::calculate_pi_parallel(num_points, threads);
    EXPECT_NEAR(pi, M_PI, 0.01);
  }
}

// Point count tests
TEST(MonteCarloPiTest, DifferentPointCounts) {
  std::vector<long long> point_counts = {1000, 10000, 100000, 1000000};

  for (long long points : point_counts) {
    double pi_seq = monte_carlo_pi::calculate_pi_sequential(points);
    double pi_par = monte_carlo_pi::calculate_pi_parallel(points);
    // Both implementations should give similar results
    EXPECT_NEAR(pi_seq, M_PI, 0.1);
    EXPECT_NEAR(pi_par, M_PI, 0.1);
  }
}

// Edge cases and error handling
TEST(MonteCarloPiTest, EdgeCases) {
  // Test with minimum points
  double pi_min = monte_carlo_pi::calculate_pi_sequential(1);
  EXPECT_TRUE(std::isfinite(pi_min));
  // Test with zero points (should handle gracefully)
  double pi_zero = monte_carlo_pi::calculate_pi_sequential(0);
  EXPECT_TRUE(std::isfinite(pi_zero));
  // Test with negative points (should handle gracefully)
  double pi_neg = monte_carlo_pi::calculate_pi_sequential(-1000);
  EXPECT_TRUE(std::isfinite(pi_neg));
}

// Performance tests
TEST(MonteCarloPiTest, PerformanceComparison) {
  const long long num_points = 1000000;
  const int num_runs = 5;
  // Measure sequential performance
  double seq_time = 0.0;

  for (int i = 0; i < num_runs; ++i) {
    seq_time += measure_time([num_points]() {
      monte_carlo_pi::calculate_pi_sequential(num_points);
    });
  }

  seq_time /= num_runs;  // Average time

  // Measure parallel performance with different thread counts
  for (int threads = 1; threads <= 8; threads *= 2) {
    double par_time = 0.0;

    for (int i = 0; i < num_runs; ++i) {
      par_time += measure_time([num_points, threads]() {
        monte_carlo_pi::calculate_pi_parallel(num_points, threads);
      });
    }

    par_time /= num_runs;  // Average time
    // Parallel version should be faster with sufficient points
    EXPECT_LE(par_time, seq_time * 1.5); // Allow some overhead
  }
}

// Thread safety tests
TEST(MonteCarloPiTest, ThreadSafety) {
  const long long num_points = 1000000;
  const int num_threads = 4;
  std::vector<std::future<double>> futures;

  // Launch multiple calculations in parallel
  for (int i = 0; i < num_threads; ++i) {
    futures.push_back(std::async(std::launch::async, [num_points]() {
      return monte_carlo_pi::calculate_pi_parallel(num_points);
    }));
  }

  // Collect results
  std::vector<double> results;

  for (auto &future : futures) {
    results.push_back(future.get());
  }

  // All results should be valid
  for (double pi : results) {
    EXPECT_NEAR(pi, M_PI, 0.01);
  }
}

// Consistency tests
TEST(MonteCarloPiTest, ResultConsistency) {
  const long long num_points = 1000000;
  const int num_runs = 5;
  // Run multiple times and check consistency
  std::vector<double> seq_results;
  std::vector<double> par_results;

  for (int i = 0; i < num_runs; ++i) {
    seq_results.push_back(monte_carlo_pi::calculate_pi_sequential(num_points));
    par_results.push_back(monte_carlo_pi::calculate_pi_parallel(num_points));
  }

  // Calculate standard deviation
  double seq_mean = std::accumulate(seq_results.begin(), seq_results.end(), 0.0) / num_runs;
  double par_mean = std::accumulate(par_results.begin(), par_results.end(), 0.0) / num_runs;
  double seq_variance = 0.0;
  double par_variance = 0.0;

  for (double result : seq_results) {
    seq_variance += std::pow(result - seq_mean, 2);
  }

  for (double result : par_results) {
    par_variance += std::pow(result - par_mean, 2);
  }

  seq_variance /= num_runs;
  par_variance /= num_runs;
  // Results should be consistent (low variance)
  EXPECT_LT(std::sqrt(seq_variance), 0.01);
  EXPECT_LT(std::sqrt(par_variance), 0.01);
}

} // namespace
