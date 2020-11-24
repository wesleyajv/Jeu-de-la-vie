Jeu de la vie
=============

Ce [dépôt](https://git.unistra.fr/wajavon/jeudelavie) contient un
exemple de documentation assistée avec l'outil Doxygen.

Prérequis
---------

Les paquets suivants sont requis pour générer correctement la documentation :

- Doxygen
  - Pour générer la documentation automatiquement.
- Graphviz (DOT)
  - Pour générer les graphes.

Pour les installer :

### Ubuntu/Debian

~~~{.sh}
sudo apt install graphviz doxygen
~~~

Générer la documentation
------------------------

~~~{.sh}
make doc
~~~

Structure des fichiers
----------------------

- / racine
	- src/ contient toutes les sources (*.c)
	- lib/ contient les librairies (*.a)
	- include/ contient toutes les en-têtes de fonctions et structures du programme (*.h)
	- grilles/ contient l'ensemble des grilles disponibles




Commandes importantes
---------------------

### Compiler le programme

~~~{.sh}
make
~~~
Le fichier exécutable se trouve au chemin suivant : /bin/main
Par défaut, le programme est compilé en mode graphique (Cairo et X requis).

Pour compiler en mode texte :
~~~{.sh}
make MODE=TEXTE
~~~
### Lancer le programme
~~~{.sh}
./bin/main <fichier grille>
~~~

Les grilles se trouvent dans le répertoire /grilles/ et ont comme nom de fichier "grilleX.txt".

Exemple :
~~~{.sh}
./bin/main grilles/grille2.txt
~~~

### Générer la documentation (Doxygen)

~~~{.sh}
doxygen
~~~
Il suffit ensuite d'ouvrir le fichier /doc/html/index.html pour accéder à l'intégralité de la documentation

### Générer une archive du programme

~~~{.sh}
make dist
~~~
L'archive sera créée dans le répertoire /dist

### Nettoyer les artefacts de compilation

~~~{.sh}
make clean
~~~

Fonctionnement
---------

### Quitter le programme
~~~{.sh}
q (clic droit avec Cairo)
~~~

### Faire évoluer la grille
~~~{.sh}
enter (clic gauche avec Cairo)
~~~

### Initialiser une nouvelle grille à partir de son numéro
~~~{.sh}
n <numéro>
~~~


### Activer/Désactiver le vieillissement des cellules
~~~{.sh}
v
~~~

### Activer/Désactiver le comptage cyclique des cellules
~~~{.sh}
c 
~~~

### Temps d'oscillation de la grille
~~~{.sh}
o
~~~

Versioning
---------
~~~{.sh}
- 1.0 : Code du niveau 1 achevé, allocation et free's correctement implémentés, no more segmentation fault, n command allows to upload another file.  
- 1.1 : Fichiers correctement séparés en dossiers: src/, include/, bin/ et obj/ qui n'est que lors de la compilation et supprimé lors du make clean.  
- 1.2 : Niveau 1 correctement commenté.  
- 1.3 : Doxyfile et donc fin du niveau 1.  
- 1.3.1 : Les graphs crées par la doc m'ont permis de voir que plusieurs de mes fichiers faisant de multiples #include alors qu'un seul, voire deux, suffisaient grâce à 'l'héritage'.  
- 2.0 : Début du niveau 2 et implémentation de l'affichage du temps d'évolution.  
- 2.1 : Affichage du temps d'évolution, fonction de calcul cylique ou non et affichage de l'âge des cellues implémentés et correctement documenté. Donc fin du niveau2.  
- 2.1.1 : Changements mineurs dans le makefile.  
- 2.1.2 : Ajout d'une fonction qui gère le signal suivant: CTRL+C  
- 2.1.3 : Problème d'affichage lors du chargement d'une nouvelle grille est géré.  
- 2.1.4 : Ajout du dossier grilles et du Doxyfile lors de la génération du dossier compressé.  
- 3.0 : Mode avec les cellules non-viables terminé
- 4.0 : Cairo développé
- 5.0 : Les temps d'oscillation sont dévéloppés, manque bibliothèque
- 5.1 : Implémenation de la compilation de bib
- 5.1.1 : Changement mineur ReadMe, Jeu de la vie terminé.
~~~

