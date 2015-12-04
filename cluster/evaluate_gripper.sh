#!/bin/sh

#These commands set up the Grid Environment for your job:
#PBS -N EvaluateGripper
#PBS -l nodes=1:ppn=2,walltime=0:30:00

# Move to the desired working directory e.g. /home/<ldap-user> or /home/<ldap-user>/my/work/directory
cd /home/awolniakowski/test
ulimit -s 80000

# Do things
time evaluate_gripper --dwc ${DWC} --td ${TD} --gripper ${GRIPPER} --result ${RESULT} --grasps ${GRASPS} -n ${NGRASPS} -t 2 --aliR 0.001 -p cutdepth -v "${VALUE}"
