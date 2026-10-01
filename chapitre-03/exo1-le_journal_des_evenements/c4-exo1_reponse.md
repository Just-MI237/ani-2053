Exercice 1 - Le journal des evenements

J'ai ecrit un programme qui affiche chaque evenement recu, avec sa
famille et son type, et qui compte le total. J'ai ensuite fait une
minute d'usage normal : bouger la souris, taper quelques touches,
redimensionner la fenetre, puis rester immobile.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que fait le programme

Le programme ouvre une fenetre et vide la file d'evenements a chaque
frame avec PollEvent. Contrairement a PollEvents, qui distribue les
evenements aux callbacks enregistres, PollEvent donne l'evenement un a
un. Cela permet de voir tous les types, pas seulement ceux auxquels on
s'abonne.

A chaque evenement, il affiche deux choses :

- la famille, lue avec GetCategory() et convertie avec
  NkEventCategory::ToString,
- le type precis, lu avec GetTypeStr().

Exemple de ligne :

    [event] INPUT|MOUSE / NK_MOUSE_MOVE

La famille est INPUT|MOUSE (deux flags combines), le type est
NK_MOUSE_MOVE.

Le resultat global

Sortie brute :

    [fin] total=1658

1658 evenements pour 60 secondes d'usage normal. Cela fait environ 27
evenements par seconde en moyenne.

Repartition par type

Commande :

    grep "\[event\]" /tmp/journal3.txt | sort | uniq -c | sort -rn | head -15

Sortie brute :

    675 [event] INPUT|MOUSE / NK_MOUSE_MOVE
    149 [event] WINDOW / NK_WINDOW_RESIZE_END
    149 [event] WINDOW / NK_WINDOW_RESIZE_BEGIN
    149 [event] WINDOW / NK_WINDOW_RESIZE
    149 [event] WINDOW / NK_WINDOW_MOVE_END
    149 [event] WINDOW / NK_WINDOW_MOVE_BEGIN
    149 [event] WINDOW / NK_WINDOW_MOVE
     62 [event] WINDOW / NK_WINDOW_PAINT
      7 [event] INPUT|MOUSE / NK_MOUSE_ENTER
      6 [event] INPUT|MOUSE / NK_MOUSE_LEAVE
      4 [event] INPUT|KEYBOARD / NK_TEXT_INPUT
      4 [event] INPUT|KEYBOARD / NK_KEY_RELEASED
      4 [event] INPUT|KEYBOARD / NK_KEY_PRESSED
      1 [event] WINDOW / NK_WINDOW_SHOWN
      1 [event] WINDOW / NK_WINDOW_FOCUS_GAINED

Repartition par famille :

- WINDOW : 958 evenements, soit 58 pour cent
- INPUT|MOUSE : 688 evenements, soit 41 pour cent
- INPUT|KEYBOARD : 12 evenements, soit moins de 1 pour cent

Repartition par type :

- souris (deplacement) : 675, soit 41 pour cent
- fenetre (resize et deplacement) : 894, soit 54 pour cent
- fenetre (peinture) : 62
- clavier (appui, relachement, texte) : 12
- souris (entree, sortie) : 13
- cycle de vie (affichee, focus) : 2

Ce que cela montre

La souris et la fenetre produisent plus de 99 pour cent des
evenements. Le clavier en produit moins de 1 pour cent. Un programme
qui traite tous les evenements de la meme facon passe presque tout son
temps sur deux sources qui ne sont pas le jeu lui-meme.

Le detail des resize est instructif. Le programme n'a fait qu'un seul
redimensionnement, mais le journal compte 149 NK_WINDOW_RESIZE, plus
149 NK_WINDOW_RESIZE_BEGIN et 149 NK_WINDOW_RESIZE_END. Soit 447
evenements pour un seul geste. Le gestionnaire de fenetres envoie un
evenement a chaque pixel de deplacement de la souris pendant le drag,
et encadre la serie par un BEGIN et un END.

Meme chose pour les deplacements : 447 evenements pour un seul
deplacement de fenetre.

Le resultat par seconde

Sortie brute pour les dernieres secondes :

    [seconde] 0 evenements
    [seconde] 0 evenements
    [seconde] 0 evenements
    [seconde] 0 evenements
    [seconde] 0 evenements

Les cinq dernieres secondes n'ont produit aucun evenement, parce que
l'usage etait immobile. Les 1658 evenements sont concentres dans les
premieres secondes, pendant les gestes. C'est pour cela que la moyenne
de 27 par seconde est trompeuse : le rythme reel va de plusieurs
centaines par seconde pendant un drag a zero quand on ne touche a rien.

Le depose de fichier : essai et resultat

L'enonce demande aussi de « deposer un fichier » pour voir apparaitre
les evenements de la famille DROP. J'ai essaye.

J'ai glisse un fichier depuis l'explorateur Windows vers la fenetre du
programme, qui tourne sous WSLg. Aucun evenement NK_DROP n'est
apparu. Le journal de cette session montre seulement du bruit :
mouvements de souris, entrees et sorties de la fenetre, changements de
focus.

Les mouvements de la souris pendant le glissement sont bien arrives.
Mais a l'instant du relachement, aucun evenement DROP n'a ete produit.

Ce n'est pas un defaut du programme. WSLg ne transmet pas le
glisser-deposer de Windows vers les applications X11. Il faudrait un
fichier glisse depuis une autre application Linux pour que le
programme recoive un NK_DROP. Je n'ai pas pu le tester, je le signale
comme limite de la plateforme.

Ce que cela dit pour un programme

Un programme qui s'abonne a tout et qui fait un traitement lourd sur
chaque evenement va peiner pendant les gestes de la souris ou de la
fenetre. Le conseil classique est de separer deux categories : les
evenements qui se traitent un a un (appui de touche, clic, fermeture),
et ceux qui se lisent une fois par frame (position de la souris,
taille de la fenetre). Le premier n'a pas besoin de toutes les
positions intermediaires. Le second n'a besoin que de la derniere.

C'est exactement la distinction entre l'evenement et l'etat interroge,
vue dans l'exercice 7.

Le programme est depose a cote sous le nom c4-exo1_main.cpp.

Mesure faite le 01/10/2026.
Version de Jenga : 2.8.0.
