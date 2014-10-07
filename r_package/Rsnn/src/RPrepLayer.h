#ifndef RPREP_LAYER_H
#define RPREP_LAYER_H

#include <Rcpp.h>

#include "RConstants.h"
#include "conv_util.h"

extern "C" {
    #include <snnlib/prep/run.h>
}



class RPrepLayer {
public:    
    RPrepLayer(RConstants *rc_, bool saveStat_, size_t nthreads_) : saveStat(saveStat_), nthreads(nthreads_), rc(rc_) {
        tc = initTuningCurves(rc->c);
        l = createAdExLayer(rc->c->preproc->N, saveStat);
    }
    ~RPrepLayer() {
        tc->free(tc);
        deleteAdExLayer(l);
    }

    Rcpp::List run(Rcpp::List ts_data, Rcpp::NumericVector ts_labels) {
        doubleVector *ts_labels_snn = RNumericVectorToDoubleVector(ts_labels);        
        pMatrixVector *ts_data_snn = RListToMatrixVector(ts_data);

        SpikePatternsList* spl = runNeurons(l, tc, ts_data_snn, ts_labels_snn, rc->c);
        
        TEMPLATE(deleteVector,double)(ts_labels_snn);            
        TEMPLATE(deleteVector,pMatrix)(ts_data_snn);
        Rcpp::List l = SpikePatternsListToRList(spl);
        deleteSpikePatternsList(spl);

        return(l);
    }
    Rcpp::NumericVector getResponse(double x, double time_limit) {
        doubleVector *resp = computeResponse(tc, x, time_limit, rc->c);
        Rcpp::NumericVector v = DoubleVectorToRNumericVector(resp);
        TEMPLATE(deleteVector,double)(resp);
        return v;
    }
    Rcpp::List getStat() {
        if(!saveStat) {
            Rcpp::stop("Need statLevel>0");
        }
        Matrix *V = vectorArrayToMatrix(l->stat_V, l->N);
        Matrix *w = vectorArrayToMatrix(l->stat_w, l->N);

        pMatrixVector *mv = TEMPLATE(createVector,pMatrix)();
        TEMPLATE(insertVector,pMatrix)(mv, V);
        TEMPLATE(insertVector,pMatrix)(mv, w);
        Rcpp::List l = MatrixVectorToRList(mv);
        TEMPLATE(deleteVector,pMatrix)(mv);
        return l;
    }
private:
    bool saveStat;
    size_t nthreads;
    RConstants *rc;
    TuningCurves *tc;
    AdExLayer *l;
};

#endif
