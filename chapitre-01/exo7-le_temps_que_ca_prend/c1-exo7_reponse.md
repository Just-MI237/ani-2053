Exercice 7 - Le temps que ca prend

J'ai lance deux constructions l'une apres l'autre et j'ai chronometre
chacune avec la commande time. J'ai ensuite essaye de corriger l'erreur
de compilation pour comprendre ce qui bloquait, et cette tentative m'a
appris quelque chose sur l'etat du depot.

Premiere construction

J'ai d'abord supprime le dossier Build pour partir d'un etat propre :

    cd ~/Projets/Nkentseu
    rm -rf Build/
    time jenga build --config Debug

Resultat :

    real    1m1.578s
    user    3m58.105s
    sys     0m25.611s

Jenga a compile 19 projets sur 227 avant de s'arreter sur une erreur
dans NkDirectory.cpp :

    error: no member named 'SubString' in 'nkentseu::NkString';
    did you mean 'SubStr'?

Le code appelait une methode qui n'existe pas. La methode disponible
s'appelle SubStr. Ce n'est pas une erreur de ma part, c'est un defaut
du depot.

Deuxieme construction

J'ai relance exactement la meme commande, sans rien modifier :

    time jenga build --config Debug

Resultat :

    real    0m6.404s
    user    0m2.358s
    sys     0m0.819s

Jenga a affiche All files up to date pour la plupart des projets. Il a
recompile un seul fichier, SandboxNKLogger/main.cpp, puis il s'est
arrete a la meme erreur dans NkDirectory.cpp.

Explication de l'ecart

La premiere construction prend environ 1 minute parce que Jenga compile
reellement les fichiers source. La deuxieme prend environ 6 secondes
parce que Jenga compare les dates des fichiers source a celles des
fichiers objets deja produits. Si rien n'a change, il ne recompile pas.
Une construction n'est donc pas une fonction du nombre de lignes du
depot, mais du nombre de lignes modifiees depuis la derniere
construction.

C'est ce qui rend un depot d'un million de lignes utilisable au
quotidien : sans cette optimisation, chaque modification demanderait
de tout recompiler.

Remarque sur les 6 secondes restantes

Les 6 secondes de la deuxieme construction ne sont pas entierement
inutiles. Jenga parcourt le workspace, lit tous les fichiers .jenga,
reconstruit le graphe de dependances, verifie les dates de chaque
fichier. C'est ce travail de verification qui prend ces quelques
secondes. Une fois ce travail fait, s'il n'y a rien a recompiler, il ne
reste plus qu'a rendre la main.

Tentative de correction de l'erreur

J'ai cherche toutes les occurrences de SubString dans le depot :

    grep -rln "SubString" --include="*.cpp" --include="*.h" \
        Kernel/ Engine/ Applications/ | grep -v Build

Deux fichiers sont sortis :
- Kernel/Foundation/NKPlatform/src/NKPlatform/NkCPUFeatures.cpp
- Kernel/System/NKFileSystem/src/NKFileSystem/NkDirectory.cpp

J'ai regarde le premier : les occurrences sont en fait des appels a
ContainsSubString, une fonction qui existe vraiment. Ce n'est pas un
bug.

Le deuxieme fichier contenait bien l'appel fautif, a la ligne 959 :

    v = home.ToString() + v.SubString(5);

J'ai corrige avec :

    sed -i 's/\.SubString(/.SubStr(/g' \
        Kernel/System/NKFileSystem/src/NKFileSystem/NkDirectory.cpp

Cette commande remplace uniquement les appels de methode .SubString(,
sans toucher a ContainsSubString.

Resultat apres correction

Apres la correction, j'ai relance :

    jenga build --config Debug

Le build a progresse de 19 a 29 projets construits. La correction a
donc debloque plusieurs modules. Mais une nouvelle erreur est
apparue, dans un autre module.

Nouvelle erreur, dans NKSerialization

Le build s'arrete maintenant dans NkGuiArchive.cpp :

    NkSchemaVersioning.h:130: error: static variable not permitted
        in a constexpr function
        static const char s_tag = 0;

    NkGuiArchive.cpp:1750: error: no matching function for call to
        'NkTypeOf'
        return NkTypeOf<NkGuiDocumentTag>();

Ce sont deux erreurs differentes de la premiere.

La premiere dit qu'une fonction constexpr contient une variable static,
ce qui n'est pas autorise en C++17. C'est une erreur de code.

La deuxieme dit que le template NkTypeOf ne peut pas s'instancier pour
le type NkGuiDocumentTag. C'est une erreur de template, plus subtile :
soit le type n'a pas les proprietes attendues, soit la specialisation
manque.

Ce que cela revele

Ces deux nouvelles erreurs ne sont pas dans ma configuration. Elles
sont dans le code source du moteur lui-meme. Le depot Nkentseu est un
projet en cours de developpement, et il n'est pas garanti qu'il compile
entierement a toutes les dates.

La correction que j'ai faite sur SubString etait un vrai bug, simple a
identifier et simple a corriger. Les erreurs suivantes demandent une
comprehension du code que je n'ai pas encore. Les corriger moi-meme
serait risquer de casser autre chose sans le savoir.

Cela montre une chose utile sur un projet reel : un depot qui ne
compile pas entierement ne veut pas dire qu'il est inutilisable. On
peut travailler sur les modules qui compilent, utiliser leurs tests, et
laisser les modules casses de cote. Ce qui compte, c'est de savoir ou
sont les frontieres.

Le build complet n'est donc pas mon objectif. Mon objectif etait de
comprendre le mecanisme de chronometrage et l'ecart entre les deux
constructions. Ce point est valide.

Ce que j'ai appris

- Une construction a froid est longue, une construction a chaud est
  tres rapide, et l'ecart vient de la comparaison des dates.
- Un depot reel peut contenir des erreurs de compilation, meme s'il
  est publie. C'est normal sur une branche de developpement.
- Corriger une erreur peut en reveler une autre, et il faut savoir
  s'arreter quand la correction depasse ce qu'on comprend.

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
Dernier commit du depot : 9c3fad3, date du 2026-09-13.
