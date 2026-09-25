Exercice 4 - Le fichier de projet annote

J'ai affiche le fichier avec la commande :

    cat -n Kernel/Foundation/NKPlatform/NKPlatform.jenga

Le contenu annote est ci-dessous. Chaque annotation porte une marque.
[teste] veut dire que j'ai retire la ligne, reconstruit, note le
message, puis remis la ligne. [deduit] veut dire que je n'ai pas
teste cette annotation, c'est une deduction raisonnable.

Debut du fichier annote

Ligne 1  | #!/usr/bin/env python3
              [deduit] Shebang. Dit au systeme d'executer ce fichier
                       avec Python 3.

Ligne 2  | # -*- coding: utf-8 -*-
              [deduit] Declare l'encodage UTF-8 pour les accents.

Lignes 3 a 8 | Docstring du module
              [deduit] Decrit NKPlatform : detection OS, arch,
                       compilateur, CPU. Module de fondation.

Ligne 10 | from Jenga import *
Ligne 11 | from jengaconfig import *
              [deduit] Importe toutes les fonctions de Jenga et de
                       jengaconfig.

Ligne 14 | with project(NKPlatform):
              [deduit] Declare un projet nomme NKPlatform.

Ligne 15 |     language(C++)
              [deduit] Le projet est en C++.

Ligne 16 |     cppdialect(C++20)
              [deduit] Norme C++20.

Ligne 17 |     location(.)
              [deduit] Le projet vit dans le dossier du fichier .jenga.

Lignes 19 a 23 | nkentseudependson([], selfexport=, extra_includes=)
              [teste] En retirant cette ligne et ses arguments, on
                      obtient :

                      Error loading workspace: unmatched ')'
                      (NKPlatform.jenga, line 23)
                      Failed to load workspace.

                      Le workspace ne charge meme pas. La parenthese
                      fermante reste seule et Python refuse de lire
                      le fichier.

Ligne 25 |     pchheader(pch/pch.h)
Ligne 26 |     pchsource(pch/pch.cpp)
              [teste] En retirant ces deux lignes, le build reussit
                      en 0.37s. Aucun message d'erreur. Seul effet :
                      les en-tetes sont inclus a chaque fichier au
                      lieu d'etre precompiles. Sur un petit module,
                      invisible.

Lignes 28 a 31 | files([...])
              [deduit] Liste des fichiers source. Le motif **.cpp
                       descend dans les sous-dossiers.

Ligne 33 |     objdir(...)
              [deduit] Ou vont les fichiers intermediaires.

Ligne 34 |     targetdir(...)
              [teste] En retirant cette ligne, le build reussit en
                      0.43s. Le binaire va dans un sous-dossier au
                      nom du projet :
                        Build/Lib/Debug-Linux/NKPlatform/NKPlatform.a
                      au lieu de :
                        Build/Lib/Debug-Linux/NKPlatform.a

Lignes 36 a 54 | Filtres par systeme
              [deduit] Choisit la chaine de compilation et les
                       options selon la plateforme cible.

Lignes 56 a 68 | Filtres par configuration (Debug, Release)
              [deduit] En Debug, optimisations desactivees et
                       symboles gardes. En Release, optimisations
                       activees et symboles retires.

Lignes 71 a 73 | with test(): testfiles([...])
              [deduit] Declare une suite de tests unitaires.

Fin du fichier annote

Datation de la mesure

Commande :

```
git log -1 --format="%h %ad" --date=short
```

Sortie brute :

```
9c3fad3 2026-09-13
```

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
