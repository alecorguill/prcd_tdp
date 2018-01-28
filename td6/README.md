## TDP6 Le jeu de la vie parallèle                                                                  
#### Usage

* Version séquentiel
    Compilation : **make sequentiel**
    Execution : *./sequentiel nb_iterations size*
* Version OpenMP
    Compilation : **make openmp**
    Execution : *./openmp nb_iterations size*
* Version pthread                          
    Compilation : **make pthread**
    Execution : *./pthread nb_iterations size* 
    Variable d'environnements : **MY_NUM_THREADS** 
* Version MPI 
    La version MPI ne comporte que la sous-version 'Communications Synchrones'
     Compilation : **make mpi**
    Execution : *mpirun -np N nb_iterations size* / N nombre de processus  
    N doit être un carré parfait et doit diviser size
------------

#### Analyse
Pour analyser des courbes, il est possible d'utiliser *make courbe*. Il est nécessaire de n'avoir aucune sortie dans les programmes *excepté les temps*. Pour véri
* plot_csv.py:
    Trace une courbe à partir d'un fichier à deux colonnes données sur matplotlib
* script.sh
    Réalise un fichier csv (speedup) liée aux différents programmes valides (openmp,phtread). Les paramètres peuvent être modifier dans le script. 
* make courbe
    Encapsule les deux actions précédentes.