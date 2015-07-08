#!/bin/bash

DIR=`dirname "$0"`

NAMES=("length" "width" "depth" "chfdepth" "chfangle" "cutdepth" "cutangle" "cuttilt" "tcpoffset")

for n in "${NAMES[@]}"
do
	echo "Plotting landscape for ${n}..."
	Rscript $DIR/landscape.R $n
done
