#!/bin/bash

echo "1,2 " > time_elaps_strong.txt

for procs in 1 2 4 8 16 20 ; do

  echo -n ${procs}, >> time_elaps_strong.txt

  /usr/bin/time -a -o time_elaps_strong.txt -f %e mpirun -np ${procs} mpi_pi.x 10000000 > walltime_${procs}_strong.txt
  
done
