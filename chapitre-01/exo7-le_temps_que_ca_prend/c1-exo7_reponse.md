Exercice 7 - Le temps que ca prend

J'ai lance deux constructions l'une apres l'autre et j'ai chronomètre
chacune avec la commande time.

Premiere construction

J'ai d'abord supprime le dossier Build pour partir d'un etat propre :

    cd ~/Projets/Nkentseu
    rm -rf Build/
    time jenga build --config Debug

Resultat :

    real    1m1.578s
    user    3m58.105s
    sys     0m25.611s

Jenga a compile 19 projets sur 227 avant de s'arreter sur une erreur de
compilation dans le moteur lui-meme, dans NkDirectory.cpp : le code
appelle SubString alors que la methode disponible s'appelle SubStr. Ce
n'est pas une erreur de ma part, c'est un defaut du depot.

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

Remarque

Les 6 secondes de la deuxieme construction ne sont pas entierement
inutiles. Jenga parcourt le workspace, lit tous les fichiers .jenga,
reconstruit le graphe de dependances, verifie les dates de chaque
fichier. C'est ce travail de verification qui prend ces quelques
secondes. Une fois ce travail fait, si rien n'a change, il ne reste
plus qu'a ne rien compiler.

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
Dernier commit du depot : 9c3fad3, date du 2026-09-13.
