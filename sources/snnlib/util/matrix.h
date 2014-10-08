#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <snnlib/util/templates.h>
#include <snnlib/util/util_vector.h>
#include <snnlib/util/util.h>

typedef struct {
    double *vals;
    size_t nrow;
    size_t ncol;
    bool transposed;
} Matrix;

#define TRANSPOSE_IND { \
    if(m->transposed) { \
        size_t b = i;\
        i=j;\
        j=b;\
    }\
}\

typedef Matrix* pMatrix;

#include <snnlib/util/templates_clean.h>
#define T pMatrix
#define DESTRUCT deleteMatrix
#include <snnlib/util/util_vector_tmpl.h>

Matrix *createMatrix(size_t nr, size_t nc);
Matrix *createMatrixOnArray(size_t nr, size_t nc, double *vals);
Matrix *createZeroMatrix(size_t nr, size_t nc);
Matrix *createNormMatrix(size_t nr, size_t nc);
Matrix *copyMatrix(Matrix *m);
void setMatrixElement(Matrix *m, size_t i, size_t j, double val);
double getMatrixElement(Matrix *m, size_t i, size_t j);
void deleteMatrix(Matrix *m);
void printMatrix(Matrix *m);
Matrix *vectorArrayToMatrix(doubleVector **dv, size_t size);
void transposeMatrix(Matrix *m);

void incMatrixElement(Matrix *m, size_t i, size_t j, double inc);

doubleVector *doubleVectorFromMatrix(Matrix *m);

#endif
