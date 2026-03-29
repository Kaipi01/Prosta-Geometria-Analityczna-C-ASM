#include "../inc/geo_math.h"
#include <math.h>

Point calc_right_triangle_C_point(Point A, Point B) {
  if (B.x == A.x) {
    return (Point){NAN, NAN};
  }
  return (Point){A.x, B.y};
}

SlopeEquationValues calc_perpendicular_bisector(Point A, Point B) {
  Point S = calc_midpoint(A, B);
  SlopeEquationValues slope_equation = calc_line_equation(A, B);
  // a1 = -1 / a2
  // b = ys - a1 * xs
  double a = -1 / slope_equation.a;
  double b = S.y - a * S.x;

  return (SlopeEquationValues){a, b};
}

EquationCircleValues calc_circle_equation(Point A, Point B) {
  Point S = calc_midpoint(A, B);
  double r = calc_distance_of_two_points(A, S);

  return (EquationCircleValues){S.x, S.y, r};
}