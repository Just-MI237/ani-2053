Exercice 11 - Deux fenetres

J'ai ecrit un programme qui ouvre deux fenetres et affiche, pour chaque
clic, laquelle l'a recu. Puis j'ai reflechi a ce qui manquerait pour
dessiner dans les deux.

Le programme est depose a cote sous le nom c3-exo11_main.cpp.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que fait le programme

Le programme cree deux fenetres, A et B, avec des titres et des
positions differents. A est a (100,100) et fait 400x300. B est a
(600,100) et fait aussi 400x300.

Pour chaque fenetre, le programme note son identifiant :

    NkWindowId idA = wa.GetId();
    NkWindowId idB = wb.GetId();

Il enregistre ensuite un seul callback sur NkMouseButtonPressEvent.
Dans ce callback, il lit l'identifiant de la fenetre qui a recu
l'evenement et le compare aux deux identifiants memorises :

    uint64 id = e->GetWindowId();
    if (id == idA) qui = "A";
    else if (id == idB) qui = "B";

Le meme callback traite donc les deux fenetres, et distingue l'origine
par l'identifiant.

Resultat du test

Sortie brute du programme, apres avoir clique plusieurs fois dans
chaque fenetre :

    idA=1 idB=2
    [clic] fenetre A (id=1) at client=(213,109)
    [clic] fenetre B (id=2) at client=(126,130)
    [clic] fenetre A (id=1) at client=(9,43)
    [clic] fenetre B (id=2) at client=(18,45)
    [clic] fenetre A (id=1) at client=(124,111)
    [clic] fenetre B (id=2) at client=(193,102)

Les deux fenetres ont des identifiants distincts : 1 pour A, 2 pour B.
Chaque clic est attribue a la bonne fenetre, sans ambiguite. Les
coordonnees sont donnees en client : le coin haut-gauche de la fenetre
concernee est toujours (0,0).

Ce qui manquerait pour dessiner dans les deux

L'exercice demande de dire ce qui manquerait pour dessiner dans les
deux fenetres. En m'appuyant sur ce que j'ai fait a l'exercice 10, la
reponse est la suivante.

Dessiner dans une fenetre demande trois choses :

1. Un contexte graphique, cree avec NkContextFactory::Create(window, desc).
2. Un renderer 2D, cree avec NkRenderer2DFactory::Create(gfx).
3. Une boucle de rendu qui appelle gfx->BeginFrame(), r2d->Clear(),
   r2d->Begin(), les appels Draw*, r2d->End(), gfx->EndFrame(),
   gfx->Present().

Pour deux fenetres, il faut deux fois tout cela :

- deux contextes graphiques, un par fenetre
- deux renderers 2D
- deux appels a Present() par tour, un dans chaque fenetre
- deux vues, une par fenetre, chacune mise a jour quand sa fenetre
  change de taille

Ce que cela demande en plus par rapport a un seul contexte

Le programme actuel n'a ni contexte ni renderer. C'est un programme
d'evenements seulement. Pour passer a un programme de rendu, il faut
monter ces objets.

A l'exercice 10, monter un seul contexte et un seul renderer a demande
plus de temps que d'ecrire la barre de titre elle-meme, parce qu'il a
fallu decouvrir la chaine de dependances Jenga. Pour deux, c'est deux
fois le meme travail, plus la gestion de deux boucles.

Deux strategies possibles

Strategie 1 : une seule boucle principale, avec un contexte par
fenetre. Dans la boucle, on fait BeginFrame sur les deux contextes,
on dessine dans chacun, on appelle EndFrame et Present sur les deux.

Strategie 2 : deux boucles, une par fenetre, eventuellement dans deux
threads. Chaque thread gere son contexte et son renderer. Mais il faut
alors resoudre les problemes de concurrence : NkEventSystem est-il
thread-safe ? Sur quelle fenetre les evenements arrivent-ils quand on
est dans deux threads ? Ce n'est pas quelque chose que j'ai explore ici.

L'exercice ne demande pas de choisir, seulement de dire ce qui
manquerait. La reponse est : deux contextes, deux renderers, et un
moyen de coordonner les deux.

Ce que cela montre

NkEventSystem gere plusieurs fenetres nativement. L'identifiant voyage
avec chaque evenement. Un seul callback peut traiter les evenements de
toutes les fenetres, a condition de lire GetWindowId() pour savoir
laquelle a parle.

Le rendu, en revanche, n'a pas d'equivalent. Il n'y a pas de fonction
qui dit dessine dans la fenetre A et dans la fenetre B avec la meme
boucle. Chaque fenetre a besoin de son propre contexte et de son
propre renderer. C'est une asymetrie entre la couche evenement et la
couche rendu.

Ce n'est pas forcement un defaut : un contexte graphique est une
ressource lourde, et il est normal qu'il y en ait un par surface. Mais
ca veut dire qu'une application multi-fenetres doit gerer elle-meme la
multiplication.

Mesure faite le 28/09/2026.
Version de Jenga : 2.8.0.
