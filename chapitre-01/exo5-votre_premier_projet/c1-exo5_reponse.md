Exercice 5 - Votre premier projet

J'ai cree un projet MonEssai dans le depot Nkentseu, je l'ai declare au
workspace, je l'ai verifie avec jenga info, puis je l'ai construit.

Les trois fichiers demandes sont deposes a cote de ce rapport :

- MonEssai.jenga : le fichier de projet
- main.cpp : le programme source
- La ligne d'inclusion ajoutee a Nkentseu.jenga est decrite ci-dessous

Etape 1 - Fichier de projet MonEssai.jenga

Le fichier est depose a cote sous le nom MonEssai.jenga. Il declare un
projet nomme MonEssai, de type consoleapp, ecrit en C++, avec la norme
C++17. Il ne depend d'aucun module du moteur : la liste passee a
nkentseudependson est vide. Il indique ou vont les fichiers
intermediaires (objdir) et ou va le binaire produit (targetdir). Deux
filtres precisent la chaine de compilation a utiliser : TC_WINDOWS
pour Windows, et clang-native pour Linux.

Etape 2 - Fichier source main.cpp

Le fichier est depose a cote sous le nom main.cpp. C'est un main qui ne
fait rien et qui retourne simplement 0.

Etape 3 - Declaration au workspace

Pour que Jenga voie le projet, il ne suffit pas de creer le fichier
.jenga. Il faut aussi l'inclure dans le workspace. J'ai ajoute ce bloc
a la fin de Nkentseu.jenga :

    with include("Applications/MonEssai/MonEssai.jenga"):

        pass

Le mot-cle pass est la parce que le bloc doit avoir un corps en Python.

Etape 4 - Verification avec jenga info

J'ai lance jenga info | grep -i "MonEssai" et voici la sortie brute :

MonEssai                                    ConsoleApp    C++        No     Yes

Etape 5 - Construction

J'ai lance jenga build --project MonEssai --config Debug.

Bloc de configuration annonce par Jenga :

Configuration: Debug
Target:        Linux x86_64
Toolchain:     host-clang

Ordre de construction :

Build Order (1 projects):
  1. MonEssai [CONSOLE_APP]

Compilation et liaison :

Project: MonEssai                                            Kind: CONSOLE_APP

Found 1 source file(s)
  [1/1] Compiled: main.cpp
Linking...
Built: Build/Bin/Debug-Linux/MonEssai/MonEssai

Bilan final :

BUILD COMPLETED
Projects Built:  1/1
Time:           0.41s
Status:         SUCCESS

Ce que j'ai retenu

- Un fichier .jenga seul ne suffit pas. Tant qu'il n'est pas inclus
  dans le workspace, Jenga ne le voit pas et jenga build --project
  MonEssai repondrait que le projet est introuvable.
- Le type du projet est decide par l'appel consoleapp() dans le fichier
  .jenga, pas par le nom du dossier.
- La cible du binaire est donnee par targetdir, qui utilise des
  variables comme cfg.buildcfg et cfg.system. C'est ce qui permet
  d'avoir un Debug et un Release cote a cote sans qu'ils s'ecrasent.

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
Dernier commit du depot Nkentseu : 9c3fad3, date du 2026-09-13.
