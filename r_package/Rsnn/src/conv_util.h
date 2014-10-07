#ifndef CONV_UTIL_H
#define CONV_UTIL_H


#include <Rcpp.h>


extern "C" {
    #include <snnlib/util/spikes_list.h>
}


Rcpp::List SpikesListToRList(SpikesList *sl);
Rcpp::List MatrixVectorToRList(pMatrixVector *mv);
Rcpp::NumericMatrix MatrixToRMatrix(pMatrix *m);
Rcpp::NumericMatrix DoublesAllocToRMatrix(double **v, int *sizes, int N);
Rcpp::NumericVector DoubleVectorToRNumericVector(doubleVector *v);
Rcpp::List SpikePatternsListToRList(SpikePatternsList *spl);

SpikesList* RListToSpikesList(Rcpp::List l);
doubleVector *RNumericVectorToDoubleVector(Rcpp::NumericVector v);
pMatrixVector* RListToMatrixVector(Rcpp::List l);
Matrix* RMatrixToMatrix(Rcpp::NumericMatrix m);

#endif
