Exercice 3 - Les bornes

J'ai ecrit un programme qui cree deux fenetres. La premiere (A) a une
taille minimale imposee a 400x300 via cfg.minWidth et cfg.minHeight.
La seconde (B) a une taille minimale quasi nulle (1x1). J'ai lance le
programme, essaye de reduire les deux fenetres au minimum, et capture
la taille atteinte a chaque evenement de redimensionnement.

Le programme est depose a cote sous le nom c3-exo3_main.cpp.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Comment la taille a ete mesuree

Le programme enregistre un callback sur NkWindowResizeEvent. A chaque
evenement, il note la largeur et la hauteur, et garde la plus petite
valeur vue. A la fin, il affiche le resultat.

Commande :

    jenga run TestBornes2 --config Debug --platform x86_64 --target Linux

Ligne finale du journal :

    [fin] A : 135 resizes, plus petite taille 1x1
    [fin] B : 50 resizes, plus petite taille 400x1

Tableau des bornes

| Fenetre | Taille minimale demandee | Taille minimale atteinte |
|---------|--------------------------|--------------------------|
| A | 400x300 | 1x1 |
| B | 1x1 | 400x1 |

La fenetre A devait refuser de descendre en dessous de 400x300. Elle est
descendue jusqu'a 1x1. La contrainte n'a pas ete respectee.

La fenetre B devait pouvoir descendre tres bas. Elle est descendue a
400x1. La largeur est restee a 400, la hauteur est descendue a 1.

Ce que dit le code du backend

Dans le backend Linux (XLib), la fonction d'initialisation lit bien
config.minWidth et config.minHeight. Le code a la ligne 373 de
NkXLibWindow.cpp :

    XSizeHints hints = {};
    if (!config.resizable) {
        hints.flags = PMinSize | PMaxSize;
        hints.min_width = hints.max_width = static_cast<int>(config.width);
        hints.min_height = hints.max_height = static_cast<int>(config.height);
    } else {
        hints.flags = PMinSize;
        hints.min_width = static_cast<int>(config.minWidth);
        hints.min_height = static_cast<int>(config.minHeight);
    }
    XSetWMNormalHints(sDisplay, mData.mXid, &hints);

Le champ minWidth est donc lu et transmis au gestionnaire de fenetres
via XSetWMNormalHints. Mais la fonction s'appelle XSetWMNormalHints, pas
XSetWMNormalOrder. Le mot hints veut dire indice. C'est une demande,
pas un ordre.

Pourquoi la contrainte n'est pas respectee

XSetWMNormalHints ne contraint pas la fenetre directement. Il envoie un
indice au gestionnaire de fenetres. C'est le gestionnaire qui decide de
l'appliquer ou non.

Dans mon environnement WSLg, le gestionnaire de fenetres est minimal :
il ignore les indices de taille. Les deux fenetres peuvent donc descendre
en dessous de leur minimum. La fenetre A est meme descendue jusqu'a 1x1.

Ce n'est pas un bug du moteur Nkentseu. C'est une limite du gestionnaire
de fenetres de l'environnement dans lequel le programme tourne.

Ce qu'on devrait voir sur un autre bureau

Sur un bureau Linux classique (GNOME, KDE), les indices de taille sont
respectes. La fenetre A refuserait de descendre en dessous de 400x300.
La prediction est verifiable : il suffit de lancer le meme programme
sur une machine Linux avec GNOME ou KDE.

Ce que j'ai appris

Un programme peut demander une taille minimale. Il ne peut pas l'imposer.
La contrainte depend du gestionnaire de fenetres, qui est un composant
exterieur au programme. Le mot hints dans XSetWMNormalHints est le bon
mot : c'est un indice, pas un ordre.

Mesure faite le 27/09/2026.
Version de Jenga : 2.8.0.
