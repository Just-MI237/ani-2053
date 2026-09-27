Exercice 7 - Le glisser qui sort

J'ai ecrit un programme qui cree une fenetre de 640x480 et qui
journalise tous les evenements souris : appui gauche, deplacements,
relachement. Le programme lit une variable d'environnement NK_MODE qui
vaut none, capture ou clip. Selon le mode, il active ou non la capture
de la souris avant la boucle principale.

Le programme est depose a cote sous le nom c3-exo7_main.cpp. Il fait
99 lignes.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que fait le programme

Trois modes sont disponibles :

- none : aucune capture demandee. C'est le comportement par defaut.
- capture : window.CaptureMouse(true) est appele au demarrage.
- clip : window.ClipMouseToClient(true) est appele au demarrage.

Dans les trois modes, le programme enregistre deux callbacks :
NkMouseButtonPressEvent, NkMouseButtonReleaseEvent et NkMouseMoveEvent.
A chaque appui gauche, il note la position. A chaque deplacement pendant
l'appui, il incremente un compteur et note si la souris est sortie de la
fenetre (position client negative ou superieure a la taille de la
fenetre). Au relachement, il affiche le bilan.

Le geste fait dans les trois tests

Chaque test suit le meme geste, fait a la main :

1. clic gauche au centre de la fenetre
2. maintien du bouton appuye
3. glissement vers la droite, en essayant de sortir de la fenetre
4. relachement du bouton une fois la souris sortie, ou au bord si elle
   ne peut pas sortir

Le geste est identique dans les trois tests. Ce qui change, c'est
uniquement ce que le programme fait avant la boucle.

Test 1 : aucune capture

Commande :

    NK_MODE=none jenga run TestGlisser --config Debug --platform x86_64 --target Linux

Sortie brute :

    [mode] none
    [mode] aucune capture
    [appui] client=(296,223) ecran=(942,550)
    [sortie] premier mouvement hors fenetre : client=(640,239)
    [relache] client=(1273,238) ecran=(1919,565)
    [bilan] deplacements=239, sortie de fenetre=oui
    [fin] mode=none, deplacements=239, sortie=oui

La souris sort de la fenetre au mouvement numero X (client 640, 239).
Puis elle continue a glisser dehors : la position au relachement est
(1273, 238), soit plus du double de la largeur de la fenetre.

Les evenements continuent d'arriver au programme pendant toute la
duree du glissement exterieur. La fenetre n'a plus la souris, mais le
programme recoit toujours les mouvements.

Ce comportement n'est pas celui de notre code. Nous n'avons rien demande.
C'est X11 qui, pendant qu'un bouton est enfonce, capture implicitement
le pointeur pour la fenetre qui a recu l'appui. Tant que le bouton n'est
pas relache, les mouvements arrivent a cette fenetre meme quand la
souris est ailleurs.

Test 2 : avec CaptureMouse

Commande :

    NK_MODE=capture jenga run TestGlisser --config Debug --platform x86_64 --target Linux

Sortie brute :

    [mode] capture
    [mode] CaptureMouse(true) appele
    [appui] client=(325,246) ecran=(971,573)
    [sortie] premier mouvement hors fenetre : client=(649,277)
    [relache] client=(1273,257) ecran=(1919,584)
    [bilan] deplacements=159, sortie de fenetre=oui
    [fin] mode=capture, deplacements=159, sortie=oui

Le resultat est identique au test 1 : la souris sort, les mouvements
continuent, le relachement arrive a la meme position.

C'est parce que NkWindow::CaptureMouse est vide sur Linux/X11. La ligne
1188 de NkXLibWindow.cpp :

    void NkWindow::CaptureMouse(bool) {
    }

Le parametre est meme commente (bool sans nom), ce qui signale qu'il
n'est pas utilise. L'appel ne fait rien. Activer ce mode revient donc a
lancer le programme sans rien faire, et on observe le comportement
implicite de X11 decrit dans le test 1.

Test 3 : avec ClipMouseToClient

Commande :

    NK_MODE=clip jenga run TestGlisser --config Debug --platform x86_64 --target Linux

Sortie brute :

    [mode] clip
    [mode] ClipMouseToClient(true) appele
    [appui] client=(322,226) ecran=(968,553)
    [relache] client=(639,225) ecran=(1285,552)
    [bilan] deplacements=176, sortie de fenetre=non
    [fin] mode=clip, deplacements=176, sortie=non

La ligne [sortie] n'apparait pas. Le curseur n'est jamais sorti de la
fenetre. La position au relachement est (639, 225). La fenetre fait 640
pixels de large, les coordonnees client vont de 0 a 639. 639 est la
derniere colonne.

Le pointeur s'est arrete exactement au bord droit. Il a essaye d'aller
plus loin pendant plusieurs secondes, mais X11 l'a retenu.

La fonction ClipMouseToClient est implementee a la ligne 1189 de
NkXLibWindow.cpp. Elle appelle XGrabPointer :

    XGrabPointer(mData.mDisplay, mData.mXid, True, ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
                 GrabModeAsync, GrabModeAsync,
                 mData.mXid, // confine_to = la fenetre elle-meme
                 None,       // cursor (None = default)
                 CurrentTime);

Le cinquieme parametre de XGrabPointer s'appelle confine_to. En passant
mData.mXid, on demande a X11 de confiner le pointeur dans le rectangle
de cette fenetre. C'est ce parametre qui fait toute la difference.

Difference du point de vue de l'utilisateur

Mode none ou capture

L'utilisateur clique dans la fenetre. Il maintient le bouton et tire la
souris vers la droite. Il sent que la souris continue de bouger comme
d'habitude. Le pointeur sort de la fenetre, traverse le bureau, peut
aller jusqu'au bord de l'ecran.

Le programme, lui, continue de recevoir les mouvements et sait a tout
moment ou se trouve le curseur par rapport au coin superieur gauche de
la fenetre. La coordonnee X peut depasser 640, devenir negative si la
souris revient vers la gauche apres avoir quitte la fenetre, etc.

Quand l'utilisateur relache le bouton, le programme recoit le
relachement et peut calculer la distance totale parcourue.

Ce comportement est utile pour un glisser-deposer vers une autre
fenetre, ou pour un geste de balayage large. Il est genant pour un jeu
qui veut garder le curseur dans son cadre.

Mode clip

L'utilisateur clique dans la fenetre. Il maintient le bouton et tire la
souris vers la droite. La souris s'arrete au bord de la fenetre. Elle
refuse d'aller plus loin, comme si un mur invisible bloquait le
pointeur. L'utilisateur peut continuer a tirer, la souris ne bouge
plus.

Le programme recoit les mouvements jusqu'au bord, mais plus aucun
mouvement hors fenetre. Il recoit toujours les coordonnees, mais elles
restent dans les limites 0 a 639 en X et 0 a 479 en Y.

Quand l'utilisateur relache le bouton, la souris est au bord. Elle
revient libre immediatement apres.

Ce comportement est utile pour les jeux qui font tourner la camera avec
la souris, ou pour eviter que le curseur disparaisse de la fenetre
pendant une action de glisser.

Tableau recapitulatif

| Mode | Fonction appelee | Sortie de fenetre | Position au relachement |
|------|------------------|-------------------|--------------------------|
| none | aucune | oui | (1273,238) |
| capture | CaptureMouse(true) | oui | (1273,257) |
| clip | ClipMouseToClient(true) | non | (639,225) |

Les modes none et capture donnent le meme resultat. La fonction
CaptureMouse n'est pas implementee sur Linux/X11. Le seul moyen de
retenir le pointeur dans la fenetre, sur cette plateforme, est
ClipMouseToClient.

Ce que cela montre

L'enonce demandait une difference decrite du point de vue de
l'utilisateur. Elle tient en une phrase : avec clip, la souris bute
contre le bord de la fenetre et ne sort plus ; sans clip, elle sort
librement en continuant d'alimenter le programme.

L'enonce demandait aussi de comparer une fois sans capture, une fois
avec. Le resultat est que CaptureMouse ne fait rien. Le vrai nom de la
fonction qui capture sur Linux/X11 est ClipMouseToClient. C'est elle qui
appelle XGrabPointer avec confine_to. C'est la ligne 1198 de
NkXLibWindow.cpp qui explique la difference.

Mesure faite le 27/09/2026.
Version de Jenga : 2.8.0.
