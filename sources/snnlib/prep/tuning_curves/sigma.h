#ifndef SIGMA_H
#define SIGMA_H

#include <snnlib/util/util.h>

#include "tuning_curves.h"

typedef struct SigmaTuningCurves {
    TuningCurves base;

    doubleVector **centers;
    doubleVector **sds;
    doubleVector **gains;
    char *encoder;
} SigmaTuningCurves;


SigmaTuningCurves* init_SigmaTuningCurves(const Constants *c);
double getResponse_SigmaTuningCurves(struct TuningCurves *tc, size_t ni, double input);
void free_SigmaTuningCurves(struct TuningCurves *);


#endif
