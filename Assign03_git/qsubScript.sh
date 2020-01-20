#!/bin/bash
#PBS -l nodes=1:ppn=20
#PBS -l walltime=02:20:00
#PBS -q regular

cd $PBS_O_WORKDIR

module load gnu/4.9.2

module load openmpi


./Wk_mandy.sh

./Str_mandy.sh

./Wk_mandb_mpi.sh

./Str_mandb_mpi.sh
