# parse command line
args <- commandArgs(T)
(name = args[1])

plot_file = paste(name, ".png", sep="")
data_file = paste(name, ".csv", sep="")

# configuration
title=paste(name)
lwd = 2
ptsize = 24
ymax = 1

# read data
data = read.csv(data_file, skip=0)

val = as.numeric(unlist(data[1]))
suc = as.numeric(unlist(data[2]))
#rob = as.numeric(unlist(data[3]))
ali = as.numeric(unlist(data[3]))
cov = as.numeric(unlist(data[4]))
wre = as.numeric(unlist(data[5]))
str = as.numeric(unlist(data[6]))
vol = as.numeric(unlist(data[7]))
qsum = as.numeric(unlist(data[8]))
qlog = as.numeric(unlist(data[9]))

# draw plot
png(filename=plot_file, width=1024, height=768, units="px", pointsize=ptsize)

# this is to make border for legend
par(oma = c(0, 0, 0, 0))

# actual plot
plot(suc~val, type='l', xlab='value', ylab='q', col='red', main=title, lwd=lwd, ylim=range(c(0, ymax)), xaxs="i", yaxs="i")
par(new=TRUE)
#plot(rob~val, type='l', col='darkred', lwd=lwd, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, ymax)), xaxs="i", yaxs="i")
#par(new=TRUE)
plot(cov~val, type='l', col='green', lwd=lwd, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, ymax)), xaxs="i", yaxs="i")
par(new=TRUE)
plot(wre~val, type='l', col='blue', lwd=lwd, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, ymax)), xaxs="i", yaxs="i")
par(new=TRUE)
plot(ali~val, type='l', col='darkorange', lwd=lwd, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, ymax)), xaxs="i", yaxs="i")
par(new=TRUE)
plot(str~val, type='l', col='violet', lwd=lwd, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, ymax)), xaxs="i", yaxs="i")
par(new=TRUE)
plot(vol~val, type='l', col='cyan', lwd=lwd, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, ymax)), xaxs="i", yaxs="i")
par(new=TRUE)
plot(qsum~val, type='l', col='black', lwd=3, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, ymax)), lty=2, xaxs="i", yaxs="i")
par(new=TRUE)
plot(qlog~val, type='l', col='black', lwd=3, axes = FALSE, xlab = "", ylab = "", ylim=range(c(0, ymax)), xaxs="i", yaxs="i")
par(new=F)

par(fig = c(0, 1, 0, 1), oma = c(0, 0, 0, 0), mar = c(0, 0, 0, 0), new = TRUE)
plot(0, 0, type = "n", bty = "n", xaxt = "n", yaxt = "n")
#legend("right", legend=c("success", "alignment", "coverage", "wrench", "stress", "volume", "average", "Q"), col=c("red", "darkorange", "green", "blue", "violet", "cyan", "black", "black"), lty=c(1, 1, 1, 1, 1, 1, 2, 1), lwd=lwd, xpd = TRUE, inset = c(0, 0), bty='n')

dev.off()
