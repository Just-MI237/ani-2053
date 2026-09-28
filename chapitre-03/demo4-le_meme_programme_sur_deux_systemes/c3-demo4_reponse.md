Demonstration 4 - Le meme programme sur deux systemes

J'ai ecrit un programme qui n'utilise que le header de detection de
plateforme du moteur (NkPlatformDetect.h) et qui affiche ce que le
preprocesseur a decide pour lui. Je l'ai compile deux fois : une pour
Linux, une pour Windows, depuis le meme fichier source, sans rien
changer entre les deux.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que fait le programme

Le programme inclut NkPlatformDetect.h, puis affiche :

- le nom du compilateur, lu depuis les macros __clang__ et __GNUC__,
- les macros de plateforme, lues avec #if defined,
- la macro de categorie (NKENTSEU_PLATFORM_DESKTOP),
- l'architecture, lue depuis __x86_64__ et __aarch64__,
- les tailles des types de base, avec sizeof.

Aucune de ces valeurs n'est ecrite en dur. Toutes viennent du
compilateur et du header.

Les deux compilations

Commande Linux :

    clang++ -std=c++17 -I ~/Projets/Nkentseu/Kernel/Foundation/NKPlatform/src test_plateforme.cpp -o test_linux
    ./test_linux

Commande Windows :

    x86_64-w64-mingw32-g++ -std=c++17 -I ~/Projets/Nkentseu/Kernel/Foundation/NKPlatform/src test_plateforme.cpp -o test_windows.exe
    wine test_windows.exe

Le meme fichier source. Deux chaines de compilation. Deux executions.

Sorties brutes

Linux :

    === Compilateur ===
    clang 14.0.0

    === Plateforme detectee ===
    NKENTSEU_PLATFORM_WINDOWS = non defini
    NKENTSEU_PLATFORM_LINUX = 1
    NKENTSEU_PLATFORM_MACOS = non defini
    NKENTSEU_PLATFORM_NAME = Linux

    === Categorie ===
    NKENTSEU_PLATFORM_DESKTOP = 1
    NKENTSEU_PLATFORM_MOBILE = non defini

    === Architecture (macros compilateur) ===
    __x86_64__ = 1
    __aarch64__ = non defini

    === Tailles des types ===
    sizeof(char)   = 1
    sizeof(short)  = 2
    sizeof(int)    = 4
    sizeof(long)   = 8
    sizeof(long long) = 8
    sizeof(void*)  = 8
    sizeof(size_t) = 8

Windows :

    === Compilateur ===
    gcc 10.0.0

    === Plateforme detectee ===
    NKENTSEU_PLATFORM_WINDOWS = 1
    NKENTSEU_PLATFORM_LINUX = non defini
    NKENTSEU_PLATFORM_MACOS = non defini
    NKENTSEU_PLATFORM_NAME = Windows

    === Categorie ===
    NKENTSEU_PLATFORM_DESKTOP = 1
    NKENTSEU_PLATFORM_MOBILE = non defini

    === Architecture (macros compilateur) ===
    __x86_64__ = 1
    __aarch64__ = non defini

    === Tailles des types ===
    sizeof(char)   = 1
    sizeof(short)  = 2
    sizeof(int)    = 4
    sizeof(long)   = 4
    sizeof(long long) = 8
    sizeof(void*)  = 8
    sizeof(size_t) = 8

Ce qui change sans que j'aie rien ecrit pour cela

1. Le compilateur. La meme ligne de source est compilee par clang
   14.0.0 d'un cote et par gcc 10.0.0 de l'autre. Le programme affiche
   le nom et la version sans les connaitre. Ce sont les macros
   __clang__ et __GNUC__ qui les fournissent.

2. La macro de plateforme active. NKENTSEU_PLATFORM_LINUX vaut 1
   d'un cote, non defini de l'autre. NKENTSEU_PLATFORM_WINDOWS fait
   l'inverse. La meme ligne de source a ete compilee dans deux
   configurations qui ne sont pas identiques.

3. La chaine NKENTSEU_PLATFORM_NAME. Elle vaut "Linux" sur un
   systeme, "Windows" sur l'autre. Le programme affiche les deux sans
   contenir cette difference. C'est le header qui la pose selon la
   plateforme detectee.

4. sizeof(long). Il vaut 8 sur Linux, 4 sur Windows. C'est le point
   le plus visible : le meme programme repond deux choses differentes
   a la meme question. La cause n'est pas dans le programme. C'est un
   choix des deux modeles de donnees :
   - Linux utilise le modele LP64 : long et pointeur font 8 octets,
     int fait 4.
   - Windows utilise le modele LLP64 : long fait 4 octets comme int,
     pointeur fait 8.
   Les deux compilent la meme ligne sizeof(long), et elle ne fait pas
   la meme chose.

Ce qui ne change pas malgre la difference

1. sizeof(void*) = 8 des deux cotes. Les deux compilations visent
   64 bits. Un programme qui passe un pointeur par une union avec un
   long marche sur Linux et casse sur Windows.

2. sizeof(size_t) = 8 des deux cotes. Cette fois les deux modeles
   s'accordent.

3. __x86_64__ = 1 des deux cotes. L'architecture est la meme.
   Le systeme d'exploitation change, mais le materiel sous-jacent est
   identique.

4. NKENTSEU_PLATFORM_DESKTOP = 1 des deux cotes. Le header range
   Linux et Windows dans la meme categorie, desktop. Le code qui
   teste cette macro compilera les deux branches de la meme facon.

Tableau recapitulatif

| Mesure | Linux | Windows |
|--------|-------|---------|
| Compilateur | clang 14.0.0 | gcc 10.0.0 |
| NKENTSEU_PLATFORM_LINUX | 1 | non defini |
| NKENTSEU_PLATFORM_WINDOWS | non defini | 1 |
| NKENTSEU_PLATFORM_NAME | "Linux" | "Windows" |
| NKENTSEU_PLATFORM_DESKTOP | 1 | 1 |
| __x86_64__ | 1 | 1 |
| sizeof(char) | 1 | 1 |
| sizeof(short) | 2 | 2 |
| sizeof(int) | 4 | 4 |
| sizeof(long) | 8 | 4 |
| sizeof(long long) | 8 | 8 |
| sizeof(void*) | 8 | 8 |
| sizeof(size_t) | 8 | 8 |

Ce que cela dit du portage

Le programme source n'a pas change. Sa compilation n'a pas change non
plus : c'est la meme ligne de commande, avec un compilateur different
et un jeu de macros different. Ce sont les macros predefinies du
compilateur qui font tout le travail.

Le header NkPlatformDetect.h est l'endroit ou ce travail est
centralise. Il lit __linux__ et _WIN32 (entre autres), pose
NKENTSEU_PLATFORM_LINUX ou NKENTSEU_PLATFORM_WINDOWS, et pose
NKENTSEU_PLATFORM_NAME avec la bonne chaine. Grace a lui, un
programme qui teste ces macros n'a pas a connaitre les macros
predefinies de chaque compilateur.

Le cas de sizeof(long) n'est pas resolu par le header. C'est une
propriete du modele de donnees, pas de la plateforme. Le header ne
peut rien y changer. Un programme qui a besoin d'un entier de taille
determinee doit utiliser int64_t, pas long.

Mesure faite le 28/09/2026.
Version de Jenga : 2.8.0.
