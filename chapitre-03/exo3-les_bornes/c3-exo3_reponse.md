Exercice 3 - Les bornes

J'ai ecrit un programme qui cree deux fenetres. La premiere (A) a
une taille minimale imposee a 400x300 via cfg.minWidth et
cfg.minHeight. La seconde (B) a une taille minimale quasi nulle
(1x1). J'ai lance le programme et essaye de reduire les deux
fenetres.

Le programme est depose a cote sous le nom c3-exo3_main.cpp. Il fait
31 lignes.

Datation de la mesure

Commande :

```
git log -1 --format="%h %ad" --date=short
```

Sortie brute :

```
9c3fad3 2026-09-13
```

Version de Jenga : 2.8.0.

Ce que dit le chapitre

Le chapitre 3 decrit NkWindowConfig avec des champs minWidth et
minHeight. L'idee est qu'on peut fixer une taille minimale a une
fenetre, et que le systeme refuse de la reduire en dessous.

Ce que dit le code du backend

Dans le backend Linux (XLib), la fonction d'initialisation lit bien
config.minWidth et config.minHeight. Voici le code a la ligne 373
de NkXLibWindow.cpp :

```
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
```

Le champ minWidth est donc lu et transmis au gestionnaire de
fenetres via XSetWMNormalHints.

Ce que j'ai observe

Fenetre A (min 400x300) :
  J'ai pu reduire la fenetre en dessous de 400x300. Aucune contrainte
  visible. La fenetre est descendue jusqu'a une taille minuscule.

Fenetre B (min 1x1) :
  J'ai pu reduire la fenetre jusqu'a une taille minuscule, comme
  attendu. Aucune contrainte visible.

Tableau des deux cas

| Fenetre | Taille minimale demandee | Taille minimale observee |
|---------|-------------------------|--------------------------|
| A | 400x300 | inferieure a 400x300 |
| B | 1x1 | proche de 1x1 |

Pourquoi ca ne fonctionne pas comme prevu

XSetWMNormalHints ne contraint pas la fenetre directement. Il envoie
un indice au gestionnaire de fenetres. C'est le gestionnaire qui
decide de l'appliquer ou non. Dans mon environnement WSLg, le
gestionnaire de fenetres est minimal : il ignore les indices de
taille. Les deux fenetres peuvent donc descendre en dessous de
leur minimum.

Ce n'est pas un bug du moteur Nkentseu. C'est une limite du
gestionnaire de fenetres de l'environnement dans lequel le
programme tourne. Sur un bureau Linux classique (GNOME, KDE), les
indices seraient respectes.

Ce que j'ai appris

Un programme peut demander une taille minimale. Il ne peut pas
l'imposer. La contrainte depend du gestionnaire de fenetres, qui
est un composant exterieur au programme.

Mesure faite le 27/09/2026.
Version de Jenga : 2.8.0.
