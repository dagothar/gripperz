# This script is used to plot cutspace data:
# a plot of alignment in slices for cutdepth, cutangle & cuttilt
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

data <- read.csv(filename)

# plot cutdepth slices
idx <- 0
for (cutdepth in seq(0, 0.05, by=0.0025)) {

	slice <- data[data$cutdepth == cutdepth,]
	if (nrow(slice) == 0) break
	
	plot_name <- paste('cutdepth_', idx, sep='')
	plot_name <- paste(plot_name, '.png', sep='')
	
	plot_title <- paste('cutdepth = ', cutdepth, sep='')

	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	
	plot <- wireframe(
		alignment ~ cutangle * cuttilt, data = slice,
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

# plot cutangle slices
idx <- 0
for (cutangle in seq(0, 180, by=18)) {

	slice <- data[data$cutangle == cutangle,]
	if (nrow(slice) == 0) break
	
	plot_name <- paste('cutangle_', idx, sep='')
	plot_name <- paste(plot_name, '.png', sep='')
	
	plot_title <- paste('cutangle = ', cutangle, sep='')

	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	
	plot <- wireframe(
		alignment ~ cutdepth * cuttilt, data = slice,
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

# plot cuttilt slices
idx <- 0
for (cuttilt in seq(-90, 90, by=18)) {

	slice <- data[data$cuttilt == cuttilt,]
	if (nrow(slice) == 0) break
	
	plot_name <- paste('cuttilt_', idx, sep='')
	plot_name <- paste(plot_name, '.png', sep='')
	
	plot_title <- paste('cuttilt = ', cuttilt, sep='')

	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	
	plot <- wireframe(
		alignment ~ cutdepth * cutangle, data = slice,
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
