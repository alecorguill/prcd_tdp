#!/bin/bash

file=time.csv
nb_step=2
size=5040
rm -f $file

make clean
make openmp
make sequentiel
make pthread

echo "nb_proc,sequentiel,openmp,pthread" >> $file

for i in {2..20..2}
do
    echo -n "$i," >> $file
    export MY_NUM_THREADS=$i
    export OMP_NUM_THREADS=$i
    ./sequentiel $nb_step $size | tr '\n' ',' >> $file
    ./openmp $nb_step $size | tr '\n' ',' >> $file
    ./pthread $nb_step $size >> $file
    echo -n  >> $file
done


