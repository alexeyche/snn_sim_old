
require(Rsnn)
const_ini = "/home/alexeyche/prog/snn_sim/constants.ini"

jobs = 8
saveStat = FALSE

const = RConstants$new(const_ini)
const$setValue("preprocess","tuning_curve", "SigmaTC")
const$setValue("preprocess","intercept", "-1:1")
const$setValue("preprocess","sigma", "0.02")
const$setValue("preprocess","sigma_gain", "30")
const$setValue("preprocess","prob_next_sigma", 0.2)
const$setValue("preprocess","max_curve_num", 1)
const$setValue("preprocess","N", 100)
const$setValue("adex neuron", "t_ref", 2.0)
const$setValue("adex neuron", "slope", 0.0)
const$setValue("adex neuron", "a", 0.0)
const$setValue("adex neuron", "b", 0.0)

pl = RPrepLayer$new(const, saveStat, jobs)
plot_tuning_curves(pl)

sample_size = 120

X = vector("list",300)
labels = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TRAIN_%s_labels",sample_size),1)
ulabels = unique(labels)
for(i in 1:length(X)) {    
    X[[i]] = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TRAIN_%s",sample_size),i)
}
min_X = min(unlist(X))
max_X = max(unlist(X))
for(i in 1:length(X)) {  
    X[[i]] =  2*(X[[i]]-min_X)/(max_X-min_X)-1
}
select_X = c(1:50, 101:150) # two classes
#select_X = c(1)

X=X[select_X] 
labels = labels[select_X]

gap_between_patterns = 100.0 # ms
p = pl$run(X, labels, gap_between_patterns)

prast(p[[1]],T0=0,Tmax=300)

p_serial = list()
p_serial[[1]] = list_to_matrix(p[[1]])
p_serial[[2]] = matrix(p[[2]], ncol=1)
p_serial[[3]] = matrix(p[[3]], ncol=1)
saveMatrixList("~/prog/sim/spikes/ucr_2_classes_test", p_serial)

