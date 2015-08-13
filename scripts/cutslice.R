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

# Read data
args <- commandArgs(T)
(filename <- args[1])

data <- read.csv(filename)

# plot success
png(filename='success.png', width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
trellis.par.set('axis.line', list(col='transparent'))
plot <- wireframe(
	success ~ x * y, data = data,
	zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
	scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
	drape=T, col.regions=COL_SUCCESS, colorkey=F,
	zlab=list(rot=90),
	par.settings=theme.novpadding,
	zoom=0.9
)
print(plot)
dev.off()

# plot robustness
png(filename='robustness.png', width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
trellis.par.set('axis.line', list(col='transparent'))
plot <- wireframe(
	robustness ~ x * y, data = data,
	zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
	scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
	drape=T, col.regions=COL_ROBUSTNESS, colorkey=F,
	zlab=list(rot=90),
	par.settings=theme.novpadding,
	zoom=0.9
)
print(plot)
dev.off()

# plot alignment
png(filename='alignment.png', width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
trellis.par.set('axis.line', list(col='transparent'))
plot <- wireframe(
	alignment ~ x * y, data = data,
	zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
	scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
	drape=T, col.regions=COL_ALIGNMENT, colorkey=F,
	zlab=list(rot=90),
	par.settings=theme.novpadding,
	zoom=0.9
)
print(plot)
dev.off()

# plot coverage
png(filename='coverage.png', width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
trellis.par.set('axis.line', list(col='transparent'))
plot <- wireframe(
	coverage ~ x * y, data = data,
	zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
	scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
	drape=T, col.regions=COL_COVERAGE, colorkey=F,
	zlab=list(rot=90),
	par.settings=theme.novpadding,
	zoom=0.9
)
print(plot)
dev.off()

# plot wrench
png(filename='wrench.png', width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
trellis.par.set('axis.line', list(col='transparent'))
plot <- wireframe(
	wrench ~ x * y, data = data,
	zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
	scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
	drape=T, col.regions=COL_WRENCH, colorkey=F,
	zlab=list(rot=90),
	par.settings=theme.novpadding,
	zoom=0.9
)
print(plot)
dev.off()

# plot stress
png(filename='stress.png', width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
trellis.par.set('axis.line', list(col='transparent'))
plot <- wireframe(
	stress ~ x * y, data = data,
	zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
	scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
	drape=T, col.regions=COL_STRESS, colorkey=F,
	zlab=list(rot=90),
	par.settings=theme.novpadding,
	zoom=0.9
)
print(plot)
dev.off()

# plot volume
png(filename='volume.png', width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
trellis.par.set('axis.line', list(col='transparent'))
plot <- wireframe(
	volume ~ x * y, data = data,
	zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
	scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
	drape=T, col.regions=COL_VOLUME, colorkey=F,
	zlab=list(rot=90),
	par.settings=theme.novpadding,
	zoom=0.9
)
print(plot)
dev.off()

# plot quality
png(filename='quality.png', width=PLOT_W, height=PLOT_H, units="px", pointsize=PTSIZE)
trellis.par.set('axis.line', list(col='transparent'))
plot <- wireframe(
	qlog ~ x * y, data = data,
	zlim=c(0, 1), screen=list(z=PLOT_ANGLE_Z, x=PLOT_ANGLE_X),
	scales=list(arrows=F, col='black', tick.number=N_TICKS, axs='i'),
	drape=T, col.regions=COL_QUALITY, colorkey=F,
	zlab=list(rot=90),
	par.settings=theme.novpadding,
	zoom=0.9
)
print(plot)
dev.off()
