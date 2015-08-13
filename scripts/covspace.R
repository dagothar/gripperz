# This script is used to plot covspace data:
# a plot of coverage in slices for length, depth & tcpoffset
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
PLOT_COLORS = colorRampPalette(c('green', 'white'))(100)
PLOT_ANGLE_Z = 35
PLOT_ANGLE_X = -65
N_TICKS = 10
PTSIZE=12
PLOT_W=1024
PLOT_H=768

# Script
args <- commandArgs(T)
(filename <- args[1])

data <- read.csv(filename)

# plot length slices
idx <- 0
for (length in seq(0, 0.1, by=0.01)) {

	slice <- data[data$length == length,]
	if (nrow(slice) == 0) break
	
	plot_name <- paste('length_', idx, sep='')
	plot_name <- paste(plot_name, '.png', sep='')
	
	plot_title <- paste('length = ', length, sep='')

	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	
	plot <- wireframe(
		coverage ~ depth * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=PLOT_COLORS, colorkey=F,
		main=list(label=plot_title, line=-2),
		zlab=list(rot=90),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	
	print(plot)
	idx <- idx + 1
}

# plot depth slices
idx <- 0
for (depth in seq(0, 0.025, by=0.0025)) {

	slice <- data[data$depth == depth,]
	if (nrow(slice) == 0) break
	
	plot_name <- paste('depth_', idx, sep='')
	plot_name <- paste(plot_name, '.png', sep='')
	
	plot_title <- paste('depth = ', depth, sep='')

	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	
	plot <- wireframe(
		coverage ~ length * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=PLOT_COLORS, colorkey=F,
		main=list(label=plot_title, line=-2),
		zlab=list(rot=90),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	
	print(plot)
	idx <- idx + 1
}

# plot tcpoffset slices
idx <- 0
for (tcpoffset in seq(0, 0.1, by=0.01)) {

	slice <- data[data$tcpoffset == tcpoffset,]
	if (nrow(slice) == 0) break
	
	plot_name <- paste('tcpoffset_', idx, sep='')
	plot_name <- paste(plot_name, '.png', sep='')
	
	plot_title <- paste('tcpoffset = ', tcpoffset, sep='')

	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	
	plot <- wireframe(
		coverage ~ length * depth, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=PLOT_COLORS, colorkey=F,
		main=list(label=plot_title, line=-2),
		zlab=list(rot=90),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	
	print(plot)
	idx <- idx + 1
}

dev.off()
