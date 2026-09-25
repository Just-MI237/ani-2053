Exercice 8 - Debug contre Release

J'ai construit le meme projet MonEssai dans les deux configurations,
et j'ai compare la taille du binaire et le temps de construction.

Configuration Debug

Commande :

    rm -rf Build/
    time jenga build --project MonEssai --config Debug
    ls -l Build/Bin/Debug-Linux/MonEssai/MonEssai

Lignes importantes de la sortie brute :

    Configuration: Debug
    Target:        Linux x86_64
    Toolchain:     host-clang

    Build Order (2 projects):
      1. NKPlatform [STATIC_LIB]
      2. MonEssai [CONSOLE_APP] (depends: NKPlatform)

    BUILD COMPLETED
    Projects Built:  2/2
    Time:           0.84s
    Status:         SUCCESS

Mesure du temps par la commande time :

    real    0m7.080s
    user    0m1.733s
    sys     0m1.147s

Taille du binaire produit :

    -rwxr-xr-x 1 justmaxit justmaxit 28168 Sep 25 04:51 Build/Bin/Debug-Linux/MonEssai/MonEssai

Configuration Release

Commande :

    rm -rf Build/
    time jenga build --project MonEssai --config Release
    ls -l Build/Bin/Release-Linux/MonEssai/MonEssai

Lignes importantes de la sortie brute :

    Configuration: Release
    Target:        Linux x86_64
    Toolchain:     host-clang

    Build Order (2 projects):
      1. NKPlatform [STATIC_LIB]
      2. MonEssai [CONSOLE_APP] (depends: NKPlatform)

    BUILD COMPLETED
    Projects Built:  2/2
    Time:           0.79s
    Status:         SUCCESS

Mesure du temps par la commande time :

    real    0m7.142s
    user    0m1.604s
    sys     0m1.074s

Taille du binaire produit :

    -rwxr-xr-x 1 justmaxit justmaxit 19768 Sep 25 04:52 Build/Bin/Release-Linux/MonEssai/MonEssai

Les quatre nombres

- Taille Debug : 28168 octets
- Taille Release : 19768 octets
- Temps Debug (shell) : 7.080s
- Temps Release (shell) : 7.142s

Difference de taille : le binaire Release est plus petit d'environ 30 pour
cent. Difference de temps : les deux temps sont presque identiques.

Les lignes du .jenga qui expliquent ces nombres

J'ai cherche les lignes qui parlent d'optimisation et de symboles dans
les fichiers .jenga du projet et de ses dependances :

    grep -rn "optimize\|symbols\|defines" --include="*.jenga" \
        Applications/MonEssai/ Kernel/Foundation/NKPlatform/ Nkentseu.jenga

Resultat :

    Kernel/Foundation/NKPlatform/NKPlatform.jenga:62:        defines(["_DEBUG", "DEBUG", "NKENTSEU_DEBUG"])
    Kernel/Foundation/NKPlatform/NKPlatform.jenga:63:        optimize("Off")
    Kernel/Foundation/NKPlatform/NKPlatform.jenga:64:        symbols(True)
    Kernel/Foundation/NKPlatform/NKPlatform.jenga:66:        defines(["NDEBUG", "NKENTSEU_RELEASE"])
    Kernel/Foundation/NKPlatform/NKPlatform.jenga:67:        optimize("Speed")
    Kernel/Foundation/NKPlatform/NKPlatform.jenga:68:        symbols(False)

Ces six lignes sont dans deux blocs with filter :

    with filter("config:Debug"):
        defines(["_DEBUG", "DEBUG", "NKENTSEU_DEBUG"])
        optimize("Off")
        symbols(True)

    with filter("config:Release"):
        defines(["NDEBUG", "NKENTSEU_RELEASE"])
        optimize("Speed")
        symbols(False)

Explication

En Debug, Jenga compile avec optimize("Off") et symbols(True). Les
optimisations sont desactivees et les symboles de debogage sont
conserves. C'est ce qui rend le binaire plus gros.

En Release, Jenga compile avec optimize("Speed") et symbols(False). Le
compilateur optimise le code et retire les symboles. C'est ce qui rend
le binaire plus petit.

Ces quatre appels expliquent les deux tailles. Pour les deux temps, en
revanche, l'explication n'est pas dans ces lignes : le projet MonEssai
est minuscule (un seul fichier main.cpp), donc la compilation elle-meme
est si rapide que la difference entre Debug et Release n'est pas
visible. Ce qui prend le plus de temps, c'est le demarrage de Jenga et
la lecture du workspace, pas la compilation. Sur un projet plus gros,
les optimisations de Release ralentiraient la compilation, parce
qu'optimiser prend plus de travail que ne rien faire.

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
Dernier commit du depot Nkentseu : 9c3fad3, date du 2026-09-13.
