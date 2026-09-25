Exercice 4 - Le fichier de projet annoté

J'ai choisi le module NKPlatform. C'est le module le plus bas du moteur,
il ne dépend d'aucun autre module, et il est le premier construit dans
l'ordre que Jenga affiche.

Le fichier se trouve à :

    Kernel/Foundation/NKPlatform/NKPlatform.jenga

Il est déposé tel quel à côté de ce rapport, sous le nom NKPlatform.jenga.

Ci-dessous, je reprends le fichier ligne par ligne et j'ajoute mon
commentaire sous chaque ligne qui en mérite un. Les commentaires du
fichier d'origine sont en `#`. Mes propres annotations commencent par
un chevron ↑ pour qu'on voie tout de suite ce qui est de moi et ce qui
est du fichier.

---

#!/usr/bin/env python3
  ↑ Shebang. Dit au système d'exécuter ce fichier avec Python 3. C'est
    normal, puisqu'un fichier .jenga est un programme Python.

# -*- coding: utf-8 -*-
  ↑ Déclare l'encodage UTF-8 du fichier, pour que les accents dans les
    commentaires soient bien lus.

"""
NKPlatform — Détection OS/arch/compilateur/CPU (C++20)
=========================================================
Fondation sans dépendances. Fournit des macros et informations
compilées sur l'environnement d'exécution.
"""
  ↑ Docstring du module. Décrit ce que fait NKPlatform : il détecte
    le système d'exploitation, l'architecture processeur, le compilateur
    et les caractéristiques du CPU. C'est un module de fondation, sans
    dépendances, en C++20.

from Jenga import *
  ↑ Importe toutes les fonctions de Jenga (project, files, filter, etc.).
    L'astérisque veut dire "tout ce que le module expose".

from jengaconfig import *
  ↑ Importe toutes les fonctions propres au dépôt Nkentseu
    (nkentseudependson, TC_WINDOWS, etc.).


with project("NKPlatform"):
  ↑ Déclare un projet nommé NKPlatform. Tout ce qui est indenté en
    dessous appartient à ce projet. Le with est un gestionnaire de
    contexte Python : à la fermeture du bloc, la déclaration est finie.

    language("C++")
      ↑ Le projet est écrit en C++.

    cppdialect("C++20")
      ↑ Le projet utilise la norme C++20.
      ? Je ne sais pas exactement quelle version précise de C++20 est visée.

    location(".")
      ↑ Le projet vit dans le dossier où se trouve ce fichier .jenga.
        Le point veut dire "ici".

    nkentseudependson(
        [],
        selfexport="NKPlatform",
        extra_includes=["src", "pch"],
    )
      ↑ Fonction propre à Nkentseu, pas de Jenga. La liste vide au début
        dit que NKPlatform ne dépend d'aucun autre module. Cohérent :
        c'est le socle, construit en premier.
        selfexport donne le nom de la macro d'export du module.
        extra_includes ajoute deux dossiers, src et pch, où le compilateur
        cherchera les en-têtes.

    pchheader("pch/pch.h")
      ↑ Déclare un en-tête précompilé. PCH veut dire precompiled header.
        C'est un mécanisme qui prépare certains en-têtes à l'avance pour
        accélérer la compilation.

    pchsource("pch/pch.cpp")
      ↑ Le fichier qui sert à construire l'en-tête précompilé.

    files([
        "src/NKPlatform/**.cpp",
        "src/NKPlatform/**.h",
    ])
      ↑ Liste des fichiers source. Les motifs veulent dire : tous les
        .cpp et tous les .h dans src/NKPlatform et ses sous-dossiers.
        Le double astérisque ** descend dans les sous-dossiers.

    objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
      ↑ Où vont les fichiers intermédiaires pendant la compilation.
        Les variables entre accolades sont remplacées au moment de la
        construction : wks.location est le dossier du workspace,
        cfg.buildcfg vaut Debug ou Release, cfg.system vaut Windows ou
        Linux, prj.name est le nom du projet courant.

    targetdir("%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}")
      ↑ Où va le fichier final produit, ici une bibliothèque statique.

    with filter("system:Windows && options:windows-runtime=uwp"):
        objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}-uwp/%{prj.name}")
        targetdir("%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}-uwp")
      ↑ Pour Windows en mode UWP, on change les dossiers de sortie pour
        ne pas écraser ceux de Windows classique.

    with filter("system:Windows && !options:windows-runtime=uwp && !system:XboxSeries && !system:XboxOne"):
        usetoolchain(TC_WINDOWS)
      ↑ Pour Windows classique (pas UWP, pas Xbox), utilise la chaîne de
        compilation TC_WINDOWS, définie dans jengaconfig.

    with filter("system:UWP || system:Windows && options:windows-runtime=uwp"):
        usetoolchain("xbox-clang")
      ↑ Pour UWP, utilise la chaîne xbox-clang.
      ? Je ne suis pas sûr de pourquoi la même chaîne sert pour UWP et Xbox.

    with filter("system:macOS"):
        usetoolchain("clang-native")
      ↑ Pour macOS, utilise clang-native.

    with filter("system:Android"):
        # Workaround: disable PCH on Android (NDK r27 + clang 18 + libc++)
        pchheader("")
        pchsource("")
        usetoolchain("android-ndk")
      ↑ Pour Android, on désactive le PCH en mettant des chaînes vides.
        Le commentaire d'origine dit que c'est un contournement à cause
        du NDK r27 et de clang 18.
      ? Je ne sais pas exactement ce qui pose problème entre le NDK et le PCH.

    with filter("system:HarmonyOS"):
        # PCH desactive (NDK OHOS clang, meme contrainte qu'Android)
        pchheader("")
        pchsource("")
        usetoolchain("ohos-ndk")
      ↑ Même chose pour HarmonyOS, avec le NDK de Huawei.

    with filter("system:Web"):
        usetoolchain("emscripten")
      ↑ Pour le Web, utilise emscripten.

    with filter("system:XboxSeries || system:XboxOne"):
        usetoolchain("xbox-clang")
      ↑ Pour Xbox Series et Xbox One, utilise xbox-clang.

    with filter("config:Debug"):
        defines(["_DEBUG", "DEBUG", "NKENTSEU_DEBUG"])
        optimize("Off")
        symbols(True)
      ↑ En Debug : on définit trois macros dont _DEBUG qui est standard,
        on désactive les optimisations, et on garde les symboles de
        débogage.

    with filter("config:Release"):
        defines(["NDEBUG", "NKENTSEU_RELEASE"])
        optimize("Speed")
        symbols(False)
      ↑ En Release : on définit NDEBUG et NKENTSEU_RELEASE, on optimise
        pour la vitesse, et on retire les symboles.
      ? Je ne sais pas comment sont générés les noms NKENTSEU_DEBUG et
        NKENTSEU_RELEASE, qui ne sont pas standards.

    # Tests unitaires/benchmarks (desktop uniquement)
      ↑ Commentaire d'origine. Les tests ne se compilent que sur les
        plateformes de bureau.

    with filter("(system:Linux || system:macOS || (system:Windows && !options:windows-runtime=uwp && !system:XboxSeries && !system:XboxOne)) && !system:Android && !system:iOS || system:Web"):
        with test():
            testfiles(["tests/**.cpp"])
      ↑ Cette longue condition dit que les tests ne se compilent que
        sur Linux, macOS, Windows classique et Web. Pas sur Android,
        iOS, UWP ou Xbox. En dessous, with test() déclare une suite de
        tests attachée au projet, et testfiles liste les fichiers de test.

---

Ce que je ne comprends pas encore

- Pourquoi la même chaîne xbox-clang est utilisée pour UWP et pour Xbox.
- Ce que fait exactement le NDK pour rendre le PCH impossible sur Android
  et HarmonyOS.
- Ce que contient le fichier pch.h.
- Comment sont générés les noms de macros NKENTSEU_DEBUG et
  NKENTSEU_RELEASE.

Rapport rédigé le 25/09/2026.
Version de Jenga : 2.8.0.
Dernier commit du dépôt Nkentseu : 9c3fad3, daté du 2026-09-13.
