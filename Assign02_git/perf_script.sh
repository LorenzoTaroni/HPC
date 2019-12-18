#!/bin/bash

N=1000000000


gcc -fopenmp -o 01_array_sum.x 01_array_sum.c

gcc -fopenmp -o 06_touch_by_all.x 06_touch_by_all.c

#for threads in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

  #export OMP_NUM_THREADS=${threads}

export OMP_NUM_THREADS=20

sudo perf stat -r 100 -o perf01.txt -e cycles:u,instructions:u,cache-references:u,cache-misses:u,L1-dcache-load-misses:u ./01_array_sum.x $N 

sudo perf stat -r 100 -o perf06.txt -e cycles:u,instructions:u,cache-references:u,cache-misses:u,L1-dcache-load-misses:u ./06_touch_by_all.x $N

#done
