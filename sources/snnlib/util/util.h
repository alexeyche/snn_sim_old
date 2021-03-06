#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <math.h>

#include <snnlib/util/util_vector.h>
#ifndef PI
    #define PI 3.1415926535897932384626433832795028841971693993751
#endif

double getUnif();
double getUnifBetween(double low, double high);
double getExp(double rate);
double getNorm();
void shuffleIndVector(indVector *v);

#endif
