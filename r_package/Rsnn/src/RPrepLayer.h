#ifndef RPREP_LAYER_H
#define RPREP_LAYER_H

#include <Rcpp.h>

#include "RConstants.h"
#include "conv_util.h"

extern "C" {
    #include <core/prep/run.h>
}



class RPrepLayer {
public:    
    RPrepLayer(RConstants *rc_, unsigned char statLevel_, size_t nthreads_) : statLevel(statLevel_), nthreads(nthreads_), rc(rc_) {}
    Rcpp::List run() {
    }
private:
    unsigned char statLevel;
    size_t nthreads;
    RConstants *rc;
};

#endif
