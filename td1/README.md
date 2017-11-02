###### Compilation ######

Pour compiler les source en objects :
     make

Pour compiler les tests :
     make test
Ceci va produire des éxecutables qui correspondent à chaque test :
     
     test_driver : Ensemble de tests unitaires pour toutes les fonctions
     test_perf_vecteur : Test du produit scalaire entre deux vecteurs. L'execution
     		         produit un fichier csv dans le répertoire test contenant les informations sur les
			 temps d'execution

     test_perf_matrice : même chose que test_perf_vecteur mais pour le produit
     		       	 de matrice

Pour afficher les courbes à partir des csv :
     python analyse_perf.py nomdufichier1.csv nomdufichier2.csv