#include "../header/teamManager.h"
#include <stdexcept>

using namespace Coruh::teamManager;

double teamManager::add(double a, double b) {
  return a + b;
}

double teamManager::subtract(double a, double b) {
  return a - b;
}

double teamManager::multiply(double a, double b) {
  return a * b;
}

double teamManager::divide(double a, double b) {
  if (b == 0) {
    throw std::invalid_argument("Division by zero is not allowed.");
  }

  return a / b;
}
