# This script is used to plot a slice of space data
#
# Input: space.csv file
# Output: a png plot

library('lattice')
theme.novpadding <- list(
    layout.heights = list(
        top.padding = 0,
        main.key.padding = 0,
        key.axis.padding = 0,
        axis.xlab.padding = 0,
        xlab.key.padding = 0,
        key.sub.padding = 0,
        bottom.padding = 0
    ),
    layout.widths = list(
        left.padding = 0,
        key.ylab.padding = 0,
        ylab.axis.padding = 0,
        axis.key.padding = 0,
        right.padding = 0
    )
)

# Configuration
PLOT_COLORS = colorRampPalette(c('black', 'white'))(100)
PLOT_ANGLE_Z = -235
PLOT_ANGLE_X = -65
N_TICKS = 10
PTSIZE=12
PLOT_W=1024
PLOT_H=768

# Script
args <- commandArgs(T)
(filename <- args[1])
(aaa <- args[2])

data <- read.csv(filename)

# plot slice
slice <- data[data$cuttilt == 0,]

png(filename='plot.png', width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
trellis.par.set('axis.line', list(col='transparent'))

plot <- wireframe(
	success ~ cutdepth * cutangle, data = slice,
	zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
	scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
	drape=T, col.regions=PLOT_COLORS, colorkey=F,
	#main=list(label=plot_title, line=-2),
	zlab=list(rot=90),
	par.settings=theme.novpadding,
	zoom=0.9
)

print(plot)

dev.off()
