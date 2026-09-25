Exercice 6 - Les deux erreurs de dependance

Pour cet exercice, j'ai modifie le projet MonEssai pour qu'il appelle
reellement une fonction de NKPlatform. J'ai choisi GetPlatformConfig,
declaree dans NkPlatformConfig.h et definie dans NkPlatformConfig.cpp.
Comme le code est dans un fichier .cpp, un vrai lien avec la
bibliotheque NKPlatform est necessaire.

Le fichier main.cpp contient :

#include "NKPlatform/NkPlatformConfig.h"

int main()
{
    const auto& config = nkentseu::platform::GetPlatformConfig();
    (void)config;
    return 0;
}

J'ai teste trois configurations l'une apres l'autre. Entre chaque test,
j'ai supprime le dossier Build pour qu'un ancien fichier .a ne fausse
pas le resultat.

Cas 0 : dependson et links presents

MonEssai.jenga contenait :

    includedirs(["%{wks.location}/Kernel/Foundation/NKPlatform/src"])
    dependson(["NKPlatform"])
    links(["NKPlatform"])

La commande jenga build --project MonEssai --config Debug a produit :

Build Order (2 projects):
  1. NKPlatform [STATIC_LIB]
  2. MonEssai [CONSOLE_APP] (depends: NKPlatform)

NKPlatform a ete compile et lie en premier, puis MonEssai a ete compile
et lie. Resultat :

BUILD COMPLETED
Projects Built:  2/2
Status: SUCCESS

Cas 1 : links seul, sans dependson

J'ai retire dependson et garde links. MonEssai.jenga contenait :

    includedirs(["%{wks.location}/Kernel/Foundation/NKPlatform/src"])
    links(["NKPlatform"])

La commande a produit :

Build Order (1 projects):
  1. MonEssai [CONSOLE_APP]

Project: MonEssai  Kind: CONSOLE_APP
Found 1 source file(s)
Compiled: main.cpp
Linking...
Compilation Error: Link Failed
/usr/bin/ld: cannot find -lNKPlatform: No such file or directory
clang: error: linker command failed with exit code 1

BUILD FAILED
Projects Built:  0/1
Status: FAILURE

Le message est :

  /usr/bin/ld: cannot find -lNKPlatform: No such file or directory

Jenga n'a construit que MonEssai. NKPlatform n'a pas ete construit,
donc le fichier Build/Lib/Debug-Linux/NKPlatform.a n'existe pas. La
compilation de main.cpp a elle-meme reussi ; c'est le lien qui echoue,
parce que le compilateur cherche une bibliotheque nommee NKPlatform et
ne la trouve pas.

Cas 2 : dependson seul, sans links

J'ai remis dependson et retire links. MonEssai.jenga contenait :

    includedirs(["%{wks.location}/Kernel/Foundation/NKPlatform/src"])
    dependson(["NKPlatform"])

La commande a produit :

Build Order (2 projects):
  1. NKPlatform [STATIC_LIB]
  2. MonEssai [CONSOLE_APP] (depends: NKPlatform)

NKPlatform a ete compile et lie en premier. Puis MonEssai a ete compile
et lie avec succes :

BUILD COMPLETED
Projects Built:  2/2
Status: SUCCESS

Contrairement a ce qu'on pourrait attendre, ce cas reussit. J'ai pris
soin de prendre la fonction GetPlatformConfig dont le code est dans un
.cpp, donc un vrai symbole externe. Et pourtant le lien se fait sans
erreur undefined reference. Cela veut dire que dans Jenga 2.8.0, pour un
projet du workspace, dependson implique aussi le lien vers ce projet.
Ce n'est pas une erreur, c'est une decision de conception de Jenga.

Ce qui distingue les deux cas

dependson repond a la question : dans quel ordre construire. Il dit
que NKPlatform doit etre construit avant MonEssai.

links repond a la question : avec quoi lier. Il dit d'ajouter
NKPlatform.a au moment de l'edition de liens.

Quand on les confond, on obtient deux situations differentes :

Sans dependson, la bibliotheque n'est pas construite au moment du lien.
L'editeur de liens echoue parce qu'il ne trouve pas le fichier a lire.
C'est le message du Cas 1 : cannot find -lNKPlatform: No such file or
directory.

Sans links, la bibliotheque est bien construite grace a dependson, et
Jenga ajoute le lien automatiquement dans cette version. Il n'y a donc
pas d'erreur.

Ce qui reste vrai dans tous les cas

La distinction entre dependson et links est importante. Pour une
bibliotheque systeme, comme pthread, links est obligatoire : elle
existe deja sur la machine, mais il faut la declarer pour que le
compilateur l'ajoute a la ligne de lien. Pour une bibliotheque du
workspace, dans cette version de Jenga, dependson seul suffit.

Une remarque sur la version de Jenga

Dans le principe, dependson et links sont deux mecanismes distincts.
Dans Jenga 2.8.0 tel qu'il est installe ici, le comportement se
superpose pour les projets du workspace. Ce detail est utile a
connaitre : sur une autre version, on pourrait observer une erreur
undefined reference a la place du succes du Cas 2.

Etat du projet a la fin

J'ai laisse MonEssai dans l'etat du Cas 0, avec dependson et links
tous les deux, pour que le projet compile normalement si on le
relance.

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
Dernier commit du depot : 9c3fad3, date du 2026-09-13.
