

#include "sigma.h"


SigmaTuningCurves* init_SigmaTuningCurves(const Constants *c) {
    SigmaTuningCurves *tc = (SigmaTuningCurves*)malloc(sizeof(SigmaTuningCurves));
    tc->base.N = c->preproc->N;

    tc->centers = (doubleVector**) malloc( tc->base.N * sizeof(doubleVector*) );
    tc->sds = (doubleVector**) malloc( tc->base.N * sizeof(doubleVector*) );
    tc->gains = (doubleVector**) malloc( tc->base.N * sizeof(doubleVector*) );
    tc->encoder = (char *) malloc( tc->base.N * sizeof(char) );
    for(size_t ni=0; ni<tc->base.N; ni++) {
        if(getUnif() >= 0.5) {
            tc->encoder[ni] = -1;
        } else {
            tc->encoder[ni] = 1;
        }

        tc->centers[ni] = TEMPLATE(createVector,double)();
        tc->sds[ni] = TEMPLATE(createVector,double)();
        tc->gains[ni] = TEMPLATE(createVector,double)();
        for(size_t ci=0; ci < c->preproc->max_curve_num; ci++) {
            if((ci>0) && (c->preproc->prob_next_sigma < getUnif() )) break;
            TEMPLATE(insertVector,double)(tc->centers[ni], getUnifBetween(c->preproc->intercept_low, c->preproc->intercept_high));
            TEMPLATE(insertVector,double)(tc->sds[ni],     getUnifBetween(c->preproc->sigma_low, c->preproc->sigma_high));
            TEMPLATE(insertVector,double)(tc->gains[ni],   getUnifBetween(c->preproc->sigma_gain_low, c->preproc->sigma_gain_high));
        }
    }
    tc->base.getResponse = &getResponse_SigmaTuningCurves;
    tc->base.free = &free_SigmaTuningCurves;

    return(tc);
}

double getResponse_SigmaTuningCurves(struct TuningCurves *tc, size_t ni, double input) {
    SigmaTuningCurves *stc = (SigmaTuningCurves*) tc;

    double response = 0.0;
    for(size_t ci=0; ci < stc->centers[ni]->size; ci++) {
        response += stc->gains[ni]->array[ci] * exp( - (input - stc->centers[ni]->array[ci])*(input - stc->centers[ni]->array[ci]) / ( 2 * stc->sds[ni]->array[ci]*stc->sds[ni]->array[ci] ));
    }
    return(response);
}

void free_SigmaTuningCurves(struct TuningCurves *tc) {
    SigmaTuningCurves *stc = (SigmaTuningCurves*) tc;
    for(size_t ni=0; ni<stc->base.N; ni++) {
        TEMPLATE(deleteVector,double)(stc->centers[ni]);
        TEMPLATE(deleteVector,double)(stc->sds[ni]);
        TEMPLATE(deleteVector,double)(stc->gains[ni]);
    }
    free(stc->centers);
    free(stc->sds);
    free(stc->gains);
    free(stc);
}


