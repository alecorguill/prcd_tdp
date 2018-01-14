#!/bin/bash


N_MAX=10000
RANGE=100

python test/generate_particules.py test/particules.txt $N_MAX
make clean
make sequentiel
echo "################# Sequentiel non bloqué ###############"
time ./sequentiel test/particules.txt test/output_seq.txt

for i in {1..5..1}
do
    np=$(($i*$i))
    echo "############## $np processus/bloc ################"
    ppb=$((N_MAX/$np))
    make clean
    make sequentiel_bc
    python test/generate_boxes.py test/particules_bc.txt $i $ppb
    time ./sequentiel_bc test/particules_bc.txt $np $RANGE test/output_bc_seq.txt

    make clean
    make parallel_bc
    time mpirun -np $np --mca pml ob1 ./parallel_bc test/particules_bc.txt $RANGE test/output_bc.txt

done
