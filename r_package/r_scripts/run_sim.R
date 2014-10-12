#!/usr/bin/env RScript


require(Rsnn)

M = 100
const_ini = "/home/alexeyche/prog/alexeyche-junk/cns/c/snn_sim/constants.ini"
N = 100
jobs = 4

const = RConstants$new(const_ini)
const$setValue("sim","M", M)
const$setValue("layer","N", N)
const$setValue("layer","neuron_type", "PoissonLayer")
const$setValue("layer","learning_rule", "OptimalSTDP")
const$setValue("layer","prob_fun", "ExpHennequin")
const$setValue("layer","lrate", 0.01)
const$setValue("layer","weight_decay_factor", 0.015)
const$setValue("layer","net_edge_prob", 0.0)
const$setValue("layer","input_edge_prob", 1.0)
const$setValue("layer","input_edge_prob", 1.0)
const$setValue("optimal stdp", "mean_p_dur", 30000.0)


input_spikes_file = "/home/alexeyche/prog/sim/spikes/ucr_2_classes_test.bin"

spikes = getSpikesFromMatrix(loadMatrix(input_spikes_file,1))

Tmax=max(sapply(spikes,max))+100


max_ep = ceiling(30000/Tmax) + 3
#max_ep = 2
mean_p = NULL
weights = list()

model = "/home/alexeyche/prog/temp_model.bin"

net = blank_net(N+M)
t0=0
statLevel = 2
s = RSim$new(const, statLevel, jobs)
for(i in 1:max_ep) {
    if(i > (max_ep-3)) { 
        s$setStatLevel(2)
    } else {
        s$setStatLevel(0)
    }
    s$setInputSpikes(spikes)
    out_sp = s$run()
    if(any(s$getStatLevel()>0)) {
        for(ni in 1:length(out_sp)) {
            net[[ni]] = c(net[[ni]], out_sp[[ni]] + t0)
        }
        t0=t0+Tmax
    }
    cat(i,"\n")
    
    s$serializeModelToFile(model)

    mean_p = cbind(mean_p, loadMatrix(model,7))
    weights[[i]] = s$W()[[1]]
}

if(any(s$getStatLevel()>0)) {
    stat = s$getLayerStat(0)
    Tplot = 1:(Tmax*3)
    nid = 1
    syn = 39
    p = stat[[1]]
    u = stat[[2]]    
    if(statLevel>1) {
        syns = stat[[2+nid]]
        w = stat[[2+N+nid]]
        B = stat[[2+2*N+1]]
        Cn = stat[[2+2*N+1+nid]]
        
        par(mfrow=c(4,1))
        plotl(B[nid,Tplot])
        spikes = net[[M+nid]][net[[M+nid]]<max(Tplot)]
        plot(spikes, rep(1,length(spikes)), xlim=c(min(Tplot),max(Tplot)) )
        plotl(syns[syn,Tplot])
        plotl(w[syn,Tplot])
    }
} else {
    prast(out_sp,T0=0,Tmax=1000)
    gr_pl(s$W()[[1]])    
}
