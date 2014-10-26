#!/usr/bin/env Rscript

args <- commandArgs(trailingOnly = FALSE)
arg_i = grep("--args", args)

l = list()
for(i in seq(arg_i+1, length(args))) {
    m = read.table(args[i], header=TRUE, sep="\t")
    l[[ length(l)+1 ]] = data.matrix(m)
}
ans = apply(simplify2array(l), 1:2, mean)
ans_sd = apply(simplify2array(l), 1:2, sd)
ans = cbind(ans, ans_sd[,2:3])
print(ans)
