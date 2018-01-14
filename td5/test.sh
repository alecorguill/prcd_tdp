#!/bin/bash


N_MAX=10000
RANGE=100

make clean

for i in {1..5..1}
do
    np=$(($i*$i))
    echo "############## $np processus/bloc ################"
    ppb=$((N_MAX/$np))
    make clean
    make sequentiel_bc
    make sequentiel
    python test/generate_boxes.py test/particules_bc.txt $i $ppb
    python test/encoder.py test/particules_bc.txt particules.txt
    echo "################# Sequentiel non bloqué ###############"
    time ./sequentiel test/particules.txt test/output_seq.txt
    echo "################# Sequentiel bloqué ###############"
    time ./sequentiel_bc test/particules_bc.txt $np $RANGE test/output_bc_seq.txt
    
    make clean
    make parallel_bc
    echo "################# Parallele ###############"
    time mpirun -np $np --mca pml ob1 ./parallel_bc test/particules_bc.txt $RANGE test/output_bc.txt

done
