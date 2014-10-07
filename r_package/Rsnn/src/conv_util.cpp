
#include "conv_util.h"

SpikesList* RListToSpikesList(Rcpp::List l) {
    SpikesList* sl = createSpikesList(l.size());
    for(size_t i=0; i<l.size(); i++) {
        Rcpp::NumericVector v = l[i];
        for(size_t vi=0; vi<v.size(); vi++) {
            TEMPLATE(insertVector,double)(sl->list[i], v[vi]);
        }
    }
    return sl;
}

Rcpp::List SpikesListToRList(SpikesList *sl) {
    Rcpp::List l(sl->size);
    for(size_t i=0; i<sl->size; i++) {
        Rcpp::NumericVector v(sl->list[i]->size);
        for(size_t vi=0; vi<sl->list[i]->size; vi++) {
            v[vi] = sl->list[i]->array[vi];
        }
        l[i] = v;
    }
    return l;
}

Rcpp::NumericMatrix MatrixToRMatrix(Matrix *m) {
    Rcpp::NumericMatrix rm(m->nrow, m->ncol);
    for(size_t i=0; i<m->nrow; i++) {
        for(size_t j=0; j<m->ncol; j++) {
            rm(i,j) = getMatrixElement(m, i, j);
        }
    }
    return rm;
}

Matrix* RMatrixToMatrix(Rcpp::NumericMatrix m) {
    Matrix *m_snn = createMatrix(m.nrow(), m.ncol());
    for(size_t i=0; i<m_snn->nrow; i++) {
        for(size_t j=0; j<m_snn->ncol; j++) {
            setMatrixElement(m_snn, i, j, m(i,j));
        }
    }
    return m_snn;
}

Rcpp::List MatrixVectorToRList(pMatrixVector *mv) {
    Rcpp::List l(mv->size);
    for(size_t i=0; i<mv->size; i++) {
        l[i] = MatrixToRMatrix(mv->array[i]);
    }
    return l;
}



Rcpp::NumericMatrix DoublesAllocToRMatrix(double **v, int *sizes, int N) {
    int max_size = -1;
    for(size_t i=0; i<N; i++) {
        if(max_size<sizes[i]) {
            max_size = sizes[i];
        }
    }
    assert(max_size>=0);
    Rcpp::NumericMatrix out(N, max_size);
    for(size_t i=0; i<N; i++) {
        for(size_t j=0; j<sizes[i]; j++) {
            out(i,j) = v[i][j];
        }
    }
    return out;
}

doubleVector *RNumericVectorToDoubleVector(Rcpp::NumericVector v) {
    doubleVector* ret = TEMPLATE(createVector,double)();
    for(size_t i=0; i<v.size(); i++) {
        TEMPLATE(insertVector,double)(ret, v[i]);
    }
    return ret;
}


Rcpp::NumericVector DoubleVectorToRNumericVector(doubleVector *v) {
    Rcpp::NumericVector ret(v->size);
    for(size_t i=0; i<v->size; i++) {
        ret(i) = v->array[i];
    }
    return ret;
}

pMatrixVector* RListToMatrixVector(Rcpp::List l) {
    pMatrixVector *v = TEMPLATE(createVector,pMatrix)();
    for(size_t i=0; i<l.size(); i++) {
        Rcpp::NumericMatrix m = l[i];
        Matrix *m_snn = RMatrixToMatrix(m); 
        TEMPLATE(insertVector,pMatrix)(v, m_snn);
    }
    return v;
}

Rcpp::List SpikePatternsListToRList(SpikePatternsList *spl) {
    Rcpp::List l(3);    
    l(0) = SpikesListToRList(spl->sl);
    l(1) = DoubleVectorToRNumericVector(spl->timeline);
    l(2) = DoubleVectorToRNumericVector(spl->pattern_classes);
    return l;
}


