## TDP6 Le jeu de la vie parallèle                                                                  
#### Usage

* Version séquentiel<br>
    Compilation : **make sequentiel**<br>
    Execution : *./sequentiel nb_iterations size*<br>
* Version OpenMP<br>
    Compilation : **make openmp**<br>
    Execution : *./openmp nb_iterations size*<br>
* Version pthread                         <br> 
    Compilation : **make pthread**<br>
    Execution : *./pthread nb_iterations size*<br> 
    Variable d'environnements : **MY_NUM_THREADS** <br>
* Version MPI <br>
    La version MPI ne comporte que la sous-version 'Communications Synchrones'<br>
     Compilation : **make mpi**<br>
    Execution : *mpirun -np N nb_iterations size* / N nombre de processus<br>  
    N doit être un carré parfait et doit diviser size<br>
------------
<br>
#### Analyse<br>
Pour analyser des courbes, il est possible d'utiliser *make courbe*. Il est nécessaire de n'avoir aucune sortie dans les programmes *excepté les temps*.
* plot_csv.py:
    Trace une courbe à partir d'un fichier à deux colonnes données sur matplotlib<br>
* script.sh<br>
    Réalise un fichier csv (speedup) liée aux différents programmes valides (openmp,phtread). Les paramètres peuvent être modifier dans le script. <br>
* make courbe<br>
    Encapsule les deux actions précédentes.<br>