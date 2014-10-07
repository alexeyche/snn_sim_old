#ifndef SIM_H
#define SIM_H

#include <stddef.h>
#include <pthread.h>

#include <snnlib/layers/poisson.h>
#include <snnlib/layers/wta.h>
#include <snnlib/constants.h>
#include <snnlib/util/spikes_list.h>
#include <snnlib/util/matrix.h>
#include <snnlib/util/io.h>


#include <snnlib/sim/runtime.h>
#include <snnlib/sim/netsim.h>
#include <snnlib/sim/configure.h>

#include <snnlib/learn/optim.h>
#include <snnlib/learn/res_stdp.h>
#include <snnlib/learn/simple_stdp.h>
#include <snnlib/learn/triple_stdp.h>


typedef struct {
    size_t layer_id;
    size_t n_id;
} NeuronAddress;


struct SimContext {
    double global_reward;
    double mean_global_reward;
    doubleVector *stat_global_reward;
    double sum_prob_wta;

    double actual_running_time;
    
    const Constants *c;
    unsigned char stat_level;
};
typedef struct SimContext SimContext;


struct SimImpl{
    size_t net_size;
    size_t nthreads;
    NeuronAddress *na;
    size_t num_neurons;
};
typedef struct SimImpl SimImpl;

struct Sim {
    pLayerVector *layers;
    NetSim *ns;
    SimRuntime *rt;
    SimContext *ctx;    

    SimImpl *impl;
};
typedef struct Sim Sim;

typedef struct {
    Sim *s;
    size_t thread_id;
    int first;
    int last;
} SimWorker;



Sim* createSim(size_t nthreads, unsigned char stat_level, Constants *c);
void appendLayerSim(Sim *s, LayerPoisson *l);
void deleteSim(Sim *s);
size_t getLayerIdOfNeuron(Sim *s, size_t n_id);

void runSim(Sim *s);
void simulateNeuron(Sim *s, const size_t *layer_id, const size_t *n_id, const double *t);
const SynSpike* getInputSpike(Sim *s, const size_t *layer_id, const size_t *n_id, const double *t);
void* simRunRoutine(void *args);
void simSetInputSpikes(Sim *s, SpikesList *sl);
void simSetInputSpikePatterns(Sim *s, SpikePatternsList *spl);

#endif
