
require(Rsnn)
const_ini = "/home/alexeyche/prog/snn_sim/constants.ini"

jobs = 8
saveStat = FALSE

const = RConstants$new(const_ini)
const$setValue("preprocess","tuning_curve", "SigmaTC")
const$setValue("preprocess","intercept", "-1:1")
const$setValue("preprocess","sigma", "0.01")
const$setValue("preprocess","sigma_gain", "4")
const$setValue("preprocess","prob_next_sigma", 0.2)
const$setValue("preprocess","max_curve_num", 1)
const$setValue("preprocess","N", 100)
const$setValue("preprocess","dt", 1)
const$setValue("adex neuron", "t_ref", 0.0)
const$setValue("adex neuron", "slope", 0.0)
const$setValue("adex neuron", "a", 0.0)
const$setValue("adex neuron", "b", 0.0)

pl = RPrepLayer$new(const, saveStat, jobs)
plot_tuning_curves(pl)

sample_size = 60

train_X = list()
train_labels = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TRAIN_%s_labels",sample_size),1)
ulabels = unique(train_labels)
for(i in 1:length(train_labels)) {    
    train_X[[i]] = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TRAIN_%s",sample_size),i)
}

test_X = list()
test_labels = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TEST_%s_labels",sample_size),1)
for(i in 1:length(test_labels)) {    
    test_X[[i]] = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TEST_%s",sample_size),i)
}

train_eval_X = list()
train_eval_labels = NULL
for(ev_i in 1:3) {
    train_eval_labels = c(train_eval_labels, train_labels)
    for(i in 1:length(train_labels)) {    
        i_m = (ev_i-1)*length(train_labels) + i
        train_eval_X[[i_m]] = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TRAIN_%s",sample_size),i)
    }
}


test_eval_X = list()
test_eval_labels = NULL
for(ev_i in 1:3) {
    test_eval_labels = c(test_eval_labels, test_labels)
    for(i in 1:length(test_labels)) {    
        i_m = (ev_i-1)*length(test_labels) + i
        test_eval_X[[i_m]] = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TEST_%s",sample_size),i)
    }
}

data = list(
    train=list(X=train_X, labels=train_labels), 
    test=list(X=test_X, labels=test_labels),
    train_eval=list(X=train_eval_X, labels=train_eval_labels),
    test_eval=list(X=test_eval_X, labels=test_eval_labels)
)

for(data_name in names(data)) {
    X = data[[ data_name ]]$X
    labels = data[[ data_name ]]$labels
    
    min_X = min(unlist(X))
    max_X = max(unlist(X))
    for(i in 1:length(X)) {  
        X[[i]] =  2*(X[[i]]-min_X)/(max_X-min_X)-1
    }
    #select_X = c(1:50, 101:150) # two classes
    #select_X = c(1:50, 51:100, 101:150) # 3 classes
    #select_X = c(1:10, 101:110) # two classes, 10 from each one
    #select_X = c(101)
    select_X = 1:length(X)
    
    X=X[select_X] 
    labels = labels[select_X]
    
    gap_between_patterns = 100.0 # ms
    p = pl$run(X, labels, gap_between_patterns)
    
    p_serial = list()
    p_serial[[1]] = list_to_matrix(p[[1]])
    p_serial[[2]] = matrix(p[[2]], ncol=1)
    p_serial[[3]] = matrix(p[[3]], ncol=1)
    saveMatrixList(sprintf("~/prog/sim/spikes/ucr_2_classes_%s",data_name), p_serial)
}

prast(p[[1]],T0=0,Tmax=1000)




