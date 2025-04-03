#include <iostream>
#include "monte_carlo_pi.h"

int main() {
  int num_points = 1000000;
  double estimated_pi = MonteCarloPi::calculate(num_points);
  std::cout << "Estimated Pi Value: " << estimated_pi << std::endl;
  return 0;
}
