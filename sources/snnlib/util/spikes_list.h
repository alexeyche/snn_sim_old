#ifndef SPIKES_LIST_H
#define SPIKES_LIST_H

#include <snnlib/util/matrix.h>
#include <snnlib/util/util_vector.h>
#include <snnlib/util/io.h>

// SpikesList :

typedef struct {
    doubleVector **list;
    size_t size;
    double dt;
    double Tmax;
} SpikesList;

typedef struct {
    SpikesList *sl;
    doubleVector *timeline;
    doubleVector *pattern_classes;
} SpikePatternsList;


SpikesList* createSpikesList(size_t size_);
void readSpikesFromMatrix(SpikesList *sl, Matrix *m);
void deleteSpikesList(SpikesList *sl);
void printSpikesList(SpikesList *sl);
SpikesList* spikesMatrixToSpikesList(Matrix *m);
Matrix* spikesListToSpikesMatrix(SpikesList *sl);

SpikePatternsList* createSpikePatternsList(size_t n);
SpikePatternsList* readSpikePatternsListFromFile(const char *filename);
void deleteSpikePatternsList(SpikePatternsList *spl);
void saveSpikePatternsListToFile(SpikePatternsList *spl, const char *filename, bool binary_spikes);
Matrix* spikesListToBinarySpikesMatrix(SpikesList *sl);

#endif
