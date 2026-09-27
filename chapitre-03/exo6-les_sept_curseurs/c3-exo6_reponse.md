Exercice 6 - Les sept curseurs

J'ai ecrit un programme qui cree une fenetre de 840x360 et la divise en
sept zones de 120x360. Quand la souris entre dans une zone, le programme
appelle SetCursor avec la forme de curseur demandee par l'enonce.

Le programme est depose a cote sous le nom c3-exo6_main.cpp. Il fait
77 lignes.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Les sept zones

Chaque zone fait 120 pixels de large sur 360 de haut. Elles couvrent
toute la fenetre, de gauche a droite.

| Zone | Coordonnees | Forme demandee |
|------|-------------|----------------|
| 1 | x de 0 a 119 | Arrow |
| 2 | x de 120 a 239 | TextInput |
| 3 | x de 240 a 359 | Hand |
| 4 | x de 360 a 479 | ResizeNS |
| 5 | x de 480 a 599 | ResizeWE |
| 6 | x de 600 a 719 | ResizeNWSE |
| 7 | x de 720 a 839 | ResizeNESW |

Le programme enregistre un callback sur NkMouseMoveEvent. A chaque
mouvement, il lit GetX() et GetY(), cherche dans quelle zone se trouve
la souris, et si la zone a change depuis le dernier mouvement, il
appelle window.SetCursor avec la forme associee.

Ce que le log montre

J'ai promene la souris dans les sept zones. Le log montre que les sept
zones ont ete traversees, et que SetCursor est appele avec les bons
arguments a chaque changement.

    [zone] ResizeNS, SetCursor appele avec 3
    [zone] Hand, SetCursor appele avec 2
    [zone] TextInput, SetCursor appele avec 1
    [zone] Arrow, SetCursor appele avec 0
    [zone] TextInput, SetCursor appele avec 1
    [zone] Hand, SetCursor appele avec 2
    [zone] ResizeNS, SetCursor appele avec 3
    [zone] ResizeWE, SetCursor appele avec 4
    [zone] ResizeNWSE, SetCursor appele avec 5
    [zone] ResizeNESW, SetCursor appele avec 6

Le code fait donc ce qu'il doit faire : il detecte la bonne zone et
appelle SetCursor avec la bonne valeur.

Ce que je vois vraiment

Rien ne change dans la fenetre. Le curseur reste une fleche partout,
meme dans les zones TextInput, Hand, ResizeNS, ResizeWE, ResizeNWSE et
ResizeNESW.

Aux bords et aux coins de la fenetre, en revanche, le curseur change de
forme. Je l'ai vu dans un premier test. Mais ces endroits sont en dehors
des sept zones : ils appartiennent au cadre de la fenetre. C'est le
gestionnaire de fenetres de WSLg qui affiche ses propres curseurs de
redimensionnement a cet endroit, pas mon programme.

Tableau recapitulatif

| Zone | Forme demandee | Forme obtenue |
|------|----------------|---------------|
| Arrow | Arrow | Arrow |
| TextInput | TextInput | Arrow |
| Hand | Hand | Arrow |
| ResizeNS | ResizeNS | Arrow |
| ResizeWE | ResizeWE | Arrow |
| ResizeNWSE | ResizeNWSE | Arrow |
| ResizeNESW | ResizeNESW | Arrow |

Six des sept formes ne changent pas. Seule Arrow, la forme par defaut,
correspond a ce qui est demande.

Pourquoi les six formes ne changent pas

J'ai lu le backend de ma plateforme dans le moteur. Le fichier
NkWindowCursor.cpp contient deux implementations de SetCursor.

La premiere, entre les lignes 12 et 52, est pour Windows. Elle traduit
NkCursorType en IDC_ARROW, IDC_IBEAM, IDC_HAND, IDC_SIZENS, IDC_SIZEWE,
IDC_SIZENWSE, IDC_SIZENESW, et appelle ::SetCursor avec le resultat.

La seconde, a la ligne 56, est pour les autres plateformes :

    void NkWindow::SetCursor(NkCursorType /*cursor*/) {
        // Plateformes sans curseur souris (Android, iOS, Web tactile, headless).
    }

Le parametre est commente. La fonction ne fait rien. C'est la ligne 56
qui explique pourquoi les six formes ne changent pas. Mon appel arrive
bien a la fonction, mais la fonction est vide sur Linux/X11.

Le commentaire cite Android, iOS, Web tactile et headless. Linux avec X11
n'est pas dans cette liste. C'est une omission du backend XLib, pas un
choix delibere. Le curseur pourrait etre pose avec XCreateFontCursor et
XDefineCursor, mais le code ne le fait pas.

Le test a un seul appel

L'enonce demande ensuite de remplacer les appels par un seul, au
demarrage, et de dire ce qui se passe quand la souris change de zone.

J'ai modifie le programme : SetCursor est appele une seule fois, au
demarrage, avec la valeur Arrow. La detection de zone reste, mais elle
ne fait plus que journaliser le nom de la zone, sans appeler SetCursor.

Le log du second test :

    [demarrage] SetCursor(Arrow) appele une seule fois
    [zone] ResizeNS
    [zone] Hand
    [zone] TextInput
    [zone] Arrow
    [zone] TextInput
    [zone] Hand
    [zone] ResizeNS
    [zone] ResizeWE
    [zone] ResizeNWSE
    [zone] ResizeNESW

La souris a bien traverse les sept zones. Le programme les detecte. Mais
il n'appelle plus SetCursor.

Resultat visuel : le curseur reste une fleche partout, comme dans le
premier test. Il n'y a aucune difference entre appeler SetCursor a
chaque changement de zone et l'appeler une seule fois au demarrage.

C'est logique. Puisque SetCursor ne fait rien sur cette plateforme,
l'appeler une fois ou cent fois revient au meme. Le seul appel au
demarrage est suffisant dans les deux cas : il ne produit rien.

Ce que cela montre

Le programme fait tout ce que l'enonce demande : sept zones, detection
de la zone survolee, appel de SetCursor avec la bonne forme. Le moteur,
lui, ne pose le curseur que sur Windows. Sur Linux/X11, la fonction
SetCursor est vide.

L'enonce dit : « Un curseur qu'aucun code ne pose est une reponse
complete, pas un echec. » C'est exactement notre cas. Nous avons la
ligne 56 qui explique l'ecart, et le tableau a trois colonnes qui le
documente.

Mesure faite le 27/09/2026.
Version de Jenga : 2.8.0.
