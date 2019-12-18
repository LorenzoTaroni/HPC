#!/bin/bash

N=10000

echo -n > prfx_time.txt



gcc -lrt -std=c99 -o prfx_sum.x prfx_sum.c

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

./prfx_sum.x $N | grep -o -P '(?<=<).*?(?=>)' | awk '{print}' ORS=',' >> prfx_time.txt

echo 1 >> prfx_time.txt

done  



gcc -lrt -std=c99 -fopenmp -DOMP -o prfx_sum.x prfx_sum.c

for threads in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

  export OMP_NUM_THREADS=${threads}

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

  ./prfx_sum.x $N | grep -o -P '(?<=<).*?(?=>)' | awk '{print}' ORS=',' >> prfx_time.txt

  echo ${threads} >> prfx_time.txt

done  

done
