#ifndef PREPARE_TS_H
#define PREPARE_TS_H


#include <float.h>
#include <math.h>

#include <snnlib/constants.h>
#include <snnlib/util/matrix.h>

pMatrixVector* processTimeSeriesSet(pMatrixVector *v, const Constants *c);
Matrix* processThroughReceptiveFields(Matrix *ts, const double *centers, const Constants *c);


#endif
