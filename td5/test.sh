make clean
make sequentiel
make sequentiel_bc
./sequentiel test/particules.txt test/output_seq.txt
./sequentiel_bc test/particules_bc.txt 4 100 test/output_bc_seq.txt

make clean
make parallel_bc
mpirun -np 4 --mca pml ob1 ./parallel_bc test/particules_bc.txt 100 test/output_bc.txt

python test/tests.py test/output.txt test/output_seq.txt 
python test/tests.py test/output_bc.txt test/output_seq.txt 
