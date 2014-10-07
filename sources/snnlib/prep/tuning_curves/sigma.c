

#include "sigma.h"


SigmaTuningCurves* init_SigmaTuningCurves(const Constants *c) {
    SigmaTuningCurves *tc = (SigmaTuningCurves*)malloc(sizeof(SigmaTuningCurves));
    tc->base.getResponse = &getResponse_SigmaTuningCurves;
    tc->base.free = &free_SigmaTuningCurves;
    return(tc);
}

double getResponse_SigmaTuningCurves(struct TuningCurves *tc, size_t ni, double input) {
}

void free_SigmaTuningCurves(struct TuningCurves *tc) {
    free(tc);
}


