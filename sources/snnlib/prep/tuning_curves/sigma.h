#ifndef SIGMA_H
#define SIGMA_H

#include "tuning_curves.h"

typedef struct SigmaTuningCurves {
    TuningCurves base;

} SigmaTuningCurves;


SigmaTuningCurves* init_SigmaTuningCurves(const Constants *c);
double getResponse_SigmaTuningCurves(struct TuningCurves *tc, size_t ni, double input);
void free_SigmaTuningCurves(struct TuningCurves *);


#endif
