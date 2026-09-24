Exercice 4 - Le fichier de projet annoté

J'ai choisi le module NKPlatform. C'est le module le plus bas du moteur, il
ne dépend d'aucun autre module, et il est le premier construit dans l'ordre
que Jenga affiche. Son fichier se trouve à :

Kernel/Foundation/NKPlatform/NKPlatform.jenga

Je l'annote ci-dessous en suivant les cinq points demandés par l'énoncé :
type, sources, dépendances, filtres, tests. Je marque d'un point
d'interrogation ce que je ne comprends pas encore.

En-tête du fichier

Le fichier s'ouvre sur deux lignes techniques. La première est un shebang
qui dit au système que ce fichier est un script Python. C'est normal,
puisqu'un fichier .jenga est un programme Python. La deuxième précise que
l'encodage du fichier est UTF-8, pour que les accents dans les commentaires
soient bien lus.

Vient ensuite un bloc entre triples guillemets. C'est une description du
module : NKPlatform détecte le système d'exploitation, l'architecture, le
compilateur et le processeur. C'est un module de fondation, sans
dépendances, écrit en C++20.

Les imports

Les deux lignes suivantes chargent les outils nécessaires :

from Jenga import *
from jengaconfig import *

Le premier import charge les fonctions de Jenga (project, files, filter,
etc.). Le second charge les fonctions propres au dépôt Nkentseu, comme
nkentseudependson et la constante TC_WINDOWS.

Type du projet

La ligne principale est :

with project("NKPlatform"):

Elle déclare un projet nommé NKPlatform. Tout ce qui est indenté en dessous
appartient à ce projet.

Deux lignes précisent le contenu et la norme :

language("C++")
cppdialect("C++20")

Le projet est donc écrit en C++ et utilise la norme C++20. Je ne sais pas
exactement quelle version précise de C++20 est visée ?

La ligne suivante dit où vit le projet :

location(".")

Le point veut dire : dans le dossier où se trouve ce fichier .jenga.

Sources

La liste des fichiers source est donnée par :

files([
    "src/NKPlatform/**.cpp",
    "src/NKPlatform/**.h",
])

Elle veut dire : tous les fichiers .cpp et tous les fichiers .h dans
src/NKPlatform et tous ses sous-dossiers. Le double astérisque ** descend
dans les sous-dossiers, contrairement à un seul astérisque qui s'arrête
au premier niveau.

Le projet déclare aussi un en-tête précompilé :

pchheader("pch/pch.h")
pchsource("pch/pch.cpp")

PCH veut dire precompiled header. C'est un mécanisme qui prépare certains
en-têtes à l'avance pour accélérer la compilation. Le .h est l'en-tête
précompilé, et le .cpp est le fichier qui sert à le construire. Je ne sais
pas ce que contient exactement le fichier pch.h ?

Enfin, deux lignes disent où vont les fichiers produits :

objdir(...)
targetdir(...)

objdir est le dossier des fichiers intermédiaires pendant la compilation.
targetdir est le dossier du fichier final produit, ici une bibliothèque
statique. Les motifs entre pourcents et accolades sont des variables
remplacées au moment de la construction. Par exemple la variable
cfg.buildcfg vaut Debug ou Release selon la configuration.

Dépendances

NKPlatform est appelé avec :

nkentseudependson(
    [],
    selfexport="NKPlatform",
    extra_includes=["src", "pch"],
)

C'est une fonction propre à Nkentseu, ce n'est pas une fonction de Jenga.
La liste vide au début dit que NKPlatform ne dépend d'aucun autre module.
C'est cohérent avec sa position dans l'arbre : c'est le socle, construit
en premier.

selfexport donne le nom de la macro d'export du module, ici NKPlatform.
extra_includes ajoute deux dossiers, src et pch, où le compilateur
cherchera les en-têtes quand d'autres modules incluront ceux de
NKPlatform.

Filtres

Le fichier contient beaucoup de filtres, qui appliquent des options selon
le système, la configuration, ou un ensemble d'options.

Filtre pour UWP

Quand on compile pour Windows en mode UWP, on change les dossiers de sortie
pour ne pas écraser ceux de Windows classique. C'est ce que fait le
premier filtre, avec system:Windows && options:windows-runtime=uwp.

Filtres de chaîne de compilation

Pour Windows classique (pas UWP, pas Xbox), on utilise la chaîne
TC_WINDOWS, définie dans jengaconfig. Le filtre exclut explicitement UWP,
XboxSeries et XboxOne.

Pour UWP, on utilise la chaîne xbox-clang. Je ne suis pas sûr de pourquoi
la même chaîne sert pour UWP et pour Xbox ?

Pour macOS, on utilise clang-native.

Pour Android, on désactive le PCH en mettant des chaînes vides, puis on
utilise android-ndk. Un commentaire dans le fichier explique que c'est un
contournement à cause du NDK r27 et de clang 18. Je ne sais pas exactement
ce qui pose problème entre le NDK et le PCH ?

Pour HarmonyOS, même chose, avec le NDK de Huawei.

Pour le Web, on utilise emscripten.

Pour Xbox Series et Xbox One, on utilise xbox-clang.

Filtres de configuration

En Debug, on définit trois macros dont _DEBUG qui est standard, on
désactive les optimisations, et on garde les symboles de débogage.

En Release, on définit NDEBUG et NKENTSEU_RELEASE, on optimise pour la
vitesse, et on retire les symboles.

Je ne sais pas comment sont générés les noms NKENTSEU_DEBUG et
NKENTSEU_RELEASE, qui ne sont pas standards ?

Tests

Le fichier déclare une suite de tests, mais seulement sur certaines
plateformes. La longue condition du filtre dit que les tests ne se
compilent que sur Linux, macOS, Windows classique et Web. Ils sont exclus
sur Android, iOS, UWP et Xbox.

En dessous, le bloc avec test() déclare une suite de tests attachée au
projet, et testfiles liste les fichiers de test.

Ce que je ne comprends pas encore

- Pourquoi la même chaîne xbox-clang est utilisée pour UWP et pour Xbox.
- Ce que fait exactement le NDK pour rendre le PCH impossible sur Android
  et HarmonyOS.
- Ce que contient le fichier pch.h.
- Comment sont générés les noms de macros NKENTSEU_DEBUG et
  NKENTSEU_RELEASE.

Fichier annoté le 24/09/2026.
Version de Jenga : 2.8.0.
Dernier commit du dépôt : 9c3fad3, daté du 2026-09-13.
