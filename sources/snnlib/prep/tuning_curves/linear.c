
#include "linear.h"


LinearTuningCurves* init_LinearTuningCurves(const Constants *c) {
    LinearTuningCurves *tc = (LinearTuningCurves*)malloc(sizeof(LinearTuningCurves));
    tc->base.N = c->preproc->N;
    
    tc->encoder = (char *) malloc( tc->base.N * sizeof(char) );
    tc->gain = (double *) malloc( tc->base.N * sizeof(double) );
    tc->bias = (double *) malloc( tc->base.N * sizeof(double) );

    for(size_t ni=0; ni<tc->base.N; ni++) {
        if(getUnif() >= 0.5) {
            tc->encoder[ni] = -1;
        } else {
            tc->encoder[ni] = 1;
        }
        double intercept = getUnifBetween(c->preproc->intercept_low, c->preproc->intercept_high);
        double rate = getUnifBetween(c->preproc->rate_low, c->preproc->rate_high);
        double z = 1.0/(1-exp( (c->adex->t_ref/1000.0 -1.0/rate)/(c->adex->C/1000.0) ));
        tc->gain[ni] = (1.0-z)/(intercept-1.0);
        tc->bias[ni] = 1 - tc->gain[ni]*intercept;
    }

    tc->base.getResponse = &getResponse_LinearTuningCurves;
    tc->base.free = &free_LinearTuningCurves;
    return(tc);
}

double getResponse_LinearTuningCurves(struct TuningCurves *tc, size_t ni, double input) {
    LinearTuningCurves *ltc = (LinearTuningCurves*) tc;
    double resp = ltc->bias[ni] + ltc->gain[ni] * ltc->encoder[ni] * input;
    return(resp);
}

void free_LinearTuningCurves(struct TuningCurves *tc) {
    free(tc);
}


