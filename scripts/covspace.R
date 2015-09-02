# This script is used to plot covspace data:
# a plot of alignment in slices for length, depth & tcp offset
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
PLOT_ANGLE_Z = 35
PLOT_ANGLE_X = -65
N_TICKS = 10
PTSIZE = 12
PLOT_W = 1024
PLOT_H = 768

# Colors
COL_SUCCESS = colorRampPalette(c('red', 'white'))(100)
COL_ROBUSTNESS = colorRampPalette(c('darkred', 'white'))(100)
COL_ALIGNMENT = colorRampPalette(c('orange', 'white'))(100)
COL_COVERAGE = colorRampPalette(c('green', 'white'))(100)
COL_WRENCH = colorRampPalette(c('blue', 'white'))(100)
COL_STRESS = colorRampPalette(c('violet', 'white'))(100)
COL_VOLUME = colorRampPalette(c('cyan', 'white'))(100)
COL_QUALITY = colorRampPalette(c('black', 'white'))(100)

# Script
args <- commandArgs(T)
(filename <- args[1])

data <- read.csv(filename)

# plot cutdepth slices
idx <- 0
for (x in seq(0, 0.1, by=0.01)) {

	# create slice data
	slice <- data[data$length == x,]
	if (nrow(slice) == 0) break
	
	# create slice name
	slice_name <- 'length'
	plot_title <- paste('length = ', x, sep='')

	# plot slice
	plot_name <- paste(slice_name, 'success', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		success ~ depth * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_SUCCESS, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'robustness', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		robustness ~ depth * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_ROBUSTNESS, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'alignment', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		alignment ~ depth * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_ALIGNMENT, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'coverage', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		coverage ~ depth * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_COVERAGE, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'wrench', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		wrench ~ depth * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_WRENCH, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'stress', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		stress ~ depth * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_STRESS, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'volume', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		volume ~ depth * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_VOLUME, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'quality', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		qlog ~ depth * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_QUALITY, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	idx <- idx + 1
}

########################################################################
# plot depth slices
idx <- 0
for (x in seq(0, 0.025, by=0.0025)) {

	# create slice data
	slice <- data[data$depth == x,]
	if (nrow(slice) == 0) break
	
	# create slice name
	slice_name <- 'depth'
	plot_title <- paste('depth = ', x, sep='')

	# plot slice
	plot_name <- paste(slice_name, 'success', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		success ~ length * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_SUCCESS, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'robustness', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		robustness ~ length * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_ROBUSTNESS, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'alignment', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		alignment ~ length * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_ALIGNMENT, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'coverage', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		coverage ~ length * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_COVERAGE, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'wrench', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		wrench ~ length * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_WRENCH, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'stress', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		stress ~ length * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_STRESS, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'volume', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		volume ~ length * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_VOLUME, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'quality', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		qlog ~ length * tcpoffset, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_QUALITY, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	idx <- idx + 1
}

########################################################################
# plot tcpoffset slices
idx <- 0
for (x in seq(0, 0.1, by=0.01)) {

	# create slice data
	slice <- data[data$tcpoffset == x,]
	if (nrow(slice) == 0) break
	
	# create slice name
	slice_name <- 'tcpoffset'
	plot_title <- paste('tcpoffset = ', x, sep='')

	# plot slice
	plot_name <- paste(slice_name, 'success', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		success ~ length * depth, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_SUCCESS, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'robustness', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		robustness ~ length * depth, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_ROBUSTNESS, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'alignment', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		alignment ~ length * depth, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_ALIGNMENT, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'coverage', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		coverage ~ length * depth, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_COVERAGE, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'wrench', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		wrench ~ length * depth, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_WRENCH, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'stress', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		stress ~ length * depth, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_STRESS, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'volume', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		volume ~ length * depth, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_VOLUME, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	plot_name <- paste(slice_name, 'quality', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		qlog ~ length * depth, data = slice,
		zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
		scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
		drape=T, col.regions=COL_QUALITY, colorkey=F,
		main=list(label=plot_title, cex=2), xlab=list(cex=2), ylab=list(cex=2),
		zlab=list(rot=90, cex=2),
		par.settings=theme.novpadding,
		zoom=0.9
	)
	print(plot)
	dev.off()
	
	idx <- idx + 1
}

