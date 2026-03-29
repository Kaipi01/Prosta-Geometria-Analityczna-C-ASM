#include "../inc/draw_math.h"
#include "../inc/raylib.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// zwróć wartość bezwględną
double abs_help(double x) {
  if (x < 0) {
    x *= -1;
  }
  return x;
}

double pow_help(int degree, double x) {
  if (degree == 0) {
    return 1;
  } else if (degree == 2) {
    return x * x;
  } else if (degree == 1) {
    return x;
  }
  if (degree % 2 != 0) {
    return x * pow_help(degree - 1, x);
  }
  double res = pow_help(degree / 2, x);
  return res * res;
}

double calculate_delta(double a, double b, double c) {
  // Δ = b^2-4ac
  return pow_help(2, b) - (4 * a * c);
}

Vector2 get_parabola_vertex(double delta, double a, double b) {
  double p = (-1 * b) / (2 * a);
  double q = (-1 * delta) / (4 * a);

  Vector2 vertex = {p, q};
  return vertex;
}

float square_function(SquareFunctionCoefficients coefs, float x) {
  return (coefs.a * x * x) + (coefs.b * x) + coefs.c;
}

float linear_function(float a, float x, float b) { return x * a + b; }

float val_to_screen(Constraints cons, float x) {
  Vector2 p1 = {x, 0};
  Vector2 p2 = {0, 0};

  Vector2 screen_p1 = map_to_screen(cons, p1);
  Vector2 screen_p2 = map_to_screen(cons, p2);

  return fabs(screen_p1.x - screen_p2.x);
}

Vector2 map_to_screen(Constraints cons, Vector2 p) {
  float width_half_pos = (cons.width / 2.0) + cons.start_p.x;
  float height_half_pos = (cons.height / 2.0) + cons.start_p.y;
  float val_x = p.x;
  float val_y = p.y;

  if (cons.scale != 0) {
    val_x = p.x * (cons.width / (float)cons.scale);
    val_y = p.y * (cons.height / (float)cons.scale);
  }
  Vector2 screen_p = {width_half_pos + val_x, height_half_pos - val_y};

  return screen_p;
}

SquareFunctionChart new_square_fun_chart(SquareFunctionCoefficients coefs,
                                         char *label, Color color) {
  double delta = calculate_delta(coefs.a, coefs.b, coefs.c);
  Vector2 vertex = get_parabola_vertex(delta, coefs.a, coefs.b);
  SquareFunctionChart square_fun_chart = {square_function, vertex, coefs, color,
                                          delta};
  square_fun_chart.label = label;
  return square_fun_chart;
}

Button new_button(Vector2 pos, float width, float height, int font_size,
                  Color color, char *text) {
  Rectangle bounds = {pos.x, pos.y - height, width, height};
  return (Button){pos, width, height, font_size, bounds, color, text, 0, false};
}

void DrawButton(Button btn) {
  int text_len = strlen(btn.text);

  DrawRectangleRec(btn.bounds, btn.color);
  DrawText(btn.text,
           btn.bounds.x + (btn.width / 2.0) - (btn.font_size + text_len),
           btn.bounds.y + (btn.height / 2.0) - (btn.font_size / 2.0),
           btn.font_size, BLACK);
}

bool CheckButtonClick(Button btn, Vector2 mouse_position) {
  btn.action_state = false;
  mouse_position = GetMousePosition();

  if (CheckCollisionPointRec(mouse_position, btn.bounds)) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      btn.state = 2; // stan PRESSED
    } else {
      btn.state = 1; // stan MOUSE_HOVER
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      btn.action_state = true;
    }
  } else {
    btn.state = 0;
  }
  return btn.action_state;
}

void DrawLabeledPoint(Constraints cons, Vector2 p, char *label, Color color) {
  Vector2 desc_p = {p.x + (cons.scale / 100.0), p.y + (cons.scale / 100.0)};

  Vector2 screen_p = map_to_screen(cons, p);
  Vector2 screen_desc_p = map_to_screen(cons, desc_p);

  DrawCircleV(screen_p, 4, color);
  DrawText(label, screen_desc_p.x, screen_desc_p.y, 20, color);
}

void DrawLinearFunctionChart(Constraints cons, LinearFunctionChart fun) {
  LinearFunction f = fun.function;
  Vector2 start_p = {-1 * cons.scale, f(fun.a, -1 * cons.scale, fun.b)};
  Vector2 end_p = {cons.scale, f(fun.a, cons.scale, fun.b)};
  Vector2 start_p_screen = map_to_screen(cons, start_p);
  Vector2 end_p_screen = map_to_screen(cons, end_p);

  DrawLineV(start_p_screen, end_p_screen, fun.color);
}

void DrawSquareFunctionChart(Constraints cons, SquareFunctionChart square_fun) {
  SquareFunction f = square_fun.function;
  SquareFunctionCoefficients coefs = square_fun.coefficients;

  DrawLabeledPoint(cons, square_fun.vertex, square_fun.label, square_fun.color);

  for (float i = (-1 * cons.scale); i < cons.scale; i += 0.01) {
    Vector2 vec = {i, f(coefs, i)};
    Vector2 vec_screen = map_to_screen(cons, vec);

    if (abs_help(vec_screen.x) <= cons.width &&
        abs_help(vec_screen.y) <= cons.height) {

      DrawCircleV(vec_screen, 1, square_fun.color);
    }
  }
}

void DrawChartAxisLines(Constraints cons, Color main_axis_color,
                        Color lines_color) {
  int width = cons.width;
  int height = cons.height;
  int start_p_x = cons.start_p.x;
  int start_p_y = cons.start_p.y;

  int width_half_pos = (width / 2) + start_p_x;
  int height_half_pos = (height / 2) + start_p_y;

  int scale_x = height / cons.scale;
  int scale_y = width / cons.scale;

  DrawText("0", width_half_pos + 5, height_half_pos + 5, 14, main_axis_color);

  // Oś X
  for (int i = 0; i < height; i += scale_x) {
    DrawLine(start_p_x, i, start_p_x + width, i, lines_color);
  }
  DrawLine(start_p_x, height_half_pos, start_p_x + width, height_half_pos,
           main_axis_color);

  // Oś Y
  for (int i = 0; i < width; i += scale_y) {
    DrawLine(i, start_p_y, i, start_p_y + height, lines_color);
  }
  DrawLine(width_half_pos, start_p_y, width_half_pos, start_p_y + height,
           main_axis_color);
}

void DrawFunctionsChartsBox(Constraints cons, SquareFunctionChart *charts,
                            int charts_number) {
  DrawChartAxisLines(cons, WHITE, DARKGRAY);

  for (int i = 0; i < charts_number; i++) {
    DrawSquareFunctionChart(cons, charts[i]);
  }
}

void DrawCustomTriangle(Vector2 A, Vector2 B, Vector2 C, Color color) {
  DrawLineV(A, B, color);
  DrawLineV(B, C, color);
  DrawLineV(A, C, color);
}

Vector2 DrawFunctionPoint(Constraints cons, Vector2 p, Color color) {
  Vector2 vec_screen = map_to_screen(cons, p);

  if (abs_help(vec_screen.x) <= cons.width &&
      abs_help(vec_screen.y) <= cons.height) {

    DrawCircleV(vec_screen, 1, color);
  }
  return vec_screen;
}

void DrawCustomFunctionChart(Constraints cons, CustomFunctionChart fun) {
  CustomFunction f = fun.function;
  Vector2 prev_point = {0, 0};

  for (float i = (-1 * cons.scale); i < cons.scale; i += DRAW_PRECISION) {
    Vector2 vec = {i, f(i, fun.function_args)};

    if (vec.x >= fun.domain.min_x && vec.x <= fun.domain.max_x &&
        vec.y >= fun.domain.min_y && vec.y <= fun.domain.max_y) {
      Vector2 vec_screen = DrawFunctionPoint(cons, vec, fun.color);

      if (prev_point.x != 0 && prev_point.y != 0) {
        DrawLineV(prev_point, vec_screen, fun.color);
      }

      prev_point = vec_screen;
    }
  }
}

bool string_has_char(char *str, char c) {
  bool has_char = false;
  int len = strlen(str);

  for (int i = 0; i < len; i++) {
    if (str[i] == c) {
      has_char = true;
      break;
    }
  }
  return has_char;
}

char *get_str_numbers_from_expression(char *str) {
  int len = strlen(str);
  bool has_char_x = false;
  char *result = malloc(len + 1); // +1 na '\0'
  if (!result)
    return NULL;

  int j = 0;
  for (int i = 0; i < len; i++) {
    if (str[i] == 'x') {
      has_char_x = true;
    }
    if (str[i] == '^') {
      break;
    }
    if (str[i] != ' ' && str[i] != '+' && str[i] != 'x') {
      result[j++] = str[i];
    }
  }
  result[j] = '\0';

  if (strcmp(result, "") == 0 && has_char_x) {
    free(result);
    result = "1";
  } else if (strcmp(result, "") == 0) {
    free(result);
    result = "0";
  } else if (strcmp(result, "-") == 0) {
    free(result);
    result = "-1";
  }

  return result;
}

SquareFunctionCoefficients parse_square_function_string(char *fun_str) {
  int fun_str_len = strlen(fun_str);
  assert(fun_str_len >= 3);
  double a = 0, b = 0, c = 0;

  // f(x) = ax^2 + bx + c
  char *terms[3] = {"", "", ""};
  int operators_pos[2] = {-1, -1};
  int expression_terms_num = 0;

  for (int i = 0; i < fun_str_len; i++) {
    if (i != 0 && (fun_str[i] == '-' || fun_str[i] == '+')) {
      operators_pos[expression_terms_num] = i;
      expression_terms_num++;
    }
  }

  if (operators_pos[0] != -1) {
    char buf_0[32];
    char buf_1[32];

    terms[0] = strncpy(buf_0, fun_str, operators_pos[0]);
    terms[0][operators_pos[0]] = '\0';

    if (operators_pos[1] != -1) {
      char buf_2[32];
      int n_1 = abs_help(operators_pos[0] - operators_pos[1]);
      int n_2 = fun_str_len - operators_pos[1];

      terms[1] = strncpy(buf_1, fun_str + operators_pos[0], n_1);
      terms[1][n_1] = '\0';
      terms[2] = strncpy(buf_2, fun_str + operators_pos[1], n_2);
      terms[2][n_2] = '\0';
    } else {
      int n_1 = fun_str_len - operators_pos[0];

      terms[1] = strncpy(buf_1, fun_str + operators_pos[0], n_1);
      terms[1][n_1] = '\0';
    }
  } else {
    terms[0] = fun_str;
  }

  a = atof(get_str_numbers_from_expression(terms[0]));

  if (string_has_char(terms[1], 'x')) {
    b = atof(get_str_numbers_from_expression(terms[1]));
    c = atof(get_str_numbers_from_expression(terms[2]));
  } else {
    c = atof(get_str_numbers_from_expression(terms[1]));
  }
  SquareFunctionCoefficients coefficients = {a, b, c};

  return coefficients;
}