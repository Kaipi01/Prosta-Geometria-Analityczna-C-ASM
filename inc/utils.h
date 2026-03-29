#include "draw_math.h"
#include "geo_math.h"
#include <stdarg.h>

#define FONT_SIZE 24
#define BUFF_SIZE 1024

typedef struct {
  char *file_name;
  SquareFunctionChart *charts;
  int charts_num;
  SquareFunctionCoefficients square_fun_coefs;
  Point A;
  Point B;
  Point C;
  Point S;
} ProgramEntryData;

Vector2 to_vec2(Point p);
void write_point(Point p, char label);
void write_circle_equation(EquationCircleValues values);
void export_data_to_txt_file(ProgramEntryData data);
int save_to_buff(const char *restrict format, ...);
char *repl_str(const char *str, const char *from, const char *to);