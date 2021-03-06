# GreenDL

## Objectif
On a décidé de travailler en particulier sur la consommation de l'entrainment des réseaux de neurones en essayant de déterminer les paramètres (nombre de neurones par couche, nombre des couches, dimension des entrées, etc) qui influencent leurs consommations.

Pour ce faire, on part sur l'implémentation de deux logiciels, un logiciel de mesure et une IA bac à sable.

Le logiciel de mesure implémente une méthode générique pour mesurer la consommation de l'exécution d'un logiciel.

Tandis que l'IA bac à sable est un programme d'entrainment pour un réseau des neurones à convolution (classification d'images de chiens et de chats),  permettant de régler les paramètres principaux lors de son lancement.

La finalité étant d'utiliser le programme de mesure pour effectuer des benchmarks de l'IA pour des paramètres différents et d'interpréter les données obtenues (comprendre l'influence des différents paramètres sur la consommation, comparer a l'efficacitée des modèles entraîner, etc).

Enfin, le cheminement pourra peut être permettre de proposer notre propre solution pour évaluer la consommation d'un réseau de neurone ou au moins donner un indice (une grandeur représentative abstraite (sans unité) pour avoir une idée de classification par rapport aux autres réseaux) de leur consommation probable.


## Utilisation des logiciels
### Logiciel de mesure
Le logiciel de mesure se trouve dans le répèrtoire dev/measureTool/. 

La compillation s'effectue en appellent la commande **make** dans le répèrtoire dev/measureTool/sources/. Il est a noté qu'il dépend de deux bibliothèques additionnelles (libpowercap et libraplcap-powercap) qui sont comprisent dans le repos git (dev/measureTool/libs/) et sont linker statiquement avec le programme de mesure.

L'utilisation du binaire obtenut consiste a créer un fichier de configuration contenant sur la première ligne le nombre d'itération qu'un test doit être effectuer (pour mesurer la moyenne) et sur les lignes suivantes les commandes a appellées (un test par ligne qui est constituer par la commande a appellée comme en shell). Ensuite, le programme est appellé en lui passent le path vers sont fichier de configuration, le path vers un fichier de sortie général (qui contient une ligne par test en renseigent moyenne de consommation, temps et écart types), le path vers un répertoirs (qui doit exister) pour contenir les fichiers de sorties détaillé pour chaque test (les donnés ayant servies au calcul des moyennes) et le nombres de coeurs du cpu (non utiliser mais demander tout de même). Ce qui donne:
> sudo ./measureTool path\_conf\_file path\_generalOutput\_file path\_directory\_benchFiles nb\_cpu\_core

### IA
L'IA se trouve dans le répèrtoire dev/IA2/ et un jeu de données se trouve dans dev/dataSet/dog\_cat/.

L'IA est un programme en python3 qui entraine un réseau de neurone pour la classification d'images avec les paramètres en entrées. Le programme repose sur l'utilisation des librairies keras et tensorflow.

Pour l'utiliser, il est conseiller de créer un environement virtuel python dans le quel les dépendences sont installées. Pour ce faire (attention a la vertion de pip utilisée si plusieurs vertions de python cohabitent sur le système):

	# installe le nécessaire a la création d'environement virtuels
	pip install virtualenv
	# crée un environement virtuel python3 dans le répèrtoire IAvenv (dans le répèrtoire ou la commande est exécutée)
	virtualenv -p /usr/bin/python3 IAvenv
	# active l'environement vituel
	source IAvenv/bin/activate
	# installation des dépendences du programme
	pip install keras pillow tensorflow-cpu pandas sklearn
	# sortie de l'environement virtuel
	deactivate

L'utilisation en temps que telle nécessite d'activer l'environement virtuel et appeller le programme aveec les bon paramètres (voir sources pour une liste exaustive). Les paramètres principaux étant path\_vers\_dog\_cat\_folder qui est le chemain vers les données d'entrainement et path\_vers\_fichier\_sortie qui est le fichier qui contiendrat les précision des réseaux obtenut après entreinement (l'appel avec ces deux seul paramètres lance un entreinement pour des valeurs par défaux des autres paramètres). Un antrainement avec configuration minimale peut être lancé avec:

	# lancement de l'environement virtuel
	source IAvenv/bin/activate
	# appel au programme
	python CNN.py path_vers_dog_cat_folder path_vers_fichier_sortie nombre_epoque nombre_filtre size_kernel
	# sortie de l'environement (quand programme finit)
	deactivate

### Lancement des mesures
Pour lancer la prise des mesures, l'utilisation du fichier de configuration present sur gitlab (dev/measureTool/config) pour le programme de mesure est requis. Ainci que la création d'un répèrtoire pour les sorties du programme de mesure et un répèrtoire pour les sorties de l'IA (requier possiblement de modifier des paths dans le fichier de configuration de gitlab).
	
	# entrer dans l'environement virtuel
	source IAvenv/bin/activate
	# lancer le programme de mesure
	sudo ./measureTool path\_conf\_file\gitlab path\_generalOutput\_file path\_directory\_benchFiles nb\_cpu\_core
	# sortie de l'environement
	deactivate