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