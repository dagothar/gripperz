#!/bin/sh

#These commands set up the Grid Environment for your job:
#PBS -N EvaluateLandscape
#PBS -l nodes=1:ppn=2,walltime=0:30:00

# Move to the desired working directory e.g. /home/<ldap-user> or /home/<ldap-user>/my/work/directory
cd ${DIRECTORY}
ulimit -s 80000

# Do things
echo "evaluate_gripper --dwc ${DWC} --td ${TD} --gripper ${GRIPPER} --result ${RESULT} --grasps ${GRASPS} --save_grasps ${SAVE} -n ${NGRASPS} -t ${THREADS} -p ${PARAM} -v '${VALUE}' --seed ${SEED} -f ${FILTERS} --aliP ${ALIP} -- aliA ${ALIA}"
evaluate_gripper --dwc ${DWC} --td ${TD} --gripper ${GRIPPER} --result ${RESULT} --grasps ${GRASPS} --save_grasps ${SAVE} -n ${NGRASPS} -t ${THREADS} -p ${PARAM} -v "${VALUE}" --seed ${SEED} -f ${FILTERS} --aliP ${ALIP} --aliA ${ALIA}
