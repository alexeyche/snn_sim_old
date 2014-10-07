
plot_tuning_curves = function(prep_layer) {
    x = seq(-1, 1, length.out=100)
    resp = NULL
    for(xv in x) {
        r = prep_layer$getResponse(xv, 1000)
        resp = cbind(resp, r*1000)
    }
    
    cols = rainbow(nrow(resp))
    for(ni in 1:nrow(resp)) {
        if(ni==1) {
            plot(x, resp[ni,], type="l", col=cols[ni], ylim=c(min(resp), max(resp)))
        } else {
            lines(x, resp[ni,], col=cols[ni])
        }
    }
}

