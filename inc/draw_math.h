#include "raylib.h"
#include <math.h>

#define DRAW_PRECISION 0.01

#define INFINITY_DOMAIN                                                        \
  ((FunctionDomain){INFINITY, -INFINITY, INFINITY, -INFINITY})

typedef struct {
  float max_x;
  float min_x;
  float max_y;
  float min_y;
} FunctionDomain;

typedef struct {
  double a;
  double b;
  double c;
} SquareFunctionCoefficients;

typedef float (*CustomFunction)(float x, void *args);

typedef struct {
  Vector2 pos;
  float width;
  float height;
  int font_size;
  Rectangle bounds;
  Color color;
  char *text;
  int state;
  bool action_state;
} Button;

typedef struct {
  CustomFunction function;
  void *function_args;
  FunctionDomain domain;
  Color color;
} CustomFunctionChart;

typedef float (*SquareFunction)(SquareFunctionCoefficients coefficients,
                                float x);

typedef float (*LinearFunction)(float a, float x, float b);

typedef struct {
  SquareFunction function;
  Vector2 vertex;
  SquareFunctionCoefficients coefficients;
  Color color;
  double delta;
  char *label;
} SquareFunctionChart;

typedef struct {
  LinearFunction function;
  float a;
  float b;
  Color color;
} LinearFunctionChart;

typedef struct {
  int width;
  int height;
  Vector2 start_p;
  int scale;
} Constraints;

SquareFunctionCoefficients parse_square_function_string(char *fun_str);

double abs_help(double x);
double pow_help(int degree, double x);
double calculate_delta(double a, double b, double c);
Vector2 get_parabola_vertex(double delta, double a, double b);
float square_function(SquareFunctionCoefficients coefs, float x);
float linear_function(float a, float x, float b);
Vector2 map_to_screen(Constraints cons, Vector2 p);
SquareFunctionChart new_square_fun_chart(SquareFunctionCoefficients coefs,
                                         char *label, Color color);
void DrawLabeledPoint(Constraints cons, Vector2 p, char *label, Color color);
void DrawLinearFunctionChart(Constraints cons, LinearFunctionChart fun);
void DrawSquareFunctionChart(Constraints cons, SquareFunctionChart square_fun);
void DrawChartAxisLines(Constraints cons, Color main_axis_color,
                        Color lines_color);
void DrawFunctionsChartsBox(Constraints cons, SquareFunctionChart *charts,
                            int charts_number);
Button new_button(Vector2 pos, float width, float height, int font_size,
                  Color color, char *text);
void DrawButton(Button btn);
bool CheckButtonClick(Button btn, Vector2 mouse_position);
Vector2 DrawFunctionPoint(Constraints cons, Vector2 p, Color color);
void DrawCustomFunctionChart(Constraints cons, CustomFunctionChart fun);
void DrawCustomTriangle(Vector2 A, Vector2 B, Vector2 C, Color color);
float val_to_screen(Constraints cons, float x);