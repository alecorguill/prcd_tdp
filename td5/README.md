# TDP5 NBody 
## Compilation
**Séquentielle** :

    make sequentiel
    make sequentiel_bc #pour la version bloc

**Parallèle** :

     make parallel
     make parallel_bc #pour la version bloc

**Il est important de realiser un 'make clean' entre le passage d'une version à un autre.**

## Exécution
Fichiers input:
&nbsp;&nbsp; Pour avoir une génération de fichiers de particules, on peut voir les scripts *generate_boxes.py* et *generate_particules.py*
    
    python test/generate_boxes.py output.txt nb_boites_par_colonne nb_particules 
    python test/generate_particules.py output.txt nb_particules
Les fichiers inputs simple et bloc ont une architecture différents. Si l'on veut transformer le fichier de particule bloc en un fichier de particule simple, on utilisera le script *encoder.py*. Les fichiers de sorties sont des vecteurs représentant les forces de chaque particule.

**Séquentielle** :
    
    ./sequentiel fichier_particule fichier_output

**Parallèle** :

     mpirun -np Np -- ./parallel_bc input taille_de_la_grille output

## Visualisation et tests
**Script test.sh** :
&nbsp;&nbsp;Un script a été codé pour faciliter l'exécution des tests qui se trouve à la racine du projet.
Test unitaires :
    
     make test
Il n'y a un qu'un fichier test present dans le dossier test/ : *test.c*

**Script test/tests.py** :
&nbsp;&nbsp; Le script test/tests.py permet de calculer l'erreur relative sur un fichier de force préablement obtenu

    python test/tests.py input
**Courbes** :     
    &nbsp;&nbsp; Pour les courbes, on pourra modifier le script gnuplot *plot.gpl*.

     