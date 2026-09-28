Demonstration 4 - Le jeu qui se joue tout seul

J'ai ecrit un programme qui enregistre les entrees pendant une partie,
puis qui les rejoue a l'identique. Meme fichier source, deux modes
choisis par la variable d'environnement NK_MODE.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que fait le programme

Le programme a deux modes.

Mode record. Il s'abonne a NkKeyPressEvent. A chaque appui, il note
dans un journal deux choses : le temps ecoule depuis le demarrage en
millisecondes, et le code de la touche. Il simule aussi une position
qui avance sur fleche droite et recule sur fleche gauche, pour avoir
un etat observable. A la fin des 15 secondes, il ecrit le journal
dans /tmp/rejeu.txt.

Mode replay. Il lit le journal, puis parcourt la liste. A chaque
entree, il attend que le temps ecoule atteigne le temps note, puis
applique la meme touche a la meme position. Il ne lit aucun evenement
du clavier : c'est le journal qui pilote tout.

Ce que l'enregistrement a produit

Commande :

    NK_MODE=record jenga run TestCheminTouche --config Debug --platform x86_64 --target Linux

Pendant 15 secondes, j'ai appuye sur Fleche droite trois fois, puis
Fleche gauche une fois. Sortie brute :

    === Mode : record ===
    [record] t=4849 ms, touche=99, position=1
    [record] t=5371 ms, touche=99, position=2
    [record] t=5715 ms, touche=99, position=3
    [record] t=7177 ms, touche=98, position=2
    [fin record] entrees=4, position finale=2

Le journal ecrit sur disque :

    4849 99
    5371 99
    5715 99
    7177 98

Quatre entrees. Chaque ligne contient un temps et un code de touche.
99 correspond a NK_RIGHT, 98 a NK_LEFT.

Ce que le rejeu a produit

Commande :

    NK_MODE=replay jenga run TestCheminTouche --config Debug --platform x86_64 --target Linux

Sortie brute :

    === Mode : replay ===
    [replay] journal charge : 4 entrees
    [replay] t=4849 ms, touche=99, position=1
    [replay] t=5371 ms, touche=99, position=2
    [replay] t=5715 ms, touche=99, position=3
    [replay] t=7177 ms, touche=98, position=2
    [fin replay] entrees=4, position finale=2

Meme nombre d'entrees, memes temps, memes touches, meme position
finale. Les deux sorties sont identiques ligne pour ligne.

Ce qui est identique, et ce qui ne l'est pas

Identique : les quatres entrees et leurs temps, dans le meme ordre.
La suite de positions (1, 2, 3, 2). La position finale (2).

Pas identique : dans le mode record, l'etat vient du clavier, personne
n'a programme les appuis. Dans le mode replay, l'etat vient du
fichier, personne n'y touche. Ce qui change entre les deux, c'est la
source des entrees. Ce qui reste identique, c'est ce que le programme
en fait.

Le point cle : le journal contient les entrees brutes, pas les
positions. Le programme qui rejoue refait le meme calcul. C'est ce
qui permet de verifier une regression : si le calcul change entre
deux versions du programme, le rejeu divergera.

Ce que cela permettrait pour les tests

Un journal d'entrees et un programme qui le rejoue donnent une partie
reproductible. Trois usages.

1. Retrouver un bug. Un joueur signale qu'a un moment precis, son
personnage traverse un mur. S'il envoie le journal, on le rejoue et on
voit exactement quand ca se produit. Sans le journal, il faudrait
essayer de refaire le geste, ce qui est rarement fiable.

2. Tester une correction. On rejoue le journal sur la version
precedente : le bug se produit. On rejoue sur la version corrigee :
le bug disparait. La comparaison est directe.

3. Executer des tests automatises. Un journal par test, une suite de
journaux, et chaque nuit on rejoue les journaux sur la nouvelle
version. Si l'un diverge, on a une regression.

Ce que cela ne couvre pas

Le journal ne contient que les entrees. Il ne contient pas les
valeurs aleatoires. Si le programme tire un nombre au hasard pour
decider d'un evenement, le rejeu divergera, parce que le tirage ne
sera pas le meme. Pour rendre une partie entierement reproductible,
il faut aussi enregistrer la graine du generateur aleatoire, ou toutes
les valeurs tirees. C'est un point que je n'ai pas traite ici.

Le journal ne contient pas non plus les entrees de la souris. Le
programme ne s'abonne qu'a NkKeyPressEvent. Pour un jeu qui utilise la
souris, il faudrait ajouter les mouvements et les clics, dans le meme
journal.

Mesure faite le 28/09/2026.
Version de Jenga : 2.8.0.
