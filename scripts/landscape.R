# parse command line
args <- commandArgs(T)
(name = args[1])

plot_file = paste(name, ".png", sep="")
data_file = paste(name, ".csv", sep="")

# configuration
title=paste(name)
lwd = 2
ptsize = 14

# read data
data = read.csv(data_file, skip=1)

val = as.numeric(unlist(data[1]))
suc = as.numeric(unlist(data[2]))
rob = as.numeric(unlist(data[3]))
ali = as.numeric(unlist(data[4]))
cov = as.numeric(unlist(data[5]))
wre = as.numeric(unlist(data[6]))
str = as.numeric(unlist(data[7]))
vol = as.numeric(unlist(data[8]))
qsum = as.numeric(unlist(data[9]))
qlog = as.numeric(unlist(data[10]))

# draw plot
png(filename=plot_file, width=1024, height=768, units="px", pointsize=ptsize)
plot(suc~val, type='l', xlab='value', ylab='q', col='red', main=title, lwd=lwd, ylim=range(c(0, 1)), xaxs="i", yaxs="i")
par(new=TRUE)
plot(rob~val, type='l', col='darkred', lwd=lwd, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, 1)), xaxs="i", yaxs="i")
par(new=TRUE)
plot(cov~val, type='l', col='green', lwd=lwd, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, 1)), xaxs="i", yaxs="i")
par(new=TRUE)
plot(wre~val, type='l', col='blue', lwd=lwd, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, 1)), xaxs="i", yaxs="i")
par(new=TRUE)
plot(ali~val, type='l', col='darkorange', lwd=lwd, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, 1)), xaxs="i", yaxs="i")
par(new=TRUE)
plot(str~val, type='l', col='violet', lwd=lwd, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, 1)), xaxs="i", yaxs="i")
par(new=TRUE)
plot(vol~val, type='l', col='cyan', lwd=lwd, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, 1)), xaxs="i", yaxs="i")
par(new=TRUE)
plot(qsum~val, type='l', col='black', lwd=3, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, 1)), lty=2, xaxs="i", yaxs="i")
par(new=TRUE)
plot(qlog~val, type='l', col='black', lwd=3, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, 1)), xaxs="i", yaxs="i")
par(new=F)

legend("topright", legend=c("success", "robustness", "alignment", "coverage", "wrench", "stress", "volume", "average", "Q"), col=c("red", "darkred", "darkorange", "green", "blue", "violet", "cyan", "black", "black"), lty=c(1, 1, 1, 1, 1, 1, 1, 2, 1), lwd=lwd)
dev.off()
