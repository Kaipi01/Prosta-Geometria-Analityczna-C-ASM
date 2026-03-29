#include "./inc/parse_str_point.h"
#include "./inc/utils.h"
#include "inc/raylib.h"
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 900

void DrawMainView(Constraints cons, ProgramEntryData data, Button zoom_out_btn,
                  Button zoom_in_btn, Button export_btn) {

  Vector2 A = to_vec2(data.A);
  Vector2 B = to_vec2(data.B);
  Vector2 C = to_vec2(data.C);
  Vector2 S = to_vec2(data.S);
  Vector2 A_screen = map_to_screen(cons, A);
  Vector2 B_screen = map_to_screen(cons, B);
  Vector2 C_screen = map_to_screen(cons, C);
  Vector2 S_screen = map_to_screen(cons, S);

  ClearBackground(BLACK);
  DrawFPS(20, 20);
  DrawFunctionsChartsBox(cons, data.charts, 1);
  DrawLabeledPoint(cons, A, "A", RED);
  DrawLabeledPoint(cons, B, "B", BLUE);
  DrawLabeledPoint(cons, C, "C", GREEN);
  DrawLabeledPoint(cons, S, "S", YELLOW);

  DrawCustomTriangle(A_screen, C_screen, B_screen, PINK);

  SlopeEquationValues equation_vals =
      calc_perpendicular_bisector(data.A, data.B);

  double r = val_to_screen(cons, calc_distance_of_two_points(data.A, data.S));

  DrawCircleLinesV(S_screen, r, WHITE);

  DrawLinearFunctionChart(cons, (LinearFunctionChart){linear_function,
                                                      equation_vals.a,
                                                      equation_vals.b, PURPLE});

  DrawButton(zoom_out_btn);
  DrawButton(zoom_in_btn);
  DrawButton(export_btn);
}

int main(int argc, char **argv) {

  if (argc < 5) {
    printf("Użycie: ./main \"NAZWA PLIKU\" \"FUNKCJA KWADRATOWA\" \"A = (X; "
           "Y)\" \"B "
           "= (X; Y)\"\n");
    printf("Przykład: ./main \"test.txt\" \"0.5x^2+x-6\" \"A = "
           "(-3; 0)\" \"B "
           "= (-1; 3)\"\n");
    return 1;
  }
  char *file_name = argv[1];
  char *square_fun_str = argv[2];
  char *point_A_str = argv[3];
  char *point_B_str = argv[4];

  Constraints chart_box_cons = {WINDOW_HEIGHT, WINDOW_HEIGHT, 0, 0, 30};
  Vector2 mouse_position = {0.0f, 0.0f};
  int btn_margin = 10;
  int btn_height = 55;
  int btn_width = 80;
  int btn_y = WINDOW_HEIGHT - btn_margin;

  Vector2 zoom_out_btn_pos = {btn_margin, btn_y};
  Vector2 zoom_in_btn_pos = {btn_width + (2 * btn_margin), btn_y};
  Vector2 export_btn_pos = {(2 * btn_width) + (3 * btn_margin), btn_y};

  Button zoom_out_btn = new_button(zoom_out_btn_pos, btn_width, btn_height,
                                   FONT_SIZE, LIGHTGRAY, "-10%");
  Button zoom_in_btn = new_button(zoom_in_btn_pos, btn_width, btn_height,
                                  FONT_SIZE, LIGHTGRAY, "+10%");
  Button export_btn =
      new_button(export_btn_pos, 100, btn_height, 20, LIGHTGRAY, "Export");

  SquareFunctionChart charts[1];

  SquareFunctionCoefficients square_fun_coefs =
      parse_square_function_string(square_fun_str);

  charts[0] = new_square_fun_chart(square_fun_coefs, square_fun_str, ORANGE);

  Point A = {parse_str_point(point_A_str, 'x'),
             parse_str_point(point_A_str, 'y')};
  Point B = {parse_str_point(point_B_str, 'x'),
             parse_str_point(point_B_str, 'y')};
  Point C = calc_right_triangle_C_point(A, B);
  Point S = calc_midpoint(A, B);

  ProgramEntryData data = {file_name, charts, 1, square_fun_coefs, A, B, C, S};

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Geometria Analityczna");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    mouse_position = GetMousePosition();

    // Save results to file
    if (CheckButtonClick(export_btn, mouse_position)) {
      TakeScreenshot(repl_str(file_name, ".txt", ".png"));
      export_data_to_txt_file(data);
    }
    if (CheckButtonClick(zoom_out_btn, mouse_position) &&
        chart_box_cons.scale + 10 < 200) {
      chart_box_cons.scale += 10;
      DrawMainView(chart_box_cons, data, zoom_out_btn, zoom_in_btn, export_btn);
    }
    if (CheckButtonClick(zoom_in_btn, mouse_position) &&
        chart_box_cons.scale - 10 > 0) {
      chart_box_cons.scale -= 10;
      DrawMainView(chart_box_cons, data, zoom_out_btn, zoom_in_btn, export_btn);
    }
    BeginDrawing();
    DrawMainView(chart_box_cons, data, zoom_out_btn, zoom_in_btn, export_btn);
    EndDrawing();
  }
  CloseWindow();

  return 0;
}
