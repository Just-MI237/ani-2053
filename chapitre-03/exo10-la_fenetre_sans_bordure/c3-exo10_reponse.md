Exercice 10 - La fenetre sans bordure

J'ai ecrit un programme qui cree une fenetre sans bordure, dessine une
barre de titre a moi, et gere trois boutons, le deplacement a la souris
et le double-clic. Voici ce que j'ai obtenu.

Le programme est depose a cote sous le nom c3-exo10_main.cpp. Il fait
178 lignes.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que fait le programme

Le programme cree une fenetre en appelant SetDecorated(false). Le
gestionnaire de fenetres n'affiche plus sa barre de titre ni ses
bordures. La fenetre est entierement noire.

Pour dessiner la barre, j'ai cree un contexte graphique OpenGL et un
renderer 2D. A chaque image, le programme dessine :

- un rectangle gris en haut de la fenetre, sur 32 pixels de haut
- trois rectangles de 40 pixels de large a droite de la barre :
  gris, gris, rouge
- un symbole dans chaque bouton : trait pour Minimize, carre pour
  Maximize, croix pour Close
- le titre "Fenetre sans bordure" ecrit a gauche

Le texte passe par NkFont charge depuis
/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf.

Les trois boutons sont geres par le callback NkMouseButtonPressEvent.
Quand l'utilisateur clique dans la zone y de 0 a 31 pixels :

- entre 680 et 719 (sur une fenetre de 800), window.Minimize()
- entre 720 et 759, window.SetSize(1200, 800)
- entre 760 et 799, window.Close()
- ailleurs, window.BeginDragMove()

Ce que j'ai observe

Deplacement a la souris

Quand je clique dans la barre et que je glisse, la fenetre se deplace.
Le log confirme chaque glissement :

    [clic] barre de titre (385,17), drag
    [clic] barre de titre (415,18), drag

Le BeginDragMove fait ce qu'il doit faire : la fenetre suit la souris
pendant le glissement.

Clic sur les boutons

Les trois boutons fonctionnent, mais seulement si je clique dans la
bonne zone. Sur une fenetre de 800x600 au demarrage :

    [clic] Minimize    (client 701,19)
    [clic] Close       (client 771,4)

L'image que j'ai capturee montre la barre grise, le titre en blanc, et
les trois boutons a droite, avec leurs symboles.

Double-clic dans la barre

Le double-clic ne fonctionne pas. J'ai cliqué deux fois de suite dans
la barre, en essayant plusieurs zones. Le log reste toujours a
clickCount=1.

    [press] client=(385,17) ecran=(951,284) clickCount=1
    [press] client=(415,18) ecran=(981,285) clickCount=1
    [press] client=(416,19) ecran=(1024,311) clickCount=1

J'ai enregistre un callback sur NkMouseDoubleClickEvent. Il n'est
jamais appele. Le backend X11 ne genere pas cet evenement.

Consequences

Le double-clic ne maximise pas. J'ai donc ajoute un deuxieme chemin :
cliquer sur le bouton Maximize de la barre. Ce chemin fonctionne.

Maximize natif sur WSLg

J'ai d'abord utilise window.Maximize() pour le bouton du milieu. Le
resultat n'est pas ce qu'on attend : la fenetre devient plus grande que
l'ecran, et les trois boutons sortent completement du cadre. Impossible
de cliquer pour restaurer.

J'ai remplace par un maximize borne :

    window.SetSize(1200, 800);

Apres SetSize, la taille reelle est bien 1200x800, comme le log le
montre :

    [clic] Maximize borne
    [taille] 1200x800

Les trois boutons sont maintenant a x = 1080 a 1199. Ils sont visibles
et cliquables.

Tableau recapitulatif

| Element | Fonctionne | Commentaire |
|---------|------------|-------------|
| Fenetre sans bordure | oui | SetDecorated(false) |
| Barre dessinee | oui | NkRenderer2D, DrawFilledRect |
| Titre ecrit | oui | NkFont + NkText |
| Trois boutons | oui | zones de 40 px a droite |
| Deplacement souris | oui | BeginDragMove |
| Double-clic | non | NkMouseDoubleClickEvent jamais recu |
| Minimize | oui | window.Minimize() |
| Maximize natif | non | deborde l'ecran sur WSLg |
| Maximize borne | oui | window.SetSize(1200, 800) |
| Close | oui | window.Close() |

Ce que cela montre

Le backend X11 ne detecte pas le double-clic. NkMouseDoubleClickEvent
existe dans NkEvent.h (ligne 684), mais rien ne le produit cote
backend. Le champ GetClickCount() reste a 1 apres deux appuis
consecutifs. C'est la cinquieme fonction de l'interface qui ne fait
rien sur une plateforme donnee, apres SetCursor, CaptureMouse et le
DPI.

Le Maximize natif fonctionne differemment. Il appelle le WM. Sur WSLg,
le WM n'a pas de notion de zone de travail : il redimensionne la
fenetre a une taille arbitraire, sans respecter les limites de l'ecran.
Le fait que les boutons disparaissent ensuite est une consequence : ils
sont dessines par rapport a la largeur de la fenetre, mais si la
fenetre est plus grande que l'ecran, ils sont hors champ.

Combien de temps cela m'a pris

Environ deux heures. Mesure par l'horloge : l'exercice precedent a ete
depose a 00h38, celui-ci a 03h13.

La partie visible du programme, la barre et les boutons, prend environ
50 lignes. Le reste, c'est la mise en place : creer un contexte
graphique, charger une police, decouvrir la chaine de dependances.

Trois quarts du temps sont passes sur des choses qui n'ont rien a voir
avec l'enonce :

- chercher quelle chaine de modules ajouter dans le .jenga (NKCanvas
  tire NKImage, NKStream, NKFont, et il faut les declarer un par un)
- trouver ou est declare NkText (dans NkSprite.h, pas dans un fichier
  a part)
- comprendre que Maximize natif deborde sur WSLg et le remplacer par un
  SetSize

Le quart restant est la barre et les boutons eux-memes.

Ce que ca dit sur l'ecosysteme

Dans beaucoup de frameworks, une fenetre sans bordure et une barre de
titre custom demandent 20 lignes. Ici, il faut monter un contexte
graphique, tirer un renderer, charger une police. C'est le prix a
payer pour ne pas dependre d'une bibliotheque UI externe.

Mesure faite le 28/09/2026.
Version de Jenga : 2.8.0.
