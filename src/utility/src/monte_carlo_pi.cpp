#include "monte_carlo_pi.h"
#include <cstdlib>  // rand(), srand()
#include <ctime>    // time()

double MonteCarloPi::calculate(int num_points) {
  int inside_circle = 0;
  srand(time(nullptr));

  for (int i = 0; i < num_points; i++) {
    double x = static_cast<double>(rand()) / RAND_MAX;
    double y = static_cast<double>(rand()) / RAND_MAX;

    if ((x * x + y * y) <= 1.0) {
      inside_circle++;
    }
  }

  return 4.0 * static_cast<double>(inside_circle) / num_points;
}
