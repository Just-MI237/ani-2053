Exercice 5 - Votre premier projet

J'ai cree un projet MonEssai dans le depot Nkentseu, je l'ai declare
au workspace, je l'ai verifie avec jenga info, puis je l'ai construit.

Les fichiers demandes sont deposes a cote de ce rapport :

- MonEssai.jenga : le fichier de projet
- main.cpp : le programme source

Etape 1 - Fichier de projet MonEssai.jenga

Le fichier est depose a cote sous le nom MonEssai.jenga. Il declare
un projet nomme MonEssai, de type consoleapp, ecrit en C++, avec la
norme C++17. Il ne depend d'aucun module du moteur : la liste passee
a nkentseudependson est vide. Il indique ou vont les fichiers
intermediaires (objdir) et ou va le binaire produit (targetdir).
Deux filtres precisent la chaine de compilation a utiliser :
TC_WINDOWS pour Windows, et clang-native pour Linux.

Etape 2 - Fichier source main.cpp

Le fichier est depose a cote sous le nom main.cpp. C'est un main qui
ne fait rien et qui retourne simplement 0.

Etape 3 - Declaration au workspace

Pour que Jenga voie le projet, il ne suffit pas de creer le fichier
.jenga. Il faut aussi l'inclure dans le workspace. J'ai ajoute ce
bloc a la fin de Nkentseu.jenga :

```
    with include("Applications/MonEssai/MonEssai.jenga"):

        pass
```

Le mot-cle pass est la parce que le bloc doit avoir un corps en Python.

Etape 4 - Verification avec jenga info

Commande :

```
jenga info | grep -i MonEssai
```

Sortie brute :

```
MonEssai                                    ConsoleApp    C++        No     Yes
```

Etape 5 - Construction

Commande :

```
jenga build --project MonEssai --config Debug
```

Sortie brute complete :

```

╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.8.0             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝

Loading workspace...
[NKCode] ATTENTION : aucun wheel Jenga trouve (dist/*.whl) -> le paquet n'aura PAS de Jenga embarque, et les boutons Construire/Executer seront inoperants. Produisez-le avec ./cri.sh dans le depot Jenga.

Configuration: Debug
Target:        Linux x86_64
Toolchain:     host-clang

Build Order (1 projects):
  1. MonEssai [CONSOLE_APP]


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: MonEssai                                                        Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
✓ Built: Build/Bin/Debug-Linux/MonEssai/MonEssai

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.33s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED                                 
════════════════════════════════════════════════════════════════════════════════
Projects Built:  1/1
Time:           0.33s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════

```

Le Build Order n'annonce qu'un seul projet : MonEssai. C'est normal,
le fichier de projet ne declare aucune dependance.

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
