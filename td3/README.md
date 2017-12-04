####### Compilation execution ######

Pour compiler :

     make

Pour executer :

     mpirun -np nombre_de_processus ./gemm_fox matrix_A.txt matrix_B.txt matrix_C.txt

Pour lancer les tests :

     make
     cd test
     python unit_test.py

Pour generer des matrices ;

     cd test
     python generate_matrix.py filename dimension

Pour clean :
     
     make clean

####### Courbes ######

Pour la courbe de speed up :
     
     cd test
     python perf_test.py output.csv np N

Cela produit un fichier csv que l'on peut afficher avec le programme plot_csv.py

     python plot_csv.py filename.csv
     