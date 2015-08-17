#!/bin/sh

#These commands set up the Grid Environment for your job:
#PBS -N GripperEvaluation
#PBS -l nodes=2:ppn=8,walltime=0:10:00

cd ${DIR}
ulimit -s 80000

EXE=/home/awolniakowski/gripperz/bin/gripper_evaluate

${EXE} -c 4 -g 1000 -r 0 -v "0.1 0.025 0.02 0.5 45 0 0 72 0.025 0 0.05 25"  --dwc /home/awolniakowski/grippers/scenes/rotor/Scene.dwc.xml --td /home/awolniakowski/grippers/scenes/rotor/task1.td.xml --samples /home/awolniakowski/grippers/scenes/rotor/samples1.xml --name .gripper
