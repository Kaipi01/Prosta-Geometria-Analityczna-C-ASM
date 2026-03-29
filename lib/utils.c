#include "../inc/utils.h"
#include "../inc/save_file.h"
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char buff[BUFF_SIZE];
static int buff_offset = 0;

Vector2 to_vec2(Point p) { return (Vector2){p.x, p.y}; }

int save_to_buff(const char *restrict format, ...) {
  va_list args;
  va_start(args, format);
  int written_bytes = vsnprintf(buff + buff_offset, BUFF_SIZE, format, args);
  va_end(args);

  if (written_bytes >= 0) {
    buff_offset += written_bytes;
  }
  return written_bytes;
}
void write_point(Point p, char label) {
  save_to_buff("%c = (%0.2f; %0.2f)\n\n", label, p.x, p.y);
}
void write_circle_equation(EquationCircleValues values) {
  double r_pow2 = pow(values.r, 2);
  double a = fabs(values.a);
  double b = fabs(values.b);

  if (values.a < 0) {
    save_to_buff("(x + %0.2f)^2 + ", a);
  } else {
    save_to_buff("(x - %0.2f)^2 + ", a);
  }
  if (values.b < 0) {
    save_to_buff("(y + %0.2f)^2 ", b);
  } else {
    save_to_buff("(y - %0.2f)^2 ", b);
  }
  save_to_buff("= %0.2f\n\n", r_pow2);
}
void write_line_equation(SlopeEquationValues equation_vals) {
  if (equation_vals.b == 0) {
    save_to_buff("y = %0.2fx\n\n", equation_vals.a);
  } else {
    save_to_buff("y = %0.2fx %c %0.2f\n\n", equation_vals.a,
                 equation_vals.b < 0 ? '-' : '+', fabs(equation_vals.b));
  }
}
void export_data_to_txt_file(ProgramEntryData data) {

  double AB_distance = calc_distance_of_two_points(data.A, data.B);
  double AC_distance = calc_distance_of_two_points(data.A, data.C);
  double BC_distance = calc_distance_of_two_points(data.B, data.C);

  EquationCircleValues circle_equation = calc_circle_equation(data.A, data.B);

  write_point(data.A, 'A');
  write_point(data.B, 'B');
  write_point(data.C, 'C');
  save_to_buff("Równanie prostej |AB|: ");
  write_line_equation(calc_line_equation(data.A, data.B));

  save_to_buff("Długość odcinka |AB| = %0.2f\n\n", AB_distance);
  save_to_buff("Środek odcinka |AB|: S = (%0.2f; %0.2f)\n\n", data.S.x,
               data.S.y);
  save_to_buff("Równanie symetralnej odcinka |AB|: ");
  write_line_equation(calc_perpendicular_bisector(data.A, data.B));
  save_to_buff("Obwód trójkąta ABC: %0.2f\n\n",
               calc_triangle_perimeter(AB_distance, AC_distance, BC_distance));
  save_to_buff("Pole trójkąta ABC: %0.2f\n\n",
               calc_triangle_area(AC_distance, BC_distance));
  write_circle_equation(circle_equation);
  save_to_buff("Promień okręgu o środku S: r = %0.2f\n\n", circle_equation.r);
  save_to_buff(
      "############################################################\n\n");
  double a = data.square_fun_coefs.a;
  double b = data.square_fun_coefs.b;
  double c = data.square_fun_coefs.c;

  save_to_buff("Współczynniki funkcji kwadratowej wynoszą:\n");
  save_to_buff("a: %f     b: %f     c: %f\n", a, b, c);
  double delta = calculate_delta(a, b, c);

  save_to_buff("Δ: %f\n", delta);

  if (delta >= 0) {
    double delta_sqrt = sqrt(delta);
    save_to_buff("√Δ: %f\n", delta_sqrt);

    if (delta == 0) {
      double x0;
      save_to_buff("Δ == 0\n");
      save_to_buff("Funkcja kwadratowa posiada jedno miejsce zerowe\n");
      x0 = (-1 * b) / (2 * a);

      save_to_buff("x0 = %f\n", x0);
    } else {
      double x1 = ((-1 * b) - delta_sqrt) / (2 * a);
      double x2 = ((-1 * b) + delta_sqrt) / (2 * a);
      save_to_buff("Δ > 0\n");
      save_to_buff("Funkcja kwadratowa posiada dwa miejsca zerowe\n\n");
      save_to_buff("x1 = %f           x2 = %f\n", x1, x2);
    }

  } else {
    save_to_buff("Δ < 0\n");
    save_to_buff("Funkcja kwadratowa nie posiada miejsc zerowych\n");
  }

  if (a > 0) {
    save_to_buff("Ramiona paraboli są skierowane do góry\n\n");
  } else {
    save_to_buff("Ramiona paraboli są skierowane w dół\n");
  }
  double p = (-1 * b) / (2 * a);
  double q = (-1 * delta) / (4 * a);

  save_to_buff("Oś symetrii paraboli jest dana równaniem:  %f\n", p);
  save_to_buff(
      "Punkt przecięcia paraboli z osią y-ów, ma współrzędne:  (0,%f)\n\n", c);
  save_to_buff("Współrzędne wieszchołka paraboli wynoszą\n");
  save_to_buff("W = (%f, %f)\n", p, q);

  buff[BUFF_SIZE - 1] = '\0';
  printf("%s\n", buff);
  save_file(data.file_name, buff, buff_offset);
}

// code from: https://creativeandcritical.net/str-replace-c
// author: Laird Shaw
char *repl_str(const char *str, const char *from, const char *to) {
  size_t cache_sz_inc = 16;
  const size_t cache_sz_inc_factor = 3;
  const size_t cache_sz_inc_max = 1048576;

  char *pret, *ret = NULL;
  const char *pstr2, *pstr = str;
  size_t i, count = 0;
#if (__STDC_VERSION__ >= 199901L)
  uintptr_t *pos_cache_tmp, *pos_cache = NULL;
#else
  ptrdiff_t *pos_cache_tmp, *pos_cache = NULL;
#endif
  size_t cache_sz = 0;
  size_t cpylen, orglen, retlen, tolen, fromlen = strlen(from);

  while ((pstr2 = strstr(pstr, from)) != NULL) {
    count++;

    if (cache_sz < count) {
      cache_sz += cache_sz_inc;
      pos_cache_tmp = realloc(pos_cache, sizeof(*pos_cache) * cache_sz);
      if (pos_cache_tmp == NULL) {
        goto end_repl_str;
      } else
        pos_cache = pos_cache_tmp;
      cache_sz_inc *= cache_sz_inc_factor;
      if (cache_sz_inc > cache_sz_inc_max) {
        cache_sz_inc = cache_sz_inc_max;
      }
    }

    pos_cache[count - 1] = pstr2 - str;
    pstr = pstr2 + fromlen;
  }

  orglen = pstr - str + strlen(pstr);

  if (count > 0) {
    tolen = strlen(to);
    retlen = orglen + (tolen - fromlen) * count;
  } else
    retlen = orglen;
  ret = malloc(retlen + 1);
  if (ret == NULL) {
    goto end_repl_str;
  }

  if (count == 0) {
    strcpy(ret, str);
  } else {
    pret = ret;
    memcpy(pret, str, pos_cache[0]);
    pret += pos_cache[0];
    for (i = 0; i < count; i++) {
      memcpy(pret, to, tolen);
      pret += tolen;
      pstr = str + pos_cache[i] + fromlen;
      cpylen =
          (i == count - 1 ? orglen : pos_cache[i + 1]) - pos_cache[i] - fromlen;
      memcpy(pret, pstr, cpylen);
      pret += cpylen;
    }
    ret[retlen] = '\0';
  }

end_repl_str:
  free(pos_cache);
  return ret;
}