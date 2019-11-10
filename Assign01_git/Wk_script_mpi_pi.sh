#!/bin/bash

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

  /usr/bin/time -a -o time_elaps_weak.txt -f %e mpirun -np 20 mpi_pi.x 2000000000 > walltime_20_12_weak.txt
