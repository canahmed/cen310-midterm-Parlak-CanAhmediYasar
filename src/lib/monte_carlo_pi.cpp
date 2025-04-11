#include "monte_carlo_pi.h"
#include <omp.h>
#include <random>
#include <chrono>
#include <stdexcept>

namespace monte_carlo_pi {

std::pair<long long, long long> generate_points(long long num_points) {
  // Handle edge cases
  if (num_points <= 0) {
    return {0, 0};
  }

  // Use current time as seed for random number generator
  auto seed_value = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 generator(static_cast<std::mt19937::result_type>(seed_value));
  std::uniform_real_distribution<double> distribution(-1.0, 1.0);
  long long points_inside = 0;
  long long total_points = num_points;

  for (long long i = 0; i < num_points; ++i) {
    double x = distribution(generator);
    double y = distribution(generator);

    // Check if point lies inside the circle (x^2 + y^2 <= 1)
    if (x*x + y*y <= 1.0) {
      points_inside++;
    }
  }

  return {points_inside, total_points};
}

double calculate_pi_sequential(long long num_points) {
  if (num_points <= 0) {
    return 0.0;
  }

  auto [points_inside, total_points] = generate_points(num_points);

  if (total_points == 0) {
    return 0.0;
  }

  return 4.0 * points_inside / total_points;
}

double calculate_pi_parallel(long long num_points, int num_threads) {
  if (num_points <= 0) {
    return 0.0;
  }

  if (num_threads > 0) {
    omp_set_num_threads(num_threads);
  }

  long long points_inside = 0;
  long long total_points = num_points;
  #pragma omp parallel
  {
    // Her thread için benzersiz bir seed oluştur
    auto now = std::chrono::high_resolution_clock::now();
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    unsigned seed = static_cast<unsigned>(nanos) + omp_get_thread_num() * 1000;
    std::mt19937 generator(seed);
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);
    long long local_points_inside = 0;
    #pragma omp for

    for (long long i = 0; i < num_points; ++i) {
      double x = distribution(generator);
      double y = distribution(generator);

      if (x*x + y*y <= 1.0) {
        local_points_inside++;
      }
    }

    #pragma omp atomic
    points_inside += local_points_inside;
  }

  if (total_points == 0) {
    return 0.0;
  }

  return 4.0 * points_inside / total_points;
}

} // namespace monte_carlo_pi
