#ifndef _PYGAME_COMPLEX_EXTENSIONS_
#define _PYGAME_COMPLEX_EXTENSIONS_


#include <Python.h>
#include <math.h>

#define ZERO_EPSILON 1e-6
#define RELATIVE_ZERO 1e-6

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif


typedef Py_complex	pgVector2;
#define PG_Set_Vector2(vec, x, y) {(vec).real = x; (vec).imag = y;}

int is_zero(double num);
int is_equal(double a, double b);
int less_equal(double a, double b);
int more_equal(double a, double b);

#define MAX(x, y) ( ((x) > (y)) ? (x) : (y) )
#define MIN(x, y) ( ((x) < (y)) ? (x) : (y) )


double c_get_length_square(pgVector2 c);
double c_get_length(pgVector2 c);
Py_complex c_mul_complex_with_real(pgVector2 c,double d);
Py_complex c_div_complex_with_real(pgVector2 c,double d);
void	c_normalize(pgVector2* pVec);
double c_dot(pgVector2 a,pgVector2 b);
double c_cross(pgVector2 a, pgVector2 b);
pgVector2 c_fcross(double a, pgVector2 b);
pgVector2 c_crossf(pgVector2 a, double b);
void c_rotate(pgVector2* a, double seta);
int c_equal(pgVector2* a, pgVector2* b);
pgVector2 c_project(pgVector2 l,pgVector2 p);

#endif

