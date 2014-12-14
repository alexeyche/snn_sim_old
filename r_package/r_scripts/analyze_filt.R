require(SynchWave)

source('filt_funcs.R')

target_signal = c(loadMatrix("~/prog/sim/target_value.bin",1))
w_opt = loadMatrix("~/prog/sim/filt_output.bin",1)
restored_signal = c(loadMatrix("~/prog/sim/filt_output.bin",2))

x = target_signal
y = restored_signal
g0 = target_signal
g1 = restored_signal


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

cat("Err:",mean( (x-y)^2 ), " snr_time:", snr_time," snr_freq:", snr_freq)
