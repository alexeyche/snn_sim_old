
require(Rsnn)
const_ini = "/home/alexeyche/prog/snn_sim/constants.ini"

jobs = 8
saveStat = TRUE

const = RConstants$new(const_ini)
const$setValue("preprocess","tuning_curve", "SigmaTC")
const$setValue("preprocess","intercept", "-1:1")
const$setValue("preprocess","sigma", "0.05:0.05")
const$setValue("preprocess","sigma_gain", "30")
const$setValue("preprocess","prob_next_sigma", 0.2)
const$setValue("preprocess","max_curve_num", 2)
const$setValue("preprocess","N", 100)
const$setValue("adex neuron", "t_ref", 2.0)
const$setValue("adex neuron", "slope", 0.0)
const$setValue("adex neuron", "a", 0.0)
const$setValue("adex neuron", "b", 0.0)

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
