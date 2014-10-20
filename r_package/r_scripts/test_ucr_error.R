
source('ucr_ts.R')
sample_size = 120
train = list()
labels = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TRAIN_%s_labels",sample_size),1)
#labels = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TEST_%s_labels",sample_size),1)
ulabels = unique(labels)
select_X = 1:length(labels)
#select_X = c(1:10, 101:110)
#select_X = c(1:50, 101:150) # two classes
#select_X = c(1:50, 51:100, 101:150) # 3 classes
for(i in select_X) {    
    s = list()
    s$data = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TRAIN_%s",sample_size),i)
    s$label = labels[i]
    train[[length(train)+1]] = s
}

test = list()
labels = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TEST_%s_labels",sample_size),1)
select_X = 1:length(labels)
#select_X = c(1:10, 101:110)
#select_X = c(1:50, 101:150) # two classes
#select_X = c(1:50, 51:100, 101:150) # 3 classes
for(i in select_X) {    
    s = list()
    s$data = loadMatrix(sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TEST_%s",sample_size),i)
    s$label = labels[i]
    test[[length(test)+1]] = s
}

ans = ucr_test(train, test, eucl_dist_alg)


train_svm = sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TRAIN_%s_svm.dat",sample_size)
if(file.exists(train_svm)) {
    system(sprintf("rm -f %s", train_svm))
}
for(i in 1:length(train)) {
    xx = cbind(1:nrow(train[[i]]$data), train[[i]]$data)
    write(c( c(train[[i]]$label), sapply(1:nrow(xx), function(j) paste(xx[j,1], xx[j,2], sep=":") )), train_svm, append=TRUE, ncolumns=sample_size+1)
}
test_svm = sprintf("~/prog/sim/ts/synthetic_control/synthetic_control_TEST_%s_svm.dat",sample_size)
if(file.exists(test_svm)) {
    system(sprintf("rm -f %s", test_svm))
}
for(i in 1:length(test)) {
    xx = cbind(1:nrow(test[[i]]$data), test[[i]]$data)
    write(c( c(test[[i]]$label), sapply(1:nrow(xx), function(j) paste(xx[j,1], xx[j,2], sep=":") )), test_svm, append=TRUE, ncolumns=sample_size+1)
}
