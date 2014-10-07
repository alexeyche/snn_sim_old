
require(Rsnn)
const_ini = "/home/alexeyche/prog/snn_sim/constants.ini"

jobs = 8
saveStat = TRUE

const = RConstants$new(const_ini)
const$setValue("preprocess","rate", "25:200")
const$setValue("preprocess","N", 100)
const$setValue("adex neuron", "t_ref", 2.0)
const$setValue("adex neuron", "slope", 1.0)
const$setValue("adex neuron", "a", 1.0)
const$setValue("adex neuron", "b", 0.75)

pl = RPrepLayer$new(const, saveStat, jobs)
plot_tuning_curves(pl)

X = vector("list",10)
labels = loadMatrix("~/prog/sim/ts/synthetic_control/synthetic_control_TRAIN_60_labels",1)
ulabels = unique(labels)
for(i in 1:length(X)) {    
    X[[i]] = loadMatrix("~/prog/sim/ts/synthetic_control/synthetic_control_TRAIN_60",i)
}
p = pl$run(X, labels)
prast(p[[1]],T0=0,Tmax=180)

s = pl$getStat()

V = s[[1]]
w = s[[2]]