Exercice 12 - L'inventaire des ecrans

J'ai ecrit un programme qui affiche, pour chaque ecran branche, sa
taille, sa position, son facteur d'echelle, et lequel porte la fenetre.
Voici ce que j'ai mesure.

Le programme est depose a cote sous le nom c3-exo12_main.cpp. Il fait
81 lignes.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que fait le programme

Le programme cree une fenetre, puis appelle window.EnumerateMonitors()
pour obtenir la liste des ecrans. Pour chacun, il affiche son nom, sa
taille logique, sa taille physique, sa position, son facteur d'echelle
dpiScale, ses dpiX / dpiY, son taux de rafraichissement, et s'il est
primaire.

Il appelle ensuite window.GetCurrentMonitor() pour savoir quel ecran
porte la fenetre.

Dans la boucle principale, il verifie a chaque frame si l'ecran porteur
a change. S'il a change, il affiche la nouvelle valeur et la position
de la fenetre.

La structure NkDisplayInfo

Les champs sont declares dans NkSystemEvent.h, ligne 158. J'ai lu la
structure avant d'ecrire le programme :

    uint32 index
    uint32 width          (logique)
    uint32 height         (logique)
    uint32 physWidth
    uint32 physHeight
    uint32 refreshRate
    float32 dpiScale
    float32 dpiX
    float32 dpiY
    int32 posX
    int32 posY
    bool isPrimary
    char name[64]

Resultat sur WSLg (:0)

Sortie brute du programme :

    Nombre d'ecrans : 1
    Ecran 0 :
      nom          = "XWAYLAND0"
      taille       = 1920 x 1080 (logique)
      taille phy   = 1920 x 1080
      position     = (0, 0)
      dpiScale     = 1.000000
      dpiX/dpiY    = 96.000000 / 96.252632
      refresh      = 60 Hz
      primaire     = 0

    Ecran portant la fenetre :
      nom          = "XWAYLAND0"
      pos=(0,0) dpiScale=1.000000

WSLg declare un seul ecran, nomme XWAYLAND0, de 1920x1080, avec un
facteur d'echelle de 1.0.

Resultat sur Xvfb (:98)

J'ai lance Xvfb avec une taille d'ecran de 1920x1080. Sortie brute :

    Nombre d'ecrans : 1
    Ecran 0 :
      nom          = "screen"
      taille       = 1920 x 1080 (logique)
      taille phy   = 1920 x 1080
      position     = (0, 0)
      dpiScale     = 1.040984
      primaire     = 0

Le nom de l'ecran est "screen". Le facteur d'echelle est 1.040984, pas
1.0. La difference vient du fait que Xvfb, quand on ne lui donne pas de
-dpi, utilise un DPI par defaut qui n'est pas 96. Sur WSLg, le DPI
declare est exactement 96, donc le facteur tombe a 1.0.

Cela recoupe l'exercice 4 : le facteur d'echelle depend entierement de
ce que le serveur X declare, et WSLg declare 96 dpi.

Le passage d'un ecran a l'autre

L'enonce demande de deplacer la fenetre d'un ecran a l'autre et de
verifier que les valeurs suivent. Je n'ai pas pu faire ce test.

Sur WSLg, il n'y a qu'un seul ecran. Deplacer la fenetre dans son
rectangle ne change rien : l'ecran porteur reste le meme.

Sur Xvfb, xrandr annonce RandR 1.6 et accepte --setmonitor sur le
papier, mais la commande ne cree pas de second moniteur. Elle affiche
des erreurs de syntaxe ("output list screen", "add monitor screen") et
xrandr --listmonitors continue de retourner un seul ecran. J'ai essaye
plusieurs variantes du nom et du rectangle, sans succes.

Pour vraiment tester le multi-ecran, il faudrait soit une machine
physique avec deux ecrans, soit un serveur X configure en Xinerama.
Aucun des deux n'est disponible dans mon environnement.

Ce que j'ai pu verifier quand meme

Le programme appelle bien window.GetCurrentMonitor() a chaque frame. Le
log le montre : au demarrage, la position initiale de la fenetre
declenche un premier changement, et la ligne suivante apparait :

    [changement d'ecran] fenetre=(710,340), nouvel ecran : nom="screen" pos=(0,0) dpiScale=1.040984

Ce que cela dit de la logique du programme

Si l'ecran porteur changeait, le programme le verrait et l'afficherait.
GetCurrentMonitor() est appele a chaque frame, et la comparaison sur les
champs posX/posY de NkDisplayInfo declencherait l'affichage.

Je n'ai pas de moyen de verifier cette branche dans mon environnement.
C'est une limite a signaler honnetement plutot qu'a masquer.

Ce que cela montre

EnumerateMonitors() fonctionne et retourne les bonnes valeurs. La
structure NkDisplayInfo porte bien tous les champs demandes par
l'enonce : taille, position, dpiScale, primaire.

Le facteur d'echelle depend entierement du serveur X. WSLg declare
96 dpi, donc 1.0. Xvfb par defaut declare un DPI different, donc
1.040984. Cela confirme la conclusion de l'exercice 4 : le facteur
d'echelle n'est pas une propriete de la fenetre, c'est une propriete du
serveur.

Mesure faite le 28/09/2026.
Version de Jenga : 2.8.0.
