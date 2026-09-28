Demonstration 2 - Le defaut invisible

J'ai ecrit un programme qui dessine deux barres horizontales de meme
hauteur voulue : 32 pixels logiques, soit la hauteur d'un bouton
standard a 96 dpi. La premiere barre est dessinee en pixels bruts. La
deuxieme est dessinee en pixels multiplies par le facteur d'echelle de
l'ecran. J'ai lance le programme sur deux bancs de test, l'un a 96 dpi,
l'autre a 192 dpi. Voici ce que j'ai mesure.

L'enonce ne demande qu'un fichier, c3-demo2_reponse.md. Le code du
programme est decrit dans ce document. Il n'y a pas de fichier
c3-demo2_main.cpp a deposer.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que fait le programme

Le programme ouvre une fenetre, lit le facteur d'echelle de l'ecran
avec window.GetDpiScale(), puis dessine deux rectangles avec un
renderer 2D.

Le premier rectangle fait 32 pixels de haut, sans transformation. C'est
la maniere naturelle d'ecrire l'interface : on veut un bouton de 32
pixels, on demande 32 pixels.

Le deuxieme rectangle fait 32 pixels multiplies par le facteur
d'echelle. A 96 dpi, le facteur vaut 1.0, donc la hauteur reste 32. A
192 dpi, le facteur vaut 2.0, donc la hauteur passe a 64.

Les lignes du programme qui font la difference :

    float32 scale = window.GetDpiScale();

    int naiveH     = WANTED_H;
    int correctedH = (int)(WANTED_H * scale);

    // Barre 1 : pixels bruts (faux sur forte densite)
    r2d->DrawFilledRect({50.f, 100.f, 300.f, (float)naiveH}, {200, 80, 80, 255});

    // Barre 2 : pixels x dpiScale (correct partout)
    r2d->DrawFilledRect({50.f, 200.f, 300.f, (float)correctedH}, {80, 180, 120, 255});

WANTED_H vaut 32. La barre 1 utilise la valeur telle quelle. La barre
2 la multiplie par le facteur d'echelle lu au demarrage. C'est la seule
difference entre les deux.

Le programme affiche les trois valeurs au demarrage : le facteur
d'echelle, la hauteur naive, et la hauteur corrigee.

Test 1 : WSLg, ecran a 96 dpi

Commande :

    jenga run TestDefautInvisible --config Debug --platform x86_64 --target Linux

Sortie brute :

    dpiScale = 1.000000
    Taille naive (pixels bruts) : 32 px
    Taille corrigee (x scale)    : 32 px

Sur WSLg, dpiScale vaut 1.0. Les deux hauteurs sont egales, 32 pixels.
A l'ecran, les deux barres ont la meme hauteur. Impossible de
distinguer celle qui est naive de celle qui est corrigee. C'est le cas
normal : a 96 dpi, multiplier par 1.0 ne change rien.

Test 2 : Xvfb a 192 dpi

Commande :

    Xvfb :98 -screen 0 1024x768x24 -dpi 192 -listen tcp -nolisten unix
    DISPLAY=localhost:98 ./TestDefautInvisible

Sortie brute :

    dpiScale = 2.006913
    Taille naive (pixels bruts) : 32 px
    Taille corrigee (x scale)    : 64 px

Sur Xvfb a 192 dpi, dpiScale vaut 2.006913, soit environ 2. La hauteur
naive reste a 32 pixels. La hauteur corrigee passe a 64.

A l'ecran, si l'environnement etait visible, la premiere barre ferait
la moitie de la hauteur de la deuxieme. Elles devraient avoir la meme
taille logique, mais elles n'en ont pas : la premiere est deux fois
trop petite.

Pourquoi 2.006913 et pas 2.000000

Le facteur n'est pas exactement 2 parce que Xvfb calcule le dpi depuis
la taille de l'ecran en millimetres. Avec -dpi 192, il convertit la
resolution de 1024x768 pixels en une taille physique qu'il arrondit au
millimetre. L'arrondi produit un dpi legerement different de 192. Le
facteur en decoule.

L'ecart n'est pas un probleme. Ce qui compte, c'est qu'il est
strictement superieur a 1 : la barre naive est trop petite.

Pourquoi c'est un defaut invisible

Le developpeur qui ecrit 32 pixels teste sur son ecran, a 96 dpi. Il
voit les deux barres identiques. Il ne remarque rien. Il livre.

L'utilisateur qui a un ecran a 192 dpi voit la barre naive deux fois
plus fine que prevu. Les proportions de l'interface sont cassees. Mais
il ne peut pas le signaler comme un bug clair : le programme
fonctionne, la barre est la, juste plus petite. C'est un defaut de
proportion, pas un arret.

Le correctif change l'endroit ou la taille est demandee

La correction n'est pas dans le dessin. Elle est dans la lecture de la
taille. Au lieu d'ecrire :

    float hauteur = 32.f;

on ecrit :

    float hauteur = 32.f * window.GetDpiScale();

Le nombre 32 reste le meme. Ce qui change, c'est qu'il est interprete
comme une taille logique plutot que comme une taille physique. Le
facteur d'echelle est applique au moment ou on demande la taille, pas
au moment ou on la dessine.

C'est ce que l'enonce demande : changer l'endroit ou la taille est
demandee. La fonction de dessin ne connait pas le dpi. La fonction qui
prepare la valeur, elle, le connait.

Ce que cela montre

Un programme en pixels bruts est correct sur une seule densite. Sur
toutes les autres, il est faux d'un facteur egal au rapport de densite.
Comme les ecrans a forte densite sont de plus en plus courants et que
le developpeur travaille rarement dessus, le defaut est invisible pour
lui et visible pour une partie croissante des utilisateurs.

La correction tient en une multiplication. Mais il faut savoir ou la
placer. Ce n'est pas la fonction de dessin qui doit la faire, parce
qu'elle ne sait pas ce qu'elle dessine. C'est l'appelant, qui connait
l'intention (32 pixels logiques) et peut la traduire avec le facteur
d'echelle de l'ecran ou la fenetre se trouve.

Mesure faite le 28/09/2026.
Version de Jenga : 2.8.0.
