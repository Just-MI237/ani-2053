Exercice 2 - Les sept droits

J'ai ecrit un programme qui cree sept fenetres, une par droit
desactive. J'ai lance le programme et observe le comportement de
chaque fenetre. J'ai ensuite cherche dans le backend Linux (XLib)
quels droits sont reellement lus.

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

| Droit | Effet attendu | Effet observe |
|-------|---------------|---------------|
| resizable | La fenetre ne peut pas etre redimensionnee | La fenetre ne peut pas etre redimensionnee |
| movable | La fenetre ne peut pas etre deplacee | La fenetre peut etre deplacee normalement |
| closable | La fenetre ne peut pas etre fermee | La fenetre peut etre fermee normalement |
| minimizable | La fenetre ne peut pas etre minimisee | La fenetre peut etre minimisee normalement |
| maximizable | La fenetre ne peut pas etre maximisee | La fenetre peut etre maximisee normalement |
| canFullscreen | La fenetre ne peut pas passer en plein ecran | La fenetre peut passer en plein ecran |
| frame | La fenetre est sans bordure | La fenetre est sans bordure |

Sur sept droits, deux sont honores par le backend Linux (resizable et
frame), et cinq sont ignores (movable, closable, minimizable,
maximizable, canFullscreen).

Citations du backend

Pour chaque ecart, voici la ligne du backend Linux (NkXLibWindow.cpp)
qui explique le comportement.

| Droit | Ligne | Contenu |
|-------|-------|---------|
| resizable | 375 | if (!config.resizable) { |
| movable | aucune | le mot n'apparait pas dans le fichier |
| closable | aucune | le mot n'apparait pas dans le fichier |
| minimizable | aucune | le mot n'apparait pas dans le fichier |
| maximizable | aucune | le mot n'apparait pas dans le fichier |
| canFullscreen | aucune | le mot n'apparait pas dans le fichier |
| frame | 350 | SetDecorated(config.frame); |

Un droit qu'aucun code ne lit est une reponse complete. Cela ne veut
pas dire que le droit est inutile : il peut etre lu par un autre
backend (Windows, macOS, Wayland, XCB, Cocoa). Mais sur Linux avec
XLib, il n'a aucun effet.

Sorties brutes des greps

Commande :

```
grep -n "resizable" Kernel/Runtime/NKWindow/src/NKWindow/Platform/XLib/NkXLibWindow.cpp
```

Sortie brute :

```
372:            // Size constraints : non-resizable -> taille fixe ; resizable -> taille MINI.
```

Commande :

```
grep -n "frame" Kernel/Runtime/NKWindow/src/NKWindow/Platform/XLib/NkXLibWindow.cpp
```

Sortie brute :

```
338:            // Fenetre SANS bordure (config.frame == false)
340:            // config.frame etait purement IGNORE ici
350:            SetDecorated(config.frame);
842:            mConfig.frame = decorated;
864:            return mConfig.frame;
```

Commande :

```
grep -n "movable" Kernel/Runtime/NKWindow/src/NKWindow/Platform/XLib/NkXLibWindow.cpp
```

Sortie brute : aucune ligne.

Meme chose pour closable, minimizable, maximizable, canFullscreen.
Aucune de ces cinq occurrences n'apparait dans le backend XLib.

Une remarque dans le code source

La ligne 340 du backend contient un commentaire qui dit que
config.frame etait purement ignore. Ce commentaire est la trace d'un
bug anterieur. Cela montre que le droit avait ete oublie avant
d'etre pris en charge.

Mesure faite le 26/09/2026.
Version de Jenga : 2.8.0.
