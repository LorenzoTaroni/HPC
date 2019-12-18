#!/usr/bin/bash
#PBS -l nodes=1:ppn=20
#PBS -l walltime=00:10:00
#PBS -q regular

cd $PBS_O_WORKDIR

module load gcc/4.8.2

./Str_script_es0.sh

./Str_mean_es0.sh

./prfx_script.sh

./perf_script.sh
