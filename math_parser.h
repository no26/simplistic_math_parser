#ifndef MATH_PARSER_H
#define MATH_PARSER_H

#include <math.h>
// Take a mathematical expression and calcul the result.
// operator accepted : ^ * / + - ( )
// this function return some error code :  -1 mathexpr or result is null
//                                          -2 internal malloc error
//                                          -3 Syntax error on mathexpr
// mathexpr : take a mathematical expression
// result   : take the result of mathexpr
int eval_str_to_bt(const char *mathexpr, double *result);

#endif
