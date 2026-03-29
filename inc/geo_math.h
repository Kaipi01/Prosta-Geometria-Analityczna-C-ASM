typedef struct {
  double x;
  double y;
} Point;

typedef struct {
  double a;
  double b;
} SlopeEquationValues;

typedef struct {
  double a;
  double b;
  double r;
} EquationCircleValues;

//  Function calculate length of the segment | AB |
double calc_distance_of_two_points(Point A, Point B);
// Function calculate coordinates of point S,
// which lies in the middle of the segment |AB|
Point calc_midpoint(Point A, Point B);
// Function calculate equation of the line passing through points A and B
SlopeEquationValues calc_line_equation(Point A, Point B);
// Function calculate equation of the perpendicular bisector of the segment AB
SlopeEquationValues calc_perpendicular_bisector(Point A, Point B);
double calc_triangle_perimeter(double a, double b, double c);
double calc_triangle_area(double a, double h);
Point calc_right_triangle_C_point(Point A, Point B);
EquationCircleValues calc_circle_equation(Point A, Point B);