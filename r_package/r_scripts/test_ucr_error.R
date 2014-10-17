
source('ucr_ts.R')
sample_size = 120
train = list()
labels = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TRAIN_%s_labels",sample_size),1)
#labels = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TEST_%s_labels",sample_size),1)
ulabels = unique(labels)
for(i in 1:length(labels)) {    
    s = list()
    s$data = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TRAIN_%s",sample_size),i)
    s$label = labels[i]
    train[[i]] = s
}

test = list()
labels = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TEST_%s_labels",sample_size),1)
ulabels = unique(labels)
for(i in 1:length(labels)) {    
    s = list()
    s$data = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TEST_%s",sample_size),i)
    s$label = labels[i]
    test[[i]] = s
}

ans = ucr_test(train, test, eucl_dist_alg)