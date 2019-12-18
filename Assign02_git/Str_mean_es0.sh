#!/bin/bash

N=1000000000

echo -n > time_w_as.txt

echo -n > time_w_tba.txt

gcc -lrt -std=c99 -fopenmp -o 01_array_sum.x 01_array_sum.c

gcc -lrt -std=c99 -fopenmp -o 06_touch_by_all.x 06_touch_by_all.c

for threads in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

  export OMP_NUM_THREADS=${threads}

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

  ./01_array_sum.x $N | grep -o -P '(?<=<).*?(?=>)' | awk '{print}' ORS=',' >> time_w_as.txt

  ./06_touch_by_all.x $N | grep -o -P '(?<=<).*?(?=>)' | awk '{print}' ORS=',' >> time_w_tba.txt

  echo ${threads} >> time_w_as.txt

  echo ${threads} >> time_w_tba.txt

done  

done
