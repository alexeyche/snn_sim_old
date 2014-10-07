#ifndef LINEAR_H
#define LINEAR_H


#include "tuning_curves.h"


#include <snnlib/util/util.h>

typedef struct LinearTuningCurves {
    TuningCurves base;

    char *encoder;
    double *gain;
    double *bias;
} LinearTuningCurves;

LinearTuningCurves* init_LinearTuningCurves(const Constants *c);
double getResponse_LinearTuningCurves(struct TuningCurves *tc, size_t ni, double input);
void free_LinearTuningCurves(struct TuningCurves *);



#endif
