#!/bin/bash

nx=300
ny=300
xl=-2.05
yl=-1.5
xr=0.95
yl=1.5
imax=65535
input=("${nx}" "${ny}" "${xl}" "${yl}" "${xr}" "${yr}" "${imax}")


echo -n > wmandy_time.txt



gcc -lrt -std=c99 -o mandelbrot.x mandelbrot.c

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

./mandelbrot.x "${input[@]}" | grep -o -P '(?<=<).*?(?=>)' | awk '{print}' ORS=',' >> wmandy_time.txt

echo 1 >> wmandy_time.txt

done  



gcc -lrt -std=c99 -fopenmp -DOMP -o mandelbrot.x mandelbrot.c

for threads in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

  export OMP_NUM_THREADS=${threads}

  new=$((${threads}*${ny}))

  input=("${nx}" "${new}" "${xl}" "${yl}" "${xr}" "${yr}" "${imax}")

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

  ./mandelbrot.x "${input[@]}" | grep -o -P '(?<=<).*?(?=>)' | awk '{print}' ORS=',' >> wmandy_time.txt

  echo ${threads} >> wmandy_time.txt

  #echo "${input[@]}"

done  

done
