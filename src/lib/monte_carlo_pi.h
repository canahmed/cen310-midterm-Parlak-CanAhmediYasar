#ifndef MONTE_CARLO_PI_H
#define MONTE_CARLO_PI_H

#include <random>
#include <vector>
#include <cmath>

namespace monte_carlo_pi {

/**
 * @brief Calculates Pi using Monte Carlo method sequentially
 * @param num_points Number of points to generate
 * @return Calculated Pi value
 */
double calculate_pi_sequential(long long num_points);

/**
 * @brief Calculates Pi using Monte Carlo method with OpenMP
 * @param num_points Number of points to generate
 * @param num_threads Number of threads to use (0 for default)
 * @return Calculated Pi value
 */
double calculate_pi_parallel(long long num_points, int num_threads = 0);

/**
 * @brief Generates random points and counts points inside circle
 * @param num_points Number of points to generate
 * @return Pair of (points_inside_circle, total_points)
 */
std::pair<long long, long long> generate_points(long long num_points);

} // namespace monte_carlo_pi

#endif // MONTE_CARLO_PI_H 
