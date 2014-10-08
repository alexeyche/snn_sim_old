

#include <snnlib/core.h>
#include <snnlib/util/spikes_list.h>
#include <snnlib/util/io.h>
#include <snnlib/util/util.h>
#include <snnlib/filt/filt_funcs.h>

#include "optim_lbfgs.h"
#include <lbfgs.h>


#include "filt_opts.h"

int main(int argc, char **argv) {
    srand(time(NULL));

    ArgOptionsFilt a = parseFiltOptions(argc, argv);
    pMatrixVector* ts_data = readMatrixListFromFile(a.input_file);
    pMatrixVector* target_data = readMatrixListFromFile(a.target_values_file);
    assert(ts_data->size > 0);
    assert(target_data->size > 0);

    Matrix *spike_data = ts_data->array[0];
    doubleVector *target = doubleVectorFromMatrix(target_data->array[0]);
    if(spike_data->ncol != target->size) {
        printf("Spikes data do not equals target data %zu != %zu\n", spike_data->ncol, target->size);
        exit(1);
    }

    Matrix *w_opt = runLbfgsOptim(spike_data, target, a.filter_size, a.jobs, a.epsilon);
    doubleVector *rec = calcConv(spike_data, w_opt, target, a.jobs);
    Matrix *rec_m = vectorArrayToMatrix(&rec, 1);
    transposeMatrix(rec_m);
    TEMPLATE(deleteVector,double)(rec);

    pMatrixVector *out = TEMPLATE(createVector,pMatrix)();
    TEMPLATE(insertVector,pMatrix)(out, w_opt);
    TEMPLATE(insertVector,pMatrix)(out, rec_m);

    saveMatrixListToFile(a.output_file, out);

    TEMPLATE(deleteVector,pMatrix)(ts_data);
    TEMPLATE(deleteVector,pMatrix)(out);
    return 0;
} 
