Exercice 11 - Deux fenetres

J'ai ecrit un programme qui ouvre deux fenetres et affiche, pour chaque
clic, laquelle l'a recu. Puis j'ai essaye de dessiner dans les deux et
mesure ce que cela demande.

Le programme est depose a cote sous le nom c3-exo11_main.cpp. Il fait
157 lignes.

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
fenetre. Le programme cree donc deux contextes OpenGL et deux
renderers 2D, un par fenetre.

Premier essai : deux contextes, un seul rend

Dans cet essai, je ne faisais rien de plus qu'a l'exercice 10, mais en
double. Un BeginFrame, Clear, Begin, Draw, End, EndFrame, Present pour
chaque fenetre, dans la meme boucle.

Resultat visuel : la fenetre A reste noire. La fenetre B affiche son
fond violet et son rectangle bleu.

Sortie brute du log :

    idA=1 idB=2
    Deux contextes et deux renderers crees
    [frame] 60
    [frame] 120
    ...

Le programme ne signale pas d'erreur. Les deux contextes ont ete crees
avec succes. Mais un seul rend quelque chose.

La cause

Un contexte OpenGL est une ressource par thread. A un instant donne,
un seul contexte est courant. C'est lui qui recoit les appels gl*.
Sans preciser lequel est courant, tous les appels vont au dernier
contexte rendu courant par le systeme, ici B.

Second essai : MakeCurrent a chaque frame

J'ai regarde l'interface de NkIGraphicsContext. Elle expose deux
methodes, lignes 49 et 53 de NkIGraphicsContext.h :

    virtual bool MakeCurrent() { return true; }
    virtual void ReleaseCurrent() { }

J'ai modifie la boucle pour rendre courant chaque contexte avant de
dessiner dedans, puis le relacher apres :

    gfxA->MakeCurrent();
    gfxA->BeginFrame();
    ...dessin dans A...
    gfxA->EndFrame();
    gfxA->Present();
    gfxA->ReleaseCurrent();

    gfxB->MakeCurrent();
    gfxB->BeginFrame();
    ...dessin dans B...
    gfxB->EndFrame();
    gfxB->Present();
    gfxB->ReleaseCurrent();

Resultat visuel : les deux fenetres rendent.

- Fenetre A : fond rouge fonce, rectangle rouge plus clair au centre.
- Fenetre B : fond bleu fonce, rectangle bleu plus clair au centre.

Les clics continuent d'etre attribues correctement a A ou B, comme au
premier test.

Ce qui manquait, mesure

Ce qui manque pour dessiner dans deux fenetres n'est donc pas seulement
deux contextes et deux renderers. C'est aussi un appel explicite a
MakeCurrent et ReleaseCurrent autour de chaque rendu.

Sans MakeCurrent : un seul contexte est courant, un seul rend.
Avec MakeCurrent : les deux rendent.

La ligne qui change tout est celle-ci :

    gfxA->MakeCurrent();

Une ligne par fenetre, par frame. Ce n'est pas une fonction cachee,
c'est une discipline a tenir.

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
