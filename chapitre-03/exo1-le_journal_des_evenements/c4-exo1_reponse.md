Exercice 1 - Le journal des evenements

J'ai ecrit un programme qui affiche chaque evenement recu, avec son
type, et qui compte le total par seconde. J'ai ensuite fait une minute
d'usage normal : bouger la souris, taper quelques touches, redimensionner
la fenetre, puis rester immobile.

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

A chaque evenement, il affiche le nom du type, avec GetTypeStr(). Il
compte aussi le total par seconde pour voir la densite.

Le resultat global

Sortie brute :

    [fin] total=1888

1888 evenements pour 60 secondes d'usage normal. Cela fait environ 31
evenements par seconde en moyenne.

Repartition par type

Commande :

    grep "\[event\]" /tmp/journal.txt | sort | uniq -c | sort -rn | head -15

Sortie brute :

    878 [event] NK_MOUSE_MOVE
    146 [event] NK_WINDOW_RESIZE_END
    146 [event] NK_WINDOW_RESIZE_BEGIN
    146 [event] NK_WINDOW_RESIZE
    146 [event] NK_WINDOW_MOVE_END
    146 [event] NK_WINDOW_MOVE_BEGIN
    146 [event] NK_WINDOW_MOVE
    109 [event] NK_WINDOW_PAINT
      6 [event] NK_MOUSE_ENTER
      5 [event] NK_MOUSE_LEAVE
      4 [event] NK_TEXT_INPUT
      4 [event] NK_KEY_RELEASED
      4 [event] NK_KEY_PRESSED
      1 [event] NK_WINDOW_SHOWN
      1 [event] NK_WINDOW_FOCUS_GAINED

Total par type :

- souris (deplacement) : 878, soit 46 pour cent
- fenetre (resize et deplacement) : 876, soit 46 pour cent
- fenetre (peinture) : 109
- clavier (appui, relachement, texte) : 12
- souris (entree, sortie) : 11
- cycle de vie (affichee, focus) : 2

Ce que cela montre

La souris et la fenetre produisent 93 pour cent des evenements. Le
clavier en produit moins de 1 pour cent. Un programme qui traite tous
les evenements de la meme facon passe presque tout son temps sur deux
sources qui ne sont pas le jeu lui-meme.

Le detail des resize est instructif. Le programme n'a fait qu'un seul
redimensionnement, mais le journal compte 146 NK_WINDOW_RESIZE, plus
146 NK_WINDOW_RESIZE_BEGIN et 146 NK_WINDOW_RESIZE_END. Soit 438
evenements pour un seul geste. Le gestionnaire de fenetres envoie un
evenement a chaque pixel de deplacement de la souris pendant le drag,
et encadre la serie par un BEGIN et un END.

Meme chose pour les deplacements : 438 evenements pour un seul
deplacement de fenetre.

Le resultat par seconde

Sortie brute pour les dernieres secondes :

    [seconde] 0 evenements
    [seconde] 0 evenements
    [seconde] 0 evenements
    [seconde] 0 evenements
    [seconde] 0 evenements

Les cinq dernieres secondes n'ont produit aucun evenement, parce que
l'usage etait immobile. Les 1888 evenements sont concentres dans les
premieres secondes, pendant les gestes. C'est pour cela que la moyenne
de 31 par seconde est trompeuse : le rythme reel va de plusieurs
centaines par seconde pendant un drag a zero quand on ne touche a rien.

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
