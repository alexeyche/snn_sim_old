#define _POSIX_C_SOURCE 200809L


#include <snnlib/core.h>

#include <snnlib/sim/sim.h>
#include <snnlib/sim/serialize.h>
#include <snnlib/util/io.h>
#include <snnlib/util/spikes_list.h>


#include "sim_opts.h"

int main(int argc, char **argv) {
	
	ArgOptionsSim *a = parseSimOptions(argc, argv);
//    printArgs(&a);
    bool saveStat = a->stat_file != NULL;
    Constants *c = createConstants(a->const_filename);
    srand(a->seed);
    for(size_t i=0; i<c->lc->size; i++) {
        if(a->learn == 0) {
            getLayerConstantsC(c,i)->learn = false;
        }
    }
    printConstants(c);
    assert(a->jobs != 0);
    unsigned char statLevel = 0;
    if(saveStat) {
        statLevel = a->statLevel;
        if(a->calcStat) {
            statLevel = 1;
        }
    }        
    Sim *s = createSim(a->jobs, statLevel, c);
    
    char *model_to_load = NULL;
    if(a->model_file) model_to_load = strdup(a->model_file);
    if(a->model_file_load) model_to_load = strdup(a->model_file_load); 
    
    configureLayersSim(s, c);

    if(model_to_load) {
        loadLayersFromFile(s, model_to_load);
        free(model_to_load);
    }

    configureNetSpikesSim(s, c);

    SpikePatternsList *spl = readSpikePatternsListFromFile(a->input_spikes_file);
    simSetInputSpikePatterns(s, spl);
    deleteSpikePatternsList(spl);

    configureSynapses(s, c);
    if(a->Tmax > 0) {
        s->rt->Tmax = a->Tmax;
    }
    if(c->reinforcement) {
        configureRewardModulation(s);
    }

//    LayerPoisson *l = s->layers->array[0];
//    l->printLayer(l);
//    printSpikesList(s->ns->net);
//    printConnMap(s->ns);
//    printInputSpikesQueue(s->ns);
//    return(0);
    runSim(s);
    
    if(a->input_port>0) {
        printf("Not implemented\n");        
    }

    if(statLevel > 0) {
        checkIdxFnameOfFile(a->stat_file);
        pMatrixVector *mv = TEMPLATE(createVector,pMatrix)();
        if(c->reinforcement) {
            Matrix *m_glob_rev = vectorArrayToMatrix(&s->ctx->stat_global_reward, 1);
            TEMPLATE(insertVector,pMatrix)(mv, m_glob_rev); 
        }
        for(size_t li=0; li < s->layers->size; li++) {
            LayerPoisson *l = s->layers->array[li];
            l->saveStat(l, mv);
        }            
        saveMatrixListToFile(a->stat_file, mv);
    }
    char *model_to_save = NULL;
    if(a->model_file) model_to_save = strdup(a->model_file);
    if(a->model_file_save) model_to_save = strdup(a->model_file_save); 
    
    if(model_to_save) { 
        saveLayersToFile(s, model_to_save);
        free(model_to_save);
    }
    if(a->output_spikes_file) {
        pMatrixVector *mv = TEMPLATE(createVector,pMatrix)();
        
        TEMPLATE(insertVector,pMatrix)(mv, vectorArrayToMatrix(s->ns->net->list, s->ns->net->size));
        TEMPLATE(insertVector,pMatrix)(mv, vectorArrayToMatrix(&s->rt->reset_timeline, 1));
        TEMPLATE(insertVector,pMatrix)(mv, vectorArrayToMatrix(&s->rt->pattern_classes, 1));

        checkIdxFnameOfFile(a->output_spikes_file);
        saveMatrixListToFile(a->output_spikes_file, mv);

        TEMPLATE(deleteVector,pMatrix)(mv);
    }
    deleteSim(s);
    deleteConstants(c);
    deleteArgOptionsSim(a);
}
