!/usr/bin/bash
#PBS -l nodes=1:ppn=20
#PBS -l walltime=00:10:00
#PBS -q regular

cd $PBS_O_WORKDIR

module load openmpi

Str_script_mpi_pi.sh

Wk_script_mpi_pi.sh

Str_script_mpi_sum.sh
