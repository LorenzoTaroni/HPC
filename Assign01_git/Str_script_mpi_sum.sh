#!/bin/bash

mpicc -o SeR_mpi_sum.x SeR_mpi_sum.c

echo "1,2 " > sum1_time_elaps_strong.txt

for procs in 1 2 4 8 16 20 ; do

  echo -n ${procs}, >> sum1_time_elaps_strong.txt

  /usr/bin/time -a -o sum1_time_elaps_strong.txt -f %e mpirun -np ${procs} SeR_mpi_sum.x size1.txt > sum1_walltime_${procs}_strong.txt
  
done




echo "1,2 " > sum2_time_elaps_strong.txt

for procs in 1 2 4 8 16 20 ; do

  echo -n ${procs}, >> sum2_time_elaps_strong.txt

  /usr/bin/time -a -o sum2_time_elaps_strong.txt -f %e mpirun -np ${procs} SeR_mpi_sum.x size2.txt > sum2_walltime_${procs}_strong.txt
  
done




echo "1,2 " > sum3_time_elaps_strong.txt

for procs in 1 2 4 8 16 20 ; do

  echo -n ${procs}, >> sum3_time_elaps_strong.txt

  /usr/bin/time -a -o sum3_time_elaps_strong.txt -f %e mpirun -np ${procs} SeR_mpi_sum.x size3.txt > sum3_walltime_${procs}_strong.txt
  
done




echo "1,2 " > sum4_time_elaps_strong.txt

for procs in 1 2 4 8 16 20 ; do

  echo -n ${procs}, >> sum4_time_elaps_strong.txt

  /usr/bin/time -a -o sum4_time_elaps_strong.txt -f %e mpirun -np ${procs} SeR_mpi_sum.x size4.txt > sum4_walltime_${procs}_strong.txt
  
done
