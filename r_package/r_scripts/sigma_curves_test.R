
require(Rsnn)
const_ini = "/home/alexeyche/prog/snn_sim/constants.ini"

jobs = 8
saveStat = TRUE

const = RConstants$new(const_ini)
const$setValue("preprocess","tuning_curve", "SigmaTC")
const$setValue("preprocess","intercept", "-1:1")
const$setValue("preprocess","sigma", "0.3:0.3")
const$setValue("preprocess","sigma_gain", "10.5")
const$setValue("preprocess","prob_next_sigma", 0.2)
const$setValue("preprocess","max_curve_num", 1)
const$setValue("preprocess","N", 10)
const$setValue("adex neuron", "t_ref", 2.0)
const$setValue("adex neuron", "slope", 0.0)
const$setValue("adex neuron", "a", 0.0)
const$setValue("adex neuron", "b", 0.0)

pl = RPrepLayer$new(const, saveStat, jobs)
plot_tuning_curves(pl)

X = vector("list",10)
labels = c(loadMatrix("~/prog/sim/ts/synthetic_control/synthetic_control_TRAIN_60_labels",1))
ulabels = unique(labels)
for(i in 1:length(X)) {    
    X[[i]] = loadMatrix("~/prog/sim/ts/synthetic_control/synthetic_control_TRAIN_60",i)
}

pl$run(X, labels,0.0)
m = pl$getBinMatrix()

saveMatrixList("~/prog/sim/target_value", list(matrix(unlist(X))))
saveMatrixList("~/prog/sim/input_matrix", list(m))
