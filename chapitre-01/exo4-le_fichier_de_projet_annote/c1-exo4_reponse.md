Exercice 4 - Le fichier de projet annote

J'ai choisi le module NKPlatform. C'est le module le plus bas du moteur,
il ne depend de rien, et il est le premier construit dans l'ordre que
Jenga affiche. Son fichier se trouve a :

Kernel/Foundation/NKPlatform/NKPlatform.jenga

Voici le contenu du fichier, annote ligne par ligne.

--- En-tete du fichier ---

#!/usr/bin/env python3
C'est la ligne qui dit au systeme que ce fichier est un script Python.
C'est normal, parce qu'un fichier .jenga est un programme Python.

# -*- coding: utf-8 -*-
Indique l'encodage du fichier. Je pense que c'est pour gerer les accents
dans les commentaires.

""" ... """
Le bloc entre triples guillemets est une description du module. Il dit :
NKPlatform detecte OS, architecture, compilateur et CPU. C'est un module
de fondation, sans dependances. Langage C++20.

--- Les imports ---

from Jenga import *
Importe les fonctions de Jenga (project, files, filter, etc.).

from jengaconfig import *
Importe les fonctions propres au depot Nkentseu (nkentseudependson,
TC_WINDOWS, etc.).

--- Le projet ---

with project("NKPlatform"):
Declare un projet nomme NKPlatform. Tout ce qui est indente en dessous
appartient a ce projet.

    language("C++")
Le projet est ecrit en C++.

    cppdialect("C++20")
Le projet utilise la norme C++20.
? Je ne sais pas exactement quelle version precise de C++20 est visee.

    location(".")
Le projet vit dans le dossier ou se trouve ce fichier .jenga.
Le point veut dire "ici".

--- Les dependances ---

    nkentseudependson(
        [],
        selfexport="NKPlatform",
        extra_includes=["src", "pch"],
    )
C'est une fonction propre a Nkentseu, pas de Jenga. La liste vide [] dit
que NKPlatform ne depend d'aucun autre module. C'est coherent : c'est le
socle. selfexport est le nom de la macro d'export du module. extra_includes
ajoute deux dossiers ou le compilateur cherchera les en-tetes.

--- L'en-tete precompile ---

    pchheader("pch/pch.h")
    pchsource("pch/pch.cpp")
PCH veut dire precompiled header. C'est un systeme qui accelere la
compilation en preparant certains en-tetes a l'avance. Le .h est l'en-tete
et le .cpp est le fichier qui sert a le construire.

--- Les sources ---

    files([
        "src/NKPlatform/**.cpp",
        "src/NKPlatform/**.h",
    ])
C'est la liste des fichiers source du projet. Les motifs veulent dire :
tous les .cpp et tous les .h dans src/NKPlatform et ses sous-dossiers.
Le double asterisque ** veut dire "et tous les sous-dossiers".

--- Ou vont les fichiers ---

    objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
    targetdir("%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}")
objdir est l'endroit ou vont les fichiers intermediaires pendant la
compilation. targetdir est l'endroit ou va le fichier final produit
(ici une bibliotheque statique). Les %{...} sont des variables qui sont
remplacees au moment de la construction.

--- Le filtre UWP ---

    with filter("system:Windows && options:windows-runtime=uwp"):
        objdir(...)
        targetdir(...)
Quand on compile pour Windows en mode UWP, on change les dossiers de
sortie pour ne pas ecraser ceux de Windows normal.

--- Les chaines de compilation ---

    with filter("system:Windows && !options:windows-runtime=uwp && !system:XboxSeries && !system:XboxOne"):
        usetoolchain(TC_WINDOWS)
Pour Windows classique (pas UWP, pas Xbox), utilise la chaine de
compilation TC_WINDOWS. TC_WINDOWS est une constante definie dans
jengaconfig.

    with filter("system:UWP || system:Windows && options:windows-runtime=uwp"):
        usetoolchain("xbox-clang")
Pour UWP, utilise la chaine xbox-clang.
? Je ne suis pas sur de pourquoi la meme chaine sert pour UWP et Xbox.

    with filter("system:macOS"):
        usetoolchain("clang-native")
Pour macOS, utilise la chaine clang-native.

    with filter("system:Android"):
        # Workaround: disable PCH on Android (NDK r27 + clang 18 + libc++)
        pchheader("")
        pchsource("")
        usetoolchain("android-ndk")
Pour Android, on desactive le PCH. Le commentaire explique : c'est un
contournement a cause du NDK r27 et de clang 18.
? Je ne sais pas ce qui pose probleme exactement entre le NDK et le PCH.

    with filter("system:HarmonyOS"):
        # PCH desactive (NDK OHOS clang, meme contrainte qu'Android)
        pchheader("")
        pchsource("")
        usetoolchain("ohos-ndk")
Meme chose pour HarmonyOS, avec le NDK d'Huawei.

    with filter("system:Web"):
        usetoolchain("emscripten")
Pour le Web, utilise la chaine emscripten.

    with filter("system:XboxSeries || system:XboxOne"):
        usetoolchain("xbox-clang")
Pour Xbox Series et Xbox One, utilise la chaine xbox-clang.

--- Les configurations ---

    with filter("config:Debug"):
        defines(["_DEBUG", "DEBUG", "NKENTSEU_DEBUG"])
        optimize("Off")
        symbols(True)
En Debug : on definit trois macros (dont _DEBUG, qui est le standard),
on desactive les optimisations, et on garde les symboles de debogage.

    with filter("config:Release"):
        defines(["NDEBUG", "NKENTSEU_RELEASE"])
        optimize("Speed")
        symbols(False)
En Release : on definit NDEBUG et NKENTSEU_RELEASE, on optimise pour la
vitesse, et on retire les symboles.

--- Les tests ---

    # Tests unitaires/benchmarks (desktop uniquement)
    with filter("(system:Linux || system:macOS || (system:Windows && !options:windows-runtime=uwp && !system:XboxSeries && !system:XboxOne)) && !system:Android && !system:iOS || system:Web"):
        with test():
            testfiles(["tests/**.cpp"])
Cette grande condition dit : les tests ne se compilent que sur Linux,
macOS, Windows classique et Web. Pas sur Android, iOS, UWP ou Xbox.
En dessous, with test() declare une suite de tests attachee au projet,
et testfiles liste les fichiers de test.

--- Ce que je ne comprends pas encore ---

? Pourquoi la meme chaine de compilation xbox-clang est utilisee pour
  UWP et pour Xbox.
? Ce que fait exactement le NDK pour rendre le PCH impossible sur
  Android et HarmonyOS.
? Ce que contient le fichier pch.h.
? Comment sont generes les noms de macros NKENTSEU_DEBUG et
  NKENTSEU_RELEASE, qui n'ont pas l'air d'etre standards.

Fichier annote le 24/09/2026.
Version de Jenga : 2.8.0.
Dernier commit du depot : 9c3fad3, date du 2026-09-13.
