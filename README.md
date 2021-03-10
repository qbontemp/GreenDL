# GreenDL

[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/qbontemp/GreenDL/master?filepath=Output_plots.ipynb)

## Objectif
Ce dépôt contient les outils développés dans le cadre d'un travail de fin de bachelier en sciences informatiques et qui a été présenté pendant le Printemps de Sciences 2021. Il traite sur la consommation de l'entrainement des réseaux de neurones CNN en essayant d'explorer deux paramètres qui semblaient pouvoir influencer cette consommation (taille du noyau et nombres de filtres sur la couche de convolution).

Pour ce faire, deux logiciels ont été implémentés, un logiciel de mesure et une IA bac à sable.

Le logiciel de mesure implémente une méthode générique pour mesurer la consommation de l'exécution d'un logiciel.

Tandis que l'IA bac à sable est un programme d'entrainement pour un réseau des neurones à convolution classifiant des images de chiens et de chats, et qui permet de régler les paramètres principaux lors de son lancement.


## Utilisation des logiciels

Le programme de mesure est uniquement utilisable sur linux, et peut-être sur d'autres systèmes UNIX mais cela n'a pas été testé.
De fait, les explications pour son utilisation ne concernent que les systèmes linux (debian-like de préférence).

### Logiciel de mesure
Le logiciel de mesure se trouve dans le répertoire dev/measureTool/. 

La compilation s'effectue en appellent la commande **make** dans le répertoire dev/measureTool/sources/. Il est à noter qu'il dépend de deux bibliothèques additionnelles (libpowercap et libraplcap-powercap refs: https://github.com/powercap). Elles sont présentes dans le dépôt git dans dev/measureTool/libs/ et sont liées statiquement avec le programme de mesure.

L'utilisation du binaire obtenu consiste à créer un fichier de configuration contenant :
* sur la première ligne :
	* le nombre d'itération qu'un test doit effectuer (pour mesurer la moyenne arithmétique).
* sur les lignes suivantes :
	* les commandes à appeler (un test par ligne, qui est constitué par la commande shell à appeler).

Ensuite, le programme est appelé en lui passent dans l'ordre : 
* le chemin vers son fichier de configuration.
* le chemin vers un fichier de sortie général (qui contiendra une ligne par test en fournissant la moyenne de consommation, le temps et les écart types)
* le chemin vers un répertoire déjà existant qui contiendra les fichiers de sortie détaillés pour chaque test (les données ayant servies au calcul des moyennes)
* le nombres de coeurs du CPU. 

Exemple d'utilisation :
> sudo ./measureTool path\_conf\_file path\_generalOutput\_file path\_directory\_benchFiles nb\_cpu\_core


### IA
L'IA se trouve dans le répertoire dev/IA2/ et son jeu de données se trouve dans dev/dataSet/dog\_cat/.

L'IA est un programme en python3 qui entraine un réseau de neurones pour la classification d'images selon les paramètres fournis en entrée. Le programme repose sur l'utilisation des librairies Keras et TensorFlow.

Pour l'utiliser, il est conseillé de créer un environnement virtuel python dans le quel les dépendances nécessaires sont installées. Pour ce faire (attention a la version de pip utilisée si plusieurs versions de python cohabitent sur le système):

	# installe le nécessaire à la création de l'environnement virtuel
	pip install virtualenv
	# crée un environnement virtuel python3 dans le répertoire IAvenv (dans le répertoire ou la commande est exécutée)
	virtualenv -p /usr/bin/python3 IAvenv
	# active l'environnement virtuel
	source IAvenv/bin/activate
	# installation des dépendences du programme
	pip install keras pillow tensorflow-cpu pandas sklearn
	# sortie de l'environnement virtuel
	deactivate

L'utilisation nécessite d'activer l'environnement virtuel et d'appeller le programme avec les bons paramètres (voir sources pour une liste exhaustive). Les paramètres principaux étant path\_vers\_dog\_cat\_folder qui est le chemin vers les données d'entrainement et path\_vers\_fichier\_sortie qui est le fichier qui contiendra les précisions des réseaux obtenus après entrainement (l'appel avec ces deux seuls paramètres lance un entrainement pour des valeurs par défaut des autres paramètres). Un entrainement avec configuration minimale peut être lancé avec :

	# lancement de l'environnement virtuel
	source IAvenv/bin/activate
	# appel du programme
	python CNN.py path_vers_dog_cat_folder path_vers_fichier_sortie nombre_epoque nombre_filtre size_kernel
	# sortie de l'environnement (lorsque le programme est fini)
	deactivate

### Lancement des mesures
Pour lancer la prise des mesures, l'utilisation d'un fichier de configuration est requise. Un exemple de fichier de config est présenté dans : dev/measureTool/config. De même, la création d'un répertoire pour les sorties du programme de mesure et d’un répertoire pour les sorties de l'IA est aussi demandée (si le fichier config du dépôt git est utilisé, les chemins vers ces répertoires devront potentiellement ils aussi être modifiés).
	
	# entrer dans l'environnement virtuel
	source IAvenv/bin/activate
	# lancer le programme de mesure
	sudo ./measureTool path_conf_file_gitlab path_generalOutput_file path_directory_benchFiles nb_cpu_core
	# sortie de l'environnement
	deactivate