
#include "matrix.h"

#include <snnlib/util/templates_clean.h>
#define T pMatrix
#define DESTRUCT deleteMatrix
#include <snnlib/util/util_vector_tmpl.c>



Matrix *createMatrix(size_t nr, size_t nc) {
    Matrix *m = (Matrix*)malloc(sizeof(Matrix));
    m->nrow = nr;
    m->ncol = nc;
    m->vals = (double*)malloc(m->nrow*m->ncol*sizeof(double));
    m->transposed = false;
    return(m);
}

Matrix *createMatrixOnArray(size_t nr, size_t nc, double *vals) {
    Matrix *m = (Matrix*)malloc(sizeof(Matrix));
    m->nrow = nr;
    m->ncol = nc;
    m->vals = vals;
    return(m);
}

Matrix *createZeroMatrix(size_t nr, size_t nc) {
    Matrix *m = createMatrix(nr,nc);
    for(size_t i=0; i<nr; i++) {
        for(size_t j=0; j<nc; j++) {
            setMatrixElement(m, i, j, 0.0);
        }
    }
    return(m);
}

Matrix *createNormMatrix(size_t nr, size_t nc) {
    Matrix *m = createMatrix(nr,nc);
    for(size_t i=0; i<nr; i++) {
        for(size_t j=0; j<nc; j++) {
            setMatrixElement(m, i, j, getNorm());
        }
    }
    return(m);
}

void transposeMatrix(Matrix *m) {
    if(m->transposed == false) {
        m->transposed = true;
    } else {
        m->transposed = false;
    }
    size_t b = m->ncol;
    m->ncol = m->nrow;
    m->nrow = b;
}

void deleteMatrix(Matrix *m) {
    free(m->vals);
    free(m);
}

void setMatrixElement(Matrix *m, size_t i, size_t j, double val) {
    TRANSPOSE_IND
    m->vals[j*m->nrow + i] = val;
}
double getMatrixElement(Matrix *m, size_t i, size_t j) {
    TRANSPOSE_IND
    return m->vals[j*m->nrow + i];
}

void printMatrix(Matrix *m) {
    for(size_t mi=0; mi < m->nrow; mi++) {
        for(size_t mj=0; mj < m->ncol; mj++) {
            printf("%f ", getMatrixElement(m, mi, mj));
        }
        printf("\n");
    }
}

Matrix* vectorArrayToMatrix(doubleVector **dv, size_t size) {
    size_t col_max = 0;
    for(size_t ri=0; ri< size; ri++) {
        if(dv[ri]->size>col_max) {
            col_max = dv[ri]->size;
        }
    }
    Matrix *m = createMatrix(size, col_max);
    for(size_t i=0; i<size; i++) {
        for(size_t j=0; j<col_max; j++) {
            if(j < dv[i]->size) {
                setMatrixElement(m, i, j, dv[i]->array[j]);
            } else {
                setMatrixElement(m, i, j, 0);
            }
        }
    }
    return(m);        
}

Matrix *copyMatrix(Matrix *m) {
    Matrix *out = createMatrix(m->nrow, m->ncol);
    for(size_t i=0; i<m->nrow; i++) {
        for(size_t j=0; j<m->ncol; j++) {
            setMatrixElement(out, i, j, getMatrixElement(m, i, j));
        } 
    }
    return(out);
}

void incMatrixElement(Matrix *m, size_t i, size_t j, double inc) {
    double v = getMatrixElement(m, i, j);
    setMatrixElement(m, i, j, v+inc);
}


doubleVector *doubleVectorFromMatrix(Matrix *m) {
    assert(m->ncol == 1);
    doubleVector *v = TEMPLATE(createVector,double)();
    for(size_t ri=0; ri < m->nrow; ri++) {
        TEMPLATE(insertVector,double)(v, getMatrixElement(m, ri, 0));
    }
    return(v);
}
