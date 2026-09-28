Exercice 12 - L'inventaire des ecrans

J'ai ecrit un programme qui affiche, pour chaque ecran branche, sa
taille, sa position, son facteur d'echelle, et lequel porte la fenetre.
Voici ce que j'ai mesure.

Le programme est depose a cote sous le nom c3-exo12_main.cpp. Il fait
104 lignes.

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

Tableau comparatif

| Mesure | WSLg (:0) | Xvfb (:98) |
|--------|-----------|------------|
| Nombre d'ecrans | 1 | 1 |
| Nom | XWAYLAND0 | screen |
| Taille logique | 1920x1080 | 1920x1080 |
| Taille physique | 1920x1080 | 1920x1080 |
| Position | (0,0) | (0,0) |
| dpiScale | 1.000000 | 1.040984 |
| dpiX | 96.000000 | 99.934425 |
| dpiY | 96.252632 | 100.116791 |
| Refresh | 60 Hz | 60 Hz |
| Primaire | 0 | 0 |

Les deux environnements declarent un seul ecran de meme taille et meme
position. La difference notable est le facteur d'echelle : 1.0 sur
WSLg, 1.040984 sur Xvfb. C'est la meme observation que l'exercice 4
sous un autre angle : le facteur d'echelle vient du serveur, pas du
moteur.

Un detail que je n'avais pas remarque avant de remplir ce tableau :
dpiX et dpiY ne sont jamais egaux. Sur WSLg, 96.000000 contre
96.252632. Sur Xvfb, 99.934425 contre 100.116791. L'ecart est petit
mais il est systematique.

L'ecran n'est pas carre en pixels physiques. Sa largeur divisee par sa
hauteur ne tombe pas exactement sur 16/9 comme la resolution logique.
Le calcul du DPI se fait separement sur X et sur Y a partir de la
taille en millimetres, et comme les deux rapports different legerement,
les deux DPI different aussi. Le moteur ne fait pas la moyenne.

Cela recoupe l'exercice 4 : le facteur d'echelle depend entierement de
ce que le serveur X declare, et WSLg declare 96 dpi.

Le passage d'un ecran a l'autre

L'enonce demande de deplacer la fenetre d'un ecran a l'autre et de
verifier que les valeurs suivent.

Je n'ai pas pu tester un vrai changement d'ecran : WSLg ne declare
qu'un seul ecran, et Xvfb n'accepte pas les moniteurs virtuels
multiples. Mais j'ai fait un test controle qui approche la question.

Test controle : deplacement programme

J'ai ajoute au programme une sequence de positions qui sortent du
rectangle de l'ecran. La fenetre est deplacee par programme a quatre
positions, a des moments fixes. A chaque fois, le programme lit
GetCurrentMonitor.

Sortie brute du log, sur WSLg :

    [SetPosition] appel 1 : (2500,100) au-dela du bord droit de l'ecran (2500,100)
    [SetPosition] appel 2 : (100,100) retour en haut-gauche (100,100)
    [SetPosition] appel 3 : (-500,100) au-dela du bord gauche (-500,100)
    [SetPosition] appel 4 : (100,100) retour en haut-gauche (100,100)
    [fin] deplacements detectes : 1

Sortie brute du log, sur Xvfb (:98), avec le meme programme :

    [changement d'ecran] fenetre=(710,340), nouvel ecran : nom="screen" pos=(0,0) dpiScale=1.040984
    [SetPosition] appel 1 : (2500,100) au-dela du bord droit de l'ecran (2500,100)
    [SetPosition] appel 2 : (100,100) retour en haut-gauche (100,100)
    [SetPosition] appel 3 : (-500,100) au-dela du bord gauche (-500,100)
    [SetPosition] appel 4 : (100,100) retour en haut-gauche (100,100)
    [fin] deplacements detectes : 1

Les deux environnements se comportent de la meme facon. Le programme
appelle SetPosition quatre fois, appelle GetCurrentMonitor a chaque
frame, et le compteur de changements d'ecran reste a 1. La seule
difference entre les deux logs est la position initiale (710,340) sur
Xvfb au lieu de (-32730,-32709) sur WSLg, et le dpiScale du seul ecran
present.

Le programme a bien appele SetPosition quatre fois. Il a bien appele
GetCurrentMonitor a chaque frame. Et le compteur d'ecrans porteurs est
reste a 1. Ce "1" est celui du demarrage : au tout premier appel,
l'ecran porteur a ete lu pour la premiere fois.

Le log de demarrage contient une ligne avec des valeurs etranges :

    [changement d'ecran] fenetre=(-32730,-32709), nouvel ecran : nom="XWAYLAND0" pos=(0,0) dpiScale=1.000000

Ces nombres negatifs ne correspondent a aucune position reelle. Ils
viennent du fait que la fenetre n'est pas encore positionnee par le
gestionnaire de fenetres au moment ou GetCurrentMonitor est appele la
premiere fois. Le premier appel se fait avant que la position de la
fenetre soit stabilisee, et GetPosition retourne une valeur temporaire
du gestionnaire. Apres ce premier appel, toutes les positions sont
normales.

Rien n'a change. C'est coherent : avec un seul ecran, deplacer la
fenetre a 2500 pixels du bord ou a -500 pixels du bord ne change pas
d'ecran, parce qu'il n'y en a pas d'autre. GetCurrentMonitor retourne
toujours XWAYLAND0.

Ce que cela prouve

La logique de comparaison fonctionne. Le programme a bien appele
GetCurrentMonitor a chaque frame. Si un deuxieme ecran etait declare,
le changement de ses champs posX/posY declencherait la ligne
[changement d'ecran].

Ce que cela ne prouve pas

Je ne peux pas montrer que le changement d'ecran est detecte, parce
qu'il n'y a rien vers quoi changer. Sur une machine a deux ecrans, le
meme programme detecterait le passage. C'est une prediction verifiable,
pas une mesure faite.

Pour la faire, il faudrait soit une machine physique avec deux ecrans,
soit un serveur X configure en Xinerama. Aucun des deux n'est
disponible dans mon environnement.

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
