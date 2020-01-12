#!/bin/bash

nx=10000
ny=10000
xl=-2.05
yl=-1.5
xr=0.95
yl=1.5
imax=65535
input=("${nx}" "${ny}" "${xl}" "${yl}" "${xr}" "${yr}" "${imax}")


echo -n > wmandy_time.txt



gcc -o mandelbrot.x mandelbrot.c

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

./mandelbrot.x "${input[@]}" | grep -o -P '(?<=<).*?(?=>)' | awk '{print}' ORS=',' >> wmandy_time.txt

echo 1 >> wmandy_time.txt

done  



gcc -fopenmp -DOMP -o mandelbrot.x mandelbrot.c

for threads in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

  export OMP_NUM_THREADS=${threads}

  input=("${nx}" "$(("${threads}"*"${ny}"))" "${xl}" "${yl}" "${xr}" "${yr}" "${imax}")

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

  ./mandelbrot.x "${input[@]}" | grep -o -P '(?<=<).*?(?=>)' | awk '{print}' ORS=',' >> wmandy_time.txt

  echo ${threads} >> wmandy_time.txt

  #echo "${input[@]}"

done  

done
