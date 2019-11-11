#!/bin/bash
#PBS -l nodes=1:ppn=20
#PBS -l walltime=00:10:00
#PBS -q regular

cd $PBS_O_WORKDIR

module load openmpi

# Str_script_mpi_pi.sh

mpicc -o mpi_pi.x mpi_pi.c

echo "1,2 " > time_elaps_strong.txt

for procs in 1 2 4 8 16 20 ; do

  echo -n ${procs}, >> time_elaps_strong.txt

  /usr/bin/time -a -o time_elaps_strong.txt -f %e mpirun -np ${procs} mpi_pi.x 1000000000 > walltime_${procs}_strong.txt
  
done

# Wk_script_mpi_pi.sh

mpicc -o mpi_pi.x mpi_pi.c

  echo "1,2 " > time_elaps_weak.txt

  echo -n 1,10000000, >> time_elaps_weak.txt

  /usr/bin/time -a -o time_elaps_weak.txt -f %e mpirun -np 1 mpi_pi.x 10000000 > walltime_1_7_weak.txt
  
  echo -n 2,100000000, >> time_elaps_weak.txt

  /usr/bin/time -a -o time_elaps_weak.txt -f %e mpirun -np 2 mpi_pi.x 20000000 > walltime_2_8_weak.txt

  echo -n 4,1000000000, >> time_elaps_weak.txt

  /usr/bin/time -a -o time_elaps_weak.txt -f %e mpirun -np 4 mpi_pi.x 40000000 > walltime_4_9_weak.txt

  echo -n 8,10000000000, >> time_elaps_weak.txt

  /usr/bin/time -a -o time_elaps_weak.txt -f %e mpirun -np 8 mpi_pi.x 80000000 > walltime_8_10_weak.txt

  echo -n 16,100000000000, >> time_elaps_weak.txt

  /usr/bin/time -a -o time_elaps_weak.txt -f %e mpirun -np 16 mpi_pi.x 160000000 > walltime_16_11_weak.txt

  echo -n 20,1000000000000, >> time_elaps_weak.txt

  /usr/bin/time -a -o time_elaps_weak.txt -f %e mpirun -np 20 mpi_pi.x 200000000 > walltime_20_12_weak.txt

# Str_script_mpi_sum.sh

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
