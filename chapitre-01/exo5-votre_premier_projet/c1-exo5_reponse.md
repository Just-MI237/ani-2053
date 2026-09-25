Exercice 5 - Votre premier projet

J'ai créé un projet MonEssai dans le dépôt Nkentseu, je l'ai déclaré au
workspace, je l'ai vérifié avec jenga info, puis je l'ai construit.

Étape 1 - Création des fichiers

J'ai créé deux fichiers dans Applications/MonEssai :

Le premier est le fichier de projet MonEssai.jenga. Il déclare un projet
nommé MonEssai, de type consoleapp, écrit en C++, avec la norme C++17.
Il ne dépend d'aucun module du moteur : la liste passée à
nkentseudependson est vide. Il indique aussi où vont les fichiers
intermédiaires (objdir) et où va le binaire produit (targetdir). Deux
filtres précisent la chaîne de compilation à utiliser : TC_WINDOWS pour
Windows, et clang-native pour Linux.

Le second fichier est src/main.cpp. C'est un main qui ne fait rien et
qui retourne simplement 0, comme l'énoncé le demande.

Étape 2 - Déclaration au workspace

Pour que Jenga voie le projet, il ne suffit pas de créer le fichier .jenga.
Il faut aussi l'inclure dans le workspace. J'ai donc ajouté ce bloc à la
fin de Nkentseu.jenga :

with include("Applications/MonEssai/MonEssai.jenga"):

    pass

Le mot-clé pass est là parce que le bloc doit avoir un corps en Python.

Étape 3 - Vérification avec jenga info

Commande lancée :

jenga info | grep -i "MonEssai"

Résultat :

MonEssai                                    ConsoleApp    C++        No     Yes

Le projet apparaît bien dans la liste, avec le type ConsoleApp. C'est
la preuve que le workspace l'a pris en compte.

Étape 4 - Construction

Commande lancée :

jenga build --project MonEssai --config Debug

Sortie obtenue :

Build Order (1 projects):
  1. MonEssai [CONSOLE_APP]

Project: MonEssai                                            Kind: CONSOLE_APP

Found 1 source file(s)
  [1/1] Compiled: main.cpp
Linking...
Built: Build/Bin/Debug-Linux/MonEssai/MonEssai

Build Successful                                          Time: 0.41s

BUILD COMPLETED
Projects Built:  1/1
Time:           0.41s
Status:         SUCCESS

Le projet est le seul dans l'ordre de construction parce qu'il ne dépend
d'aucun autre module. La construction a réussi en 0.41 seconde.

Ce que j'ai retenu

- Un fichier .jenga seul ne suffit pas. Tant qu'il n'est pas inclus dans
  le workspace, Jenga ne le voit pas et jenga build --project MonEssai
  répondrait que le projet est introuvable.
- Le type du projet est décidé par l'appel consoleapp() dans le fichier
  .jenga, pas par le nom du dossier.
- La cible du binaire est donnée par targetdir, qui utilise des variables
  comme cfg.buildcfg et cfg.system. C'est ce qui permet d'avoir un Debug
  et un Release côte à côte sans qu'ils s'écrasent.

Rapport rédigé le 25/09/2026.
Version de Jenga : 2.8.0.
Dernier commit du dépôt Nkentseu : 9c3fad3, daté du 2026-09-13.
