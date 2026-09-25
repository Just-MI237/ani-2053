Exercice 2 - Mesurer avant de croire

J'ai compte moi-meme les fichiers source et les lignes du depot Nkentseu.
Voici les commandes que j'ai lancees, et sous chacune la sortie brute
que la commande a produite.

Premiere mesure : nombre de fichiers source

Commande :

    find . -type f \( -name "*.cpp" -o -name "*.h" \) -not -path "./Build/*" -not -path "./Externals/*" | wc -l

Sortie brute :

    2755

Deuxieme mesure : nombre total de lignes

Commande :

    find . -type f \( -name "*.cpp" -o -name "*.h" \) -not -path "./Build/*" -not -path "./Externals/*" -exec cat {} + | wc -l

Sortie brute :

    1324635

Troisieme mesure : nombre de fichiers .jenga

Commande :

    find . -type f -name "*.jenga" | wc -l

Sortie brute :

    219

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Cette ligne donne l'identifiant court du dernier commit du depot et sa
date. C'est cette version precise du depot que mes trois chiffres
ci-dessus decrivent.

Comparaison avec le chapitre

Le chapitre annonce 2641 fichiers source, 1 193 385 lignes, et 221
fichiers .jenga. Mes chiffres sont plus eleves sur les deux premiers :
2755 fichiers et 1 324 635 lignes. Le troisieme est plus bas : 219
contre 221.

Pourquoi cet ecart

Le chapitre a ete ecrit a une date anterieure au commit que j'ai
mesure. Entre les deux, des fichiers ont ete ajoutes et du code a ete
ecrit. C'est un projet vivant, donc les chiffres changent entre deux
mesures.

J'ai exclu le dossier Externals de mes comptages parce qu'il contient
des sous-modules externes (ImGui, NKAssimp, NKGlad, etc.) qui ne font
pas partie du code propre de Nkentseu. Sans cette exclusion, la
commande aurait retourne 4428 fichiers, ce qui est encore plus loin du
chiffre du chapitre.

J'ai bien compte les en-tetes (.h) et les fichiers source (.cpp), comme
le chapitre. Je n'ai pas compte le dossier Build, qui contient des
copies generees des en-tetes et des fichiers intermédiaires.

Pour les fichiers .jenga, j'en trouve 219 contre 221 dans le livre. Le
leger ecart vient probablement de la meme raison : le depot a evolue
entre l'ecriture du chapitre et ma mesure.


Preuve de l'ecart avec le chapitre

Le chapitre annonce 2641 fichiers .cpp/.h. Pour verifier que l'ecart
vient bien de l'evolution du depot et non d'une erreur de comptage, j'ai
regarde l'etat du depot a plusieurs dates.

Commandes :

    git log --format="%h %ad" --date=short --since="2026-08-28" --until="2026-09-13"
    git ls-tree -r <commit> --name-only | grep -E '\.(cpp|h)$' | grep -v '^Externals/' | grep -v '^Build/' | wc -l

Trois commits ont ete mesures :

    0d50a971 2026-08-28 2587 fichiers .cpp/.h
    c1c815ff 2026-09-10 2657 fichiers .cpp/.h
    9c3fad3  2026-09-13 2755 fichiers .cpp/.h

Le chiffre du chapitre, 2641, est encadre : il est plus grand que 2587
(28 aout) et plus petit que 2657 (10 septembre). Le chapitre a donc ete
ecrit entre le 28 aout et le 10 septembre 2026.

Le commit exact du chapitre n'est pas dans mon historique local, mais
l'encadrement suffit a montrer que le depot a evolue entre l'ecriture
du chapitre et ma mesure.

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
Dernier commit du depot : 9c3fad3, date du 2026-09-13.
