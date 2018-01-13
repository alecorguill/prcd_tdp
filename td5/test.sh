# rm test/*.txt
# python test/generate_boxes.py test/particules_bc.txt 2 500
# python test/encoder.py test/particules_bc.txt test/particules.txt
make clean
make sequentiel
make sequentiel_bc
time ./sequentiel test/particules.txt test/output_seq.txt
time ./sequentiel_bc test/particules_bc.txt 4 100 test/output_bc_seq.txt

# make clean
# make parallel_bc
# time mpirun -np 4 --mca pml ob1 ./parallel_bc test/particules_bc.txt 100 test/output_bc.txt

python test/tests.py test/output_bc_seq.txt test/output_seq.txt 
# python test/tests.py test/output_bc.txt test/output_seq.txt 
