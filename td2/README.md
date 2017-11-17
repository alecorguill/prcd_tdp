######## COMPILATION ##########
Pour compiler les code sequentiel :
     make sequentiel

Pour compiler le code parallel :
     make parallel

###### Pour executer ######
En sequentiel :
   ./sequentiel fichier_particule fichier_output

En parallel :
   mpirun -np nb_processus fichier_particule fichier_output

###### Pour lancer les tests (et les courbes)########

Pour les tests unitaires :
     make test

Pour lancer les courbes :     
     cd test
     python analyse_perf.py fichier_input fichier output
    
Cela va produire 4 fichier csv qu'il est possible d'afficher avec le programme
plot_csv.py.
	python plot_csv.py fichier.csv titre [LABEL X] [LABEL Y]

Pour lancer une visualisation il faut d'abord produire un fichier de resultat avec
l'execution en parallèle ou en séquentiel. Ensuite il faut se rendre dans le 
répertoire test et lancer visu.py

Exemple :
	make sequentiel
	./sequentiel fichier_particule fichier_output
	cd test
	python visu.py fichier_output
     