#!/bin/bash
#PBS -l nodes=1:ppn=20
#PBS -l walltime=00:10:00
#PBS -q regular

cd $PBS_O_WORKDIR

module load openmpi

#PBS Str_script_mpi_pi.sh

#PBS Wk_script_mpi_pi.sh

#PBS Str_script_mpi_sum.sh
