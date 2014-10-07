#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <snnlib/constants.h>
#include <snnlib/sim/runtime.h>
#include <snnlib/layers/wta.h>
#include <snnlib/layers/wtaadapt.h>
#include <snnlib/layers/adapt.h>

struct Sim;
struct SimImpl;

void configureLayersSim(struct Sim *s, Constants *c);
void configureNetSpikesSim(struct Sim *s, const Constants *c);
void configureSimImpl(struct Sim *s);
void configureSynapses(struct Sim *s, Constants *c);
void configureRewardModulation(struct Sim *s);

#endif
