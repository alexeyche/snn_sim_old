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
        spl = NULL;
    }
    ~RPrepLayer() {
        tc->free(tc);
        deleteAdExLayer(l);
    }

    void run(Rcpp::List ts_data, Rcpp::NumericVector ts_labels, double gap_between_patterns = 0.0) {
        doubleVector *ts_labels_snn = RNumericVectorToDoubleVector(ts_labels);        
        pMatrixVector *ts_data_snn = RListToMatrixVector(ts_data);
        
        if(spl) deleteSpikePatternsList(spl);
        spl = runNeurons(l, tc, ts_data_snn, ts_labels_snn, rc->c, gap_between_patterns);
        
        TEMPLATE(deleteVector,double)(ts_labels_snn);            
        TEMPLATE(deleteVector,pMatrix)(ts_data_snn);
    }
    Rcpp::List getList() {
        if(spl) {
            Rcpp::List l = SpikePatternsListToRList(spl);
            return l;
        }
        Rcpp::stop("Need make run before getting an answer");
    }
    Rcpp::NumericMatrix getBinMatrix() {
        if(spl) {
            Matrix *m = spikesListToBinarySpikesMatrix(spl->sl);
            Rcpp::NumericMatrix rm = MatrixToRMatrix(m);
            deleteMatrix(m);
            return rm;
        }
        Rcpp::stop("Need make run before getting an answer");
    }
    Rcpp::NumericVector getResponse(double x, double time_limit) {
        doubleVector *resp = computeResponse(tc, x, time_limit, rc->c);
        Rcpp::NumericVector v = DoubleVectorToRNumericVector(resp);
        TEMPLATE(deleteVector,double)(resp);
        return v;
    }
    Rcpp::List getParams() {
        if(rc->c->preproc->tc == ESigmaTC) {
            Rcpp::NumericMatrix centers = DoubleVectorsToRMatrix( ((SigmaTuningCurves*)tc)->centers, tc->N );
            Rcpp::NumericMatrix sds = DoubleVectorsToRMatrix( ((SigmaTuningCurves*)tc)->sds, tc->N );
            Rcpp::NumericMatrix gains = DoubleVectorsToRMatrix( ((SigmaTuningCurves*)tc)->gains, tc->N );
            return Rcpp::List::create( Rcpp::Named("centers") = centers, Rcpp::Named("sds") = sds, Rcpp::Named("gains") = gains );
        } else 
        if(rc->c->preproc->tc == ELinearTC) {
            Rcpp::NumericVector encoder(tc->N);
            Rcpp::NumericVector gain(tc->N);
            Rcpp::NumericVector bias(tc->N);
            LinearTuningCurves* lts = (LinearTuningCurves*)tc;
            for(size_t ni=0; ni<tc->N; ni++) {
                encoder(ni) = lts->encoder[ni];
                gain(ni) = lts->gain[ni];
                bias(ni) = lts->bias[ni];
            }                
            return Rcpp::List::create( Rcpp::Named("gain") = gain, Rcpp::Named("gain") = gain, Rcpp::Named("encoder") = encoder );
        } else {
            Rcpp::stop("Unknown tuning curve type");
        }
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
    SpikePatternsList *spl;
};

#endif
