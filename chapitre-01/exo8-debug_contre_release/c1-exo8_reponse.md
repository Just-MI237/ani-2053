Exercice 8 - Debug contre Release

J'ai construit le meme projet MonEssai dans les deux configurations,
et j'ai compare la taille du binaire et le temps de construction.
Pour que la comparaison ait un sens, j'ai d'abord ajoute a
MonEssai.jenga les filtres qui distinguent Debug et Release.
Sans ces filtres, les deux configurations produisent le meme binaire,
et l'ecart de taille serait nul.

Le fichier MonEssai.jenga utilise pour cet exercice est depose a cote
sous le nom MonEssai.jenga.

Datation de la mesure

Commande :

```
git log -1 --format="%h %ad" --date=short
```

Sortie brute :

```
9c3fad3 2026-09-13
```

Configuration Debug

Commande :

```
rm -rf Build/
jenga build --project MonEssai --config Debug
ls -l Build/Bin/Debug-Linux/MonEssai/MonEssai
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

-rwxr-xr-x 1 justmaxit justmaxit 17000 Sep 25 16:27 Build/Bin/Debug-Linux/MonEssai/MonEssai
```

Taille Debug : 17000 octets

Configuration Release

Commande :

```
rm -rf Build/
jenga build --project MonEssai --config Release
ls -l Build/Bin/Release-Linux/MonEssai/MonEssai
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

Configuration: Release
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
✓ Built: Build/Bin/Release-Linux/MonEssai/MonEssai

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.31s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED                                 
════════════════════════════════════════════════════════════════════════════════
Projects Built:  1/1
Time:           0.31s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════

-rwxr-xr-x 1 justmaxit justmaxit 15912 Sep 25 16:28 Build/Bin/Release-Linux/MonEssai/MonEssai
```

Taille Release : 15912 octets

Les deux nombres

- Taille Debug : 17000 octets
- Taille Release : 15912 octets

Le binaire Release est plus petit de 1088 octets, soit environ 6 pour
cent.

Ce qui explique la difference

MonEssai.jenga contient deux blocs with filter :

```
with filter("config:Debug"):
    defines(["_DEBUG", "NKENTSEU_DEBUG"])
    optimize("Off")
    symbols(True)

with filter("config:Release"):
    defines(["NDEBUG", "NKENTSEU_RELEASE"])
    optimize("Speed")
    symbols(False)
```

En Debug, optimize(Off) desactive les optimisations et symbols(True)
garde les symboles de debogage. C'est ce qui rend le binaire plus gros.

En Release, optimize(Speed) active les optimisations et symbols(False)
retire les symboles. C'est ce qui rend le binaire plus petit.

Sur le temps de construction

Les deux temps sont presque identiques : 0.33s en Debug, 0.31s en
Release. Le projet MonEssai est minuscule (un seul fichier main.cpp).
La compilation elle-meme prend une fraction de seconde, et le reste
du temps est le demarrage de Jenga et la lecture du workspace. Sur un
projet plus gros, les optimisations de Release ralentiraient la
compilation, parce qu'optimiser prend plus de travail que ne rien faire.

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
