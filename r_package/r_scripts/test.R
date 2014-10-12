
model = "/home/alexeyche/prog/sim/runs/PoissonLayer_OptimalSTDP_ExpHennequin_0010/1_model.bin"

mod = list()
for(i in 1:8) {
    mod[[i]] = loadMatrix(model,i)
}

