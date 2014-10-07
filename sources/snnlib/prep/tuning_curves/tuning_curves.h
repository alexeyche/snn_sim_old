#ifndef TUNING_CURVES_H
#define TUNING_CURVES_H

#include <snnlib/constants.h>

struct TuningCurves {
    size_t N;

    double (*getResponse)(struct TuningCurves *, size_t ni, double input);
    void (*free)(struct TuningCurves *);
};

typedef struct TuningCurves TuningCurves;

#endif
