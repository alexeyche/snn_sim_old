require(SynchWave)

source('filt_funcs.R')

x = loadMatrix("~/prog/sim/ts/synthetic_control/synthetic_control_TRAIN_60_cat.bin",1)
y = loadMatrix("~/prog/snn_sim/build/out.bin",1)
w_opt = loadMatrix("~/prog/snn_sim/build/wopt.bin",1)


#opt_res = optim(w, m_E, m_dEdw, method="BFGS",control=list(trace=1), hessian=FALSE)
#w_opt = opt_res$par
mean_area = 1:length(x)
area_cut = mean_area
if(length(area_cut) > 200) {
    area_cut = area_cut[1:200]
}


d = conv_mat(1:100,y,w_opt)
plot(x[area_cut], type="l", col="black", lwd=2)
lines(d[area_cut], col="red")



g0 = x
g1 = d


p_signal = sum(abs(g0)^2)/length(x)
p_noise = sd(g1-g0)^2

snr_time = 10*log10(p_signal/p_noise)

G0 = fft(g0)/length(x)
G0 = fftshift(G0)
G0_dB = 20*log10(abs(G0))

G1 = fft(g1)/length(x)
G1 = fftshift(G1)
G1_dB = 20*log10(abs(G1))

Fs = 1000 # Hz, sampling rate
f = Fs/2*seq(-1,1,length.out=length(x))

p_signal_f = sum(abs(G0)^2)/length(G0)
p_noise_f = sd( abs(G1-G0)^2 )
p_noise_f1 = sum( abs(G1-G0)^2 )

snr_freq = 10*log10(p_signal_f/p_noise_f)

plot(f, G0_dB, type="l")
lines(f, G1_dB, col="red")

cat("Err:",m_E(w_opt), " snr_time:", snr_time," snr_freq:",snr_freq)
