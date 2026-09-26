Exercice 2 - Les sept droits

J'ai ecrit un programme qui cree sept fenetres, une par droit desactive.
J'ai lance le programme et observe le comportement de chaque fenetre.
J'ai ensuite cherche dans le backend Linux (XLib) quels droits sont
reellement lus.

Le programme est depose a cote sous le nom c3-exo2_main.cpp. Il fait
60 lignes.

Datation de la mesure

Commande :

```
git log -1 --format="%h %ad" --date=short
```

Sortie brute :

```
9c3fad3 2026-09-13
```

Version de Jenga : 2.8.0.

Tableau des sept droits

1. resizable
   Effet attendu : la fenetre ne peut pas etre redimensionnee.
   Effet observe : la fenetre ne peut pas etre redimensionnee.
   Backend : lu. NkXLibWindow.cpp ligne 375 :

2. movable
   Effet attendu : la fenetre ne peut pas etre deplacee.
   Effet observe : la fenetre peut etre deplacee normalement.
   Backend : jamais lu. Aucune occurrence de movable dans
   NkXLibWindow.cpp.

3. closable
   Effet attendu : la fenetre ne peut pas etre fermee par
   l'utilisateur.
   Effet observe : la fenetre peut etre fermee normalement.
   Backend : jamais lu. Aucune occurrence de closable dans
   NkXLibWindow.cpp.

4. minimizable
   Effet attendu : la fenetre ne peut pas etre minimisee.
   Effet observe : la fenetre peut etre minimisee normalement.
   Backend : jamais lu. Aucune occurrence de minimizable dans
   NkXLibWindow.cpp.

5. maximizable
   Effet attendu : la fenetre ne peut pas etre maximisee.
   Effet observe : la fenetre peut etre maximisee normalement.
   Backend : jamais lu. Aucune occurrence de maximizable dans
   NkXLibWindow.cpp.

6. canFullscreen
   Effet attendu : la fenetre ne peut pas passer en plein ecran.
   Effet observe : la fenetre peut passer en plein ecran normalement.
   Backend : jamais lu. Aucune occurrence de canFullscreen dans
   NkXLibWindow.cpp.

7. frame
   Effet attendu : la fenetre est sans bordure.
   Effet observe : la fenetre est sans bordure.
   Backend : lu. NkXLibWindow.cpp ligne 350 :
       SetDecorated(config.frame);

Ce que ces ecarts montrent

Sur les sept droits, deux sont honores par le backend Linux :
resizable et frame. Les cinq autres sont declares dans
NkWindowConfig mais ne sont lus nulle part dans NkXLibWindow.cpp.

Un droit qu'aucun code ne lit est une reponse complete. Cela ne veut
pas dire que le droit est inutile : il peut etre lu par un autre
backend (Windows, macOS, Wayland, XCB, Cocoa). Mais sur Linux avec
XLib, il n'a aucun effet.

Citations du backend

Commande :

```
grep -n "resizable" Kernel/Runtime/NKWindow/src/NKWindow/Platform/XLib/NkXLibWindow.cpp
```

Sortie brute :

```
372:            // Size constraints : non-resizable -> taille fixe ; resizable -> taille MINI.
   375:                    if (!config.resizable) {
```

Commande :

```
grep -n "frame" Kernel/Runtime/NKWindow/src/NKWindow/Platform/XLib/NkXLibWindow.cpp
```

Sortie brute :

```
338:            // Fenetre SANS bordure (config.frame == false)
340:            // config.frame etait purement IGNORE ici : le gestionnaire de fenetres
350:            SetDecorated(config.frame);
842:            mConfig.frame = decorated;
864:            return mConfig.frame;
```

Commande :

```
grep -n "movable\|closable\|minimizable\|maximizable\|canFullscreen" Kernel/Runtime/NKWindow/src/NKWindow/Platform/XLib/NkXLibWindow.cpp
```

Sortie brute : aucune ligne.

Une remarque dans le code source

La ligne 340 du backend contient un commentaire qui dit :
config.frame etait purement IGNORE ici : le gestionnaire de fenetres.
Ce commentaire explique pourquoi frame a ete corrige. C'est la trace
d'un bug anterieur, et cela montre que le droit avait ete oublie
avant d'etre pris en charge.

Mesure faite le 26/09/2026.
Version de Jenga : 2.8.0.
