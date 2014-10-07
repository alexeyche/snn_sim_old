#include <snnlib/core.h>
#include <time.h>


#include <snnlib/util/spikes_list.h>
#include <snnlib/util/io.h>
#include <snnlib/util/util.h>
#include <snnlib/prep/run.h>

#include <snnlib/constants.h>

#include "prep_opts.h"

int main(int argc, char **argv) {
    ArgOptionsPrep a = parsePrepOptions(argc, argv);
    Constants *c = createConstants(a.const_filename);
//    printConstants(c);

    pMatrixVector* ts_data = readMatrixListFromFile(a.input_file);
    assert(ts_data->size > 0);

    pMatrixVector* ts_labels_m = readMatrixListFromFile(a.input_labels_file);
    assert(ts_labels_m->size == 1);
    assert(ts_labels_m->array[0]->nrow == ts_data->size);

    doubleVector *ts_labels = doubleVectorFromMatrix(ts_labels_m->array[0]);

    AdExLayer *l = createAdExLayer(c->preproc->N, false);
    TuningCurves *tc = initTuningCurves(c);
    SpikePatternsList *spl = runNeurons(l, tc, ts_data, ts_labels, c);
    saveSpikePatternsListToFile(spl, a.output_file);
    
    tc->free(tc);
    deleteAdExLayer(l);        
    return(0.0);
} 
