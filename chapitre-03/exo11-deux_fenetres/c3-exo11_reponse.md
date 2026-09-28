Exercice 11 - Deux fenetres

J'ai ecrit un programme qui ouvre deux fenetres et affiche, pour chaque
clic, laquelle l'a recu. Puis j'ai essaye de dessiner dans les deux et
mesure ce que cela demande.

Le programme est depose a cote sous le nom c3-exo11_main.cpp. Il fait
153 lignes.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que fait le programme

Le programme cree deux fenetres, A et B. A est a (100,100) et fait
400x300. B est a (600,100) et fait aussi 400x300.

Pour chaque fenetre, le programme note son identifiant :

    NkWindowId idA = wa.GetId();
    NkWindowId idB = wb.GetId();

Il enregistre un seul callback sur NkMouseButtonPressEvent. Dans ce
callback, il lit l'identifiant de la fenetre qui a recu l'evenement et
le compare aux deux identifiants memorises. Le meme callback traite les
deux fenetres, et distingue l'origine par l'identifiant.

Premier test : detection des clics

J'ai lance le programme et clique plusieurs fois dans chaque fenetre.
Sortie brute :

    idA=1 idB=2
    [clic] fenetre A (id=1) at client=(213,109)
    [clic] fenetre B (id=2) at client=(126,130)
    [clic] fenetre A (id=1) at client=(9,43)
    [clic] fenetre B (id=2) at client=(18,45)

Les deux fenetres ont des identifiants distincts : 1 pour A, 2 pour B.
Chaque clic est attribue a la bonne fenetre, sans ambiguite. Les
coordonnees sont donnees en client : le coin haut-gauche de la fenetre
concernee est toujours (0,0).

Second test : dessiner dans les deux

J'ai ajoute un contexte graphique et un renderer 2D pour chaque
fenetre. Le meme programme cree donc deux contextes OpenGL et deux
renderers 2D, un par fenetre.

Dans la boucle principale, pour chaque fenetre, le programme fait :

    gfx->BeginFrame()
    r2d->Clear(couleur de fond)
    r2d->Begin()
    r2d->SetView(vue de la fenetre)
    r2d->DrawFilledRect(...)
    r2d->End()
    gfx->EndFrame()
    gfx->Present()

Le programme fait cela deux fois, une fois pour A, une fois pour B,
dans la meme iteration de la boucle.

Sortie brute, au demarrage et pendant 25 secondes :

    idA=1 idB=2
    Deux contextes et deux renderers crees
    [frame] 60
    [frame] 120
    ...
    [frame] 1500
    [clic] fenetre A (id=1) at client=(120,250)
    [fin]

Sur l'ecran, les deux fenetres ont des couleurs differentes :

- Fenetre A : fond rouge fonce, rectangle rouge plus clair au centre
- Fenetre B : fond bleu fonce, rectangle bleu plus clair au centre

Les deux fenetres sont dessinees, chacune avec sa couleur. Les clics
continuent d'etre detectes sur les deux.

Ce qui manquerait pour dessiner dans les deux, mesure

Ce qui manque, ce n'est pas une fonction cachee dans NkWindow. C'est
un deuxieme exemplaire de chaque objet de rendu :

- un deuxieme contexte graphique, cree avec NkContextFactory::Create
- un deuxieme renderer 2D, cree avec NkRenderer2DFactory::Create
- un deuxieme appel a BeginFrame / EndFrame / Present par tour
- une deuxieme vue, mise a jour quand la fenetre change de taille

Le programme montre cela. Il n'y a pas de fonction qui dit dessine dans
les deux fenetres avec une seule boucle. Il faut deux fois tout, et une
boucle qui appelle les deux.

Ce que cela montre

NkEventSystem gere plusieurs fenetres nativement. L'identifiant voyage
avec chaque evenement. Un seul callback peut traiter les evenements de
toutes les fenetres, a condition de lire GetWindowId().

Le rendu n'a pas d'equivalent. Il n'y a pas de fonction unique qui prend
une liste de fenetres et qui dessine dans toutes. Chaque fenetre a
besoin de son propre contexte et de son propre renderer. C'est une
asymetrie entre la couche evenement et la couche rendu.

Ce n'est pas forcement un defaut : un contexte graphique est une
ressource lourde, et il est normal qu'il y en ait un par surface. Mais
ca veut dire qu'une application multi-fenetres doit gerer elle-meme la
multiplication.

Mesure faite le 28/09/2026.
Version de Jenga : 2.8.0.
