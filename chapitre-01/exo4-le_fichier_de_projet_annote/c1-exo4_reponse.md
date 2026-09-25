Exercice 4 - Le fichier de projet annote

J'ai affiche le fichier avec la commande :

```
cat -n Kernel/Foundation/NKPlatform/NKPlatform.jenga
```

Sortie brute :

```
     1	#!/usr/bin/env python3
     2	# -*- coding: utf-8 -*-
     3	"""
     4	NKPlatform — Détection OS/arch/compilateur/CPU (C++20)
     5	=========================================================
     6	Fondation sans dépendances. Fournit des macros et informations
     7	compilées sur l'environnement d'exécution.
     8	"""
     9	
    10	from Jenga import *
    11	from jengaconfig import *
    12	
    13	
    14	with project("NKPlatform"):
    15	    language("C++")
    16	    cppdialect("C++20")
    17	    location(".")
    18	
    19	    nkentseudependson(
    20	        [],
    21	        selfexport="NKPlatform",
    22	        extra_includes=["src", "pch"],
    23	    )
    24	
    25	    pchheader("pch/pch.h")
    26	    pchsource("pch/pch.cpp")
    27	
    28	    files([
    29	        "src/NKPlatform/**.cpp",
    30	        "src/NKPlatform/**.h",
    31	    ])
    32	
    33	    objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
    34	    targetdir("%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}")
    35	
    36	    with filter("system:Windows && options:windows-runtime=uwp"):
    37	        objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}-uwp/%{prj.name}")
    38	        targetdir("%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}-uwp")
    39	
    40	    with filter("system:Windows && !options:windows-runtime=uwp && !system:XboxSeries && !system:XboxOne"):
    41	        usetoolchain(TC_WINDOWS)
    42	    with filter("system:UWP || system:Windows && options:windows-runtime=uwp"):
    43	        usetoolchain("xbox-clang")
    44	    with filter("system:macOS"):
    45	        usetoolchain("clang-native")
    46	    with filter("system:Android"):
    47	        # Workaround: disable PCH on Android (NDK r27 + clang 18 + libc++)
    48	        pchheader("")
    49	        pchsource("")
    50	        usetoolchain("android-ndk")
    51	    with filter("system:HarmonyOS"):
    52	        # PCH desactive (NDK OHOS clang, meme contrainte qu'Android)
    53	        pchheader("")
    54	        pchsource("")
    55	        usetoolchain("ohos-ndk")
    56	    with filter("system:Web"):
    57	        usetoolchain("emscripten")
    58	    with filter("system:XboxSeries || system:XboxOne"):
    59	        usetoolchain("xbox-clang")
    60	
    61	    with filter("config:Debug"):
    62	        defines(["_DEBUG", "DEBUG", "NKENTSEU_DEBUG"])
    63	        optimize("Off")
    64	        symbols(True)
    65	    with filter("config:Release"):
    66	        defines(["NDEBUG", "NKENTSEU_RELEASE"])
    67	        optimize("Speed")
    68	        symbols(False)
    69	
    70	    # Tests unitaires/benchmarks (desktop uniquement)
    71	    with filter("(system:Linux || system:macOS || (system:Windows && !options:windows-runtime=uwp && !system:XboxSeries && !system:XboxOne)) && !system:Android && !system:iOS || system:Web"):
    72	        with test():
    73	            testfiles(["tests/**.cpp"])
```

Annotations

Chaque annotation porte une marque. [teste] veut dire que j'ai
retire la ligne, reconstruit, note le message, puis remis la ligne.
[deduit] veut dire que je n'ai pas teste cette annotation, c'est
une deduction raisonnable.

```
Ligne 1  | #!/usr/bin/env python3
              [deduit] Shebang. Execute ce fichier avec Python 3.

Ligne 2  | # -*- coding: utf-8 -*-
              [deduit] Encodage UTF-8 declare.

Lignes 3 a 8 | Docstring du module
              [deduit] Decrit NKPlatform : detection OS, architecture,
                       compilateur, CPU. Module de fondation sans
                       dependances.

Ligne 10 | from Jenga import *
Ligne 11 | from jengaconfig import *
              [deduit] Importe toutes les fonctions de Jenga et de
                       jengaconfig.

Ligne 14 | with project(NKPlatform):
              [deduit] Declare le projet NKPlatform.

Ligne 15 |     language(C++)
              [deduit] Langage du projet.

Ligne 16 |     cppdialect(C++20)
              [deduit] Norme C++20.

Ligne 17 |     location(.)
              [deduit] Le projet vit dans le dossier du fichier .jenga.

Lignes 19 a 23 | nkentseudependson([], selfexport=, extra_includes=)
              [teste] En retirant cette instruction et ses arguments,
                      on obtient :

                      Error loading workspace: unmatched ')'
                      (NKPlatform.jenga, line 23)
                      Failed to load workspace.

                      Le workspace ne charge meme pas. La parenthese
                      fermante reste seule et Python refuse de lire
                      le fichier.

Ligne 25 |     pchheader(pch/pch.h)
Ligne 26 |     pchsource(pch/pch.cpp)
              [teste] En retirant ces deux lignes, le build reussit
                      en 0.37s sans message d'erreur. Seul effet :
                      les en-tetes sont inclus a chaque fichier au
                      lieu d'etre precompiles.

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

Lignes 36 a 54 | Filtres par systeme (Windows, UWP, macOS, Android,
                HarmonyOS, Web, Xbox)
              [deduit] Choisit la chaine de compilation et les options
                       selon la plateforme cible.

Lignes 56 a 68 | Filtres par configuration (Debug, Release)
              [deduit] En Debug, optimisations desactivees et
                       symboles gardes. En Release, optimisations
                       activees et symboles retires.

Lignes 71 a 73 | with test(): testfiles([...])
              [deduit] Declare une suite de tests unitaires.
```

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
