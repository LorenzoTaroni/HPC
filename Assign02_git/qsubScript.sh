#!/bin/bash
#PBS -l nodes=1:ppn=20
#PBS -l walltime=00:40:00
#PBS -q regular

cd $PBS_O_WORKDIR

module load gnu/4.9.2

#./Str_script_es0.sh

#./Str_mean_es0.sh

#./perf_script.sh

./prfx_script.sh


