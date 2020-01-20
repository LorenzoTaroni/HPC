#!/bin/bash

nx=300
ny=300
xl=-2.05
yl=-1.5
xr=0.95
yl=1.5
imax=65535
input=("${nx}" "${ny}" "${xl}" "${yl}" "${xr}" "${yr}" "${imax}")



export OMP_NUM_THREADS=5



gcc -lrt -std=c99 -fopenmp -DOMP -DTth -o mandelbrot.x mandelbrot.c


./mandelbrot.x "${input[@]}"  > th_dynamic.txt


gcc -lrt -std=c99 -fopenmp -DOMP -DTth -o mandelbrot_no_dy.x mandelbrot_no_dy.c


./mandelbrot_no_dy.x "${input[@]}"  > th_no_dynamic.txt
