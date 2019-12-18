#!/bin/bash

N=1000000000

# 0123456789

#a= '(?<=<).*?(?=>)'

gcc -o 01_array_sum.x 01_array_sum.c

gcc -o 06_touch_by_all.x 06_touch_by_all.c


echo -n 1, > time_elaps_as.txt

echo -n 1, > time_elaps_tba.txt

/usr/bin/time -a -o time_elaps_as.txt -f %e ./01_array_sum.x $N | grep -o -P '(?<=<).*?(?=>)' | awk '{print}' ORS=',' > walltime_as.txt

/usr/bin/time -a -o time_elaps_tba.txt -f %e ./06_touch_by_all.x $N | grep -o -P '(?<=<).*?(?=>)' | awk '{print}' ORS=',' > walltime_tba.txt

echo 1 >> walltime_as.txt

echo 1 >> walltime_tba.txt


gcc -fopenmp -o 01_array_sum.x 01_array_sum.c

gcc -fopenmp -o 06_touch_by_all.x 06_touch_by_all.c

for threads in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do

  echo -n ${threads}, >> time_elaps_as.txt

  echo -n ${threads}, >> time_elaps_tba.txt

  export OMP_NUM_THREADS=${threads}

  /usr/bin/time -a -o time_elaps_as.txt -f %e ./01_array_sum.x $N | grep -o -P '(?<=<).*?(?=>)' | awk '{print}' ORS=',' >> walltime_as.txt

  /usr/bin/time -a -o time_elaps_tba.txt -f %e ./06_touch_by_all.x $N | grep -o -P '(?<=<).*?(?=>)' | awk '{print}' ORS=',' >> walltime_tba.txt

  echo ${threads} >> walltime_as.txt

  echo ${threads} >> walltime_tba.txt
  
done
