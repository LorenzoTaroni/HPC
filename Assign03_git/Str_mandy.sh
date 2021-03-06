#!/bin/bash

nx=300
ny=300
xl=-2.05
yl=-1.5
xr=0.95
yl=1.5
imax=65535
input=("${nx}" "${ny}" "${xl}" "${yl}" "${xr}" "${yr}" "${imax}")


echo -n > smandy_time.txt

#echo "${nx}"*"${ny}"

#echo $(("${nx}"*"${ny}"))

#echo $((${nx}*${ny}))

gcc -lrt -std=c99 -DTIME -o mandelbrot.x mandelbrot.c

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

./mandelbrot.x "${input[@]}" | grep -o -P '(?<=<).*?(?=>)' | awk '{print}' ORS=',' >> smandy_time.txt

echo 1 >> smandy_time.txt

done  



gcc -lrt -std=c99 -fopenmp -DOMP -DTIME -o mandelbrot.x mandelbrot.c

for threads in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

  export OMP_NUM_THREADS=${threads}

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

  ./mandelbrot.x "${input[@]}" | grep -o -P '(?<=<).*?(?=>)' | awk '{print}' ORS=',' >> smandy_time.txt

  echo ${threads} >> smandy_time.txt

done  

done



gcc -lrt -std=c99 -fopenmp -DOMP -DTth -o mandelbrot.x mandelbrot.c


./mandelbrot.x "${input[@]}"  > th_dynamic.txt


gcc -lrt -std=c99 -fopenmp -DOMP -DTth -o mandelbrot_no_dy.x mandelbrot_no_dy.c


./mandelbrot_no_dy.x "${input[@]}"  > th_no_dynamic.txt


