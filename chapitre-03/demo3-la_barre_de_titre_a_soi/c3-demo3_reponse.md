Demonstration 3 - La barre de titre a soi

J'ai repris le programme de l'exercice 10 (fenetre sans bordure avec
barre de titre custom) et je l'ai fait tourner pour verifier qu'il
fonctionne. Puis j'ai liste ce qu'on perd en refaisant la barre du
systeme.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que fait le programme

Le programme reprend la structure de c3-exo10_main.cpp : fenetre sans
bordure via SetDecorated(false), barre de titre dessinee avec
NkRenderer2D (fond gris, titre ecrit avec NkFont, trois boutons a
droite), drag via BeginDragMove, detection des clics dans les zones.

Pour la demonstration, le programme fait quatre etats automatiquement
a des moments fixes, ce qui permet de capturer chaque etat sans avoir
a manipuler la souris :

- t = 0 : etat initial, 800x600 a (200,100)
- t = 15 s : deplacement a (400,300)
- t = 30 s : agrandissement a 1200x800
- t = 45 s : retour a 800x600 a (200,100)

Resultat du test

Sortie brute du programme :

    [etape 1] etat initial : 800x600 a (200,100)
    [etape 2] deplacement a (400,300)
    [etape 3] agrandissement a 1200x800
    [taille] 1200x800
    [etape 4] retour a 800x600 a (200,100)
    [taille] 800x600
    [fin]

Les quatre etapes se sont deroulees dans l'ordre. La barre de titre,
les trois boutons et le titre ecrit sont visibles a chaque etat.
Apres les appels a SetSize, la taille reelle de la fenetre correspond
a ce qui a ete demande : 1200x800 en etape 3, 800x600 en etape 4.

Ce que la barre fait

Deplacer la fenetre. Le callback NkMouseButtonPressEvent lit la
position du clic. Si le clic est dans la zone y 0 a 31 et hors des
trois boutons, le programme appelle window.BeginDragMove(). La fenetre
suit ensuite la souris.

Agrandir. Le bouton Maximize (deuxieme en partant de la droite)
appelle window.SetSize(1200, 800). Ce n'est pas un vrai maximize du
gestionnaire de fenetres, mais un agrandissement borne a une taille
raisonnable. La raison : Maximize() natif deborde l'ecran sur WSLg,
comme explique dans le rapport de l'exercice 10.

Fermer. Le bouton Close (le plus a droite, rouge) appelle
window.Close().

Reduire. Le bouton Minimize (le plus a gauche des trois) appelle
window.Minimize().

Ce que j'ai perdu par rapport a la barre du systeme

La barre de titre d'un gestionnaire de fenetres fournit beaucoup de
choses gratuitement. En la refaisant a la main, on recupere le
controle, mais on perd ces fonctions.

1. Le deplacement vers un autre ecran. Une barre systeme permet de
faire glisser une fenetre d'un ecran a l'autre, et le WM adapte la
taille, le dpi et la position. Notre barre ne connait que l'ecran ou
la fenetre a ete creee. Deplacer vers un autre ecran n'est pas gere.

2. Le double-clic pour maximiser. Une barre systeme detecte le
double-clic et maximise ou restaure. Notre programme enregistre un
callback sur NkMouseDoubleClickEvent, mais le backend X11 ne le produit
pas, comme documente dans notes/fonctions-sans-implementation.md
(trouvaille 5). Le double-clic ne fonctionne donc pas.

3. Le snap. Sur Windows, tirer une fenetre contre un bord la colle a
moitie d'ecran. Sur GNOME, la meme chose avec Super plus fleche. Notre
barre ne fait rien de tel. Il faudrait detecter la proximite d'un bord
pendant le drag et ajuster la taille.

4. Le menu systeme. Clic droit sur une barre systeme ouvre un menu
(Reduire, Agrandir, Deplacer, Fermer). Notre barre n'a pas de menu.

5. Les raccourcis clavier. Alt+F4 ferme, Alt+Espace ouvre le menu,
Super plus fleche fait le snap. Ces raccourcis sont fournis par le WM,
pas par la fenetre. Sans WM complet (ce qui est notre cas sous WSLg),
ils n'existent pas.

6. L'apparence native. La barre systeme suit le theme du bureau
(clair, sombre, accent color). Notre barre a une couleur fixe.

7. L'adaptation au facteur d'echelle. Une barre systeme s'adapte
automatiquement au dpi. La notre doit multiplier ses constantes par
window.GetDpiScale() a la main, comme montre dans la Demonstration 2.
Sinon elle est trop petite sur forte densite.

8. L'accessibilite. Une barre systeme est annoncee aux lecteurs
d'ecran. La notre non.

9. La restauration apres plantage du WM. Une barre systeme survit a un
redemarrage du WM. La notre disparait avec le programme.

10. La gestion du focus. Le WM decide quelle fenetre a le focus, quelle
fenetre recoit les clics. Notre barre n'a pas de notion de focus
globale.

Ce que cela montre

La barre de titre est un composant qu'on croit simple et qui concentre
des decennies de decisions du systeme. Le refaire, c'est decider
lesquelles on garde et lesquelles on laisse tomber. Notre programme
garde le strict necessaire : deplacer, agrandir, fermer. Tout le reste
est abandonne.

La Demonstration 1 avait deja montre le prix en lignes : 22 lignes
pour une fenetre decoree (exercice 1), 178 pour la meme chose sans
decoration (exercice 10). La Demonstration 3 montre le prix en
fonctionnalites.

Mesure faite le 28/09/2026.
Version de Jenga : 2.8.0.
