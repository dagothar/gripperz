# This script is used to plot cutspace data:
# a plot of alignment in slices for cutdepth, cutangle
#
# Input: space.csv file
# Output: a series of plots in .png representing slices of 3d space

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
PLOT_COLORS = colorRampPalette(c('orange', 'white'))(100)
PLOT_ANGLE_Z = 35
PLOT_ANGLE_X = -65
N_TICKS = 10
PTSIZE = 12
PLOT_W = 1024
PLOT_H = 768

Q = expression(coverage)

# Script
args <- commandArgs(T)
(filename <- args[1])

data <- read.csv(filename)

# plot cutdepth slice
plot_name <- paste('slice', '.png', sep='')

png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
trellis.par.set('axis.line', list(col='transparent'))

plot <- wireframe(
	alignment ~ cutdepth * cutangle, data = data,
	zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
	scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
	drape=T, col.regions=PLOT_COLORS, colorkey=F,
	main='alignment',
	zlab=list(rot=90),
	par.settings=theme.novpadding,
	zoom=0.9
)
	
print(plot)


dev.off()
