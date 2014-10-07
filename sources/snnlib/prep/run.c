

#include "run.h"

TuningCurves* initTuningCurves(const Constants *c) {
    if(c->preproc->tc == ESigmaTC) {
        return( (TuningCurves*) init_SigmaTuningCurves(c) );
    } else
    if(c->preproc->tc == ELinearTC) {
        return( (TuningCurves*) init_LinearTuningCurves(c) );
    }
}

doubleVector* computeResponse(TuningCurves *tc, double x, double time_limit, const Constants *c) {
    AdExLayer *l = createAdExLayer(tc->N, false);
    SpikesList *sl = createSpikesList(tc->N);
    toStartValuesAdExLayer(l, c);

    for(double t=0; t<time_limit; t+=c->preproc->dt) {
        for(size_t ni=0; ni < l->N; ni++) {
            double input_value = tc->getResponse(tc, ni, x);
//            printf("%f->%f\n", x, input_value);
            simulateAdExLayerNeuron(l, &ni, &input_value, c);
            if(l->fired[ni] == 1) {
                TEMPLATE(insertVector,double)(sl->list[ni], t);
                l->fired[ni] = 0;
            }
        }
    }    
    doubleVector *resp = TEMPLATE(createVector,double)();
    for(size_t ni=0; ni<tc->N; ni++) {
        TEMPLATE(insertVector,double)(resp, (double)(sl->list[ni]->size)/(time_limit));
    }
    return(resp);
}

SpikePatternsList* runNeurons(AdExLayer *l, TuningCurves *tc, pMatrixVector *ts_data, doubleVector *ts_labels, Constants *c) {
    indVector *ts_indices = TEMPLATE(createVector,ind)();
    for(size_t ti=0; ti< ts_data->size; ti++) {
        TEMPLATE(insertVector,ind)(ts_indices, ti);    
    }
    srand(time(NULL));
    shuffleIndVector(ts_indices);

    double t = 0.0; 
    SpikePatternsList *spl = createSpikePatternsList(c->preproc->N);
    
    for(size_t ts_i=0; ts_i < ts_indices->size; ts_i++) {
        doubleVector *ts = doubleVectorFromMatrix(ts_data->array[ ts_indices->array[ts_i] ]);
        toStartValuesAdExLayer(l, c);
        size_t j;
        for(j = 0; j < ts->size; t+= c->preproc->dt, j++) {
            double I = ts->array[j];
            for(size_t ni=0; ni < l->N; ni++) {
                double input_value = tc->getResponse(tc, ni, I);
                simulateAdExLayerNeuron(l, &ni, &input_value, c);
                if(l->fired[ni] == 1) {
                    TEMPLATE(insertVector,double)(spl->sl->list[ni], t);
                    l->fired[ni] = 0;
                }
            }
        }
        TEMPLATE(insertVector,double)(spl->timeline, t);
        TEMPLATE(insertVector,double)(spl->pattern_classes, ts_labels->array[ts_indices->array[ts_i] ]);
    }
    spl->sl->Tmax = t;
    spl->sl->dt = c->preproc->dt;
    return(spl);
}


