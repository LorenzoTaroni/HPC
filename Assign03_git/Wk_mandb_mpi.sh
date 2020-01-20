#!/bin/bash

nx=300
ny=300
xl=-2.05
yl=-1.5
xr=0.95
yl=1.5
imax=65535
input=("${nx}" "${ny}" "${xl}" "${yl}" "${xr}" "${yr}" "${imax}")


#echo -n > wmandy_time_mpi.txt

mpicc -lrt -std=c99 -o mandb_mpi.x mandb_mpi.c



for proc in 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

  new=$((${proc}*${ny}))

  input=("${nx}" "${new}" "${xl}" "${yl}" "${xr}" "${yr}" "${imax}")

  mpirun -np ${proc} mandb_mpi.x "${input[@]}"  > wmandy_time_${proc}_mpi.txt

  #echo ${proc} >> wmandy_time_${proc}_mpi.txt

done  


