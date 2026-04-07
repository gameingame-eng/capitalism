#include <cmath>

inline float ease_in_out_quadratic(float t) {
  return t < 0.5 ? 2 * t * t : 1 - std::pow(-2 * t + 2, 2) / 2;
}

inline float ease_in_out_cubic(float t) {
  return t < 0.5 ? 4 * t * t * t : 1 - std::pow(-2 * t + 2, 3) / 2;
}
