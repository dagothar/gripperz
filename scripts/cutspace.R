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
for (cutdepth in seq(0, 0.05, by=0.0025)) {

	# create slice data
	slice <- data[data$cutdepth == cutdepth,]
	if (nrow(slice) == 0) break
	
	# create slice name
	slice_name <- 'cutdepth'
	plot_title <- paste('cutdepth = ', cutdepth, sep='')

	# plot slice
	plot_name <- paste(slice_name, 'success', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		success ~ cutangle * cuttilt, data = slice,
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
		robustness ~ cutangle * cuttilt, data = slice,
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
		alignment ~ cutangle * cuttilt, data = slice,
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
		coverage ~ cutangle * cuttilt, data = slice,
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
		wrench ~ cutangle * cuttilt, data = slice,
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
		stress ~ cutangle * cuttilt, data = slice,
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
		volume ~ cutangle * cuttilt, data = slice,
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
		qlog ~ cutangle * cuttilt, data = slice,
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
# plot cutangle slices
idx <- 0
for (cutangle in seq(0, 180, by=18)) {

	# create slice data
	slice <- data[data$cutangle == cutangle,]
	if (nrow(slice) == 0) break
	
	# create slice name
	slice_name <- 'cutangle'
	plot_title <- paste('cutangle = ', cutangle, sep='')

	# plot slice
	plot_name <- paste(slice_name, 'success', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		success ~ cutdepth * cuttilt, data = slice,
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
		robustness ~ cutdepth * cuttilt, data = slice,
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
		alignment ~ cutdepth * cuttilt, data = slice,
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
		coverage ~ cutdepth * cuttilt, data = slice,
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
		wrench ~ cutdepth * cuttilt, data = slice,
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
		stress ~ cutdepth * cuttilt, data = slice,
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
		volume ~ cutdepth * cuttilt, data = slice,
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
		qlog ~ cutdepth * cuttilt, data = slice,
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
# plot cuttilt slices
idx <- 0
for (cuttilt in seq(-90, 90, by=18)) {

	# create slice data
	slice <- data[data$cuttilt == cuttilt,]
	if (nrow(slice) == 0) break
	
	# create slice name
	slice_name <- 'cuttilt'
	plot_title <- paste('cuttilt = ', cuttilt, sep='')

	# plot slice
	plot_name <- paste(slice_name, 'success', sep='_')
	plot_name <- paste(plot_name, idx, sep='_')
	plot_name <- paste(plot_name, '.png', sep='')
	png(filename=plot_name, width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
	trellis.par.set('axis.line', list(col='transparent'))
	plot <- wireframe(
		success ~ cutdepth * cutangle, data = slice,
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
		robustness ~ cutdepth * cutangle, data = slice,
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
		alignment ~ cutdepth * cutangle, data = slice,
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
		coverage ~ cutdepth * cutangle, data = slice,
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
		wrench ~ cutdepth * cutangle, data = slice,
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
		stress ~ cutdepth * cutangle, data = slice,
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
		volume ~ cutdepth * cutangle, data = slice,
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
		qlog ~ cutdepth * cutangle, data = slice,
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

