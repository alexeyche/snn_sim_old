#ifndef RUN_H
#define RUN_H

#include <time.h>

#include <snnlib/util/matrix.h>
#include <snnlib/util/spikes_list.h>

#include "adex.h"


#include <snnlib/prep/tuning_curves/linear.h>
#include <snnlib/prep/tuning_curves/sigma.h>

TuningCurves* initTuningCurves(const Constants *c);

doubleVector* computeResponse(TuningCurves *tc, double x, double time_limit, const Constants *c);
SpikePatternsList* runNeurons(AdExLayer *l, TuningCurves *tc, pMatrixVector *ts_data, doubleVector *ts_labels, Constants *c, double t_gap);

#endif
