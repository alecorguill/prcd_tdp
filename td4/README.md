### Compilation execution ######
Pour compiler source

     make
     
Pour compiler les entités esclaves du code parallèle, on lance

    make slave_lu
L'executable slave_lu est crée et servira pour l'algorithme parallèle LU
     
Voir section Tests, si l'on veut compiler la partie test parallèle, 

### Tests ######
Pour lancer un test unitaire, on exécute

    make test
Un executable test_unitaire est crée dans le dossier courant et on peut lancer par exemple avec les paramètres suivants

    ./test_unitaire CSV_FLAG EPS TAILLE_MAX
CSV_FLAG : signifie que l'on veut obtenir des fichiers csv en sortie (1 pour True 0 pour False)

EPS : détermine l'erreur que l'on désire définr dans nos tests

TAILLE_MAX : est la taille maximale de la matrice qui s'agrandit exponentiellement dans nos tests.

Exemple
	
	./test_unitaire 1 10E-8 100

### Plot ######
Pour obtenir des courbes après avoir lancé 

    ./test_unitaire args
on lance le fichier script gnuplot plot.gpl présent dans le dossier test et crée en sortie des fichiers png

    gnuplot plot.gpl

