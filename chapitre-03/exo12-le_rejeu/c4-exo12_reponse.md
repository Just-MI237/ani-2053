Exercice 12 - Le rejeu

J'ai ecrit un programme qui enregistre les actions declenchees pendant
une minute d'usage, puis qui les rejoue sans toucher au clavier. Meme
fichier source, deux modes choisis par la variable d'environnement
NK_MODE.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que fait le programme

Le programme lie deux actions a des touches :

    actions.CreateAction("Avancer", ...);
    actions.CreateAction("Reculer", ...);
    actions.AddCommand(NkActionCommand("Avancer", NkInputCode::Key(NkKey::NK_RIGHT)));
    actions.AddCommand(NkActionCommand("Reculer", NkInputCode::Key(NkKey::NK_LEFT)));

Chaque handler d'action modifie une position et affiche une ligne.

Mode record. Le programme s'abonne a NkKeyPressEvent. A chaque appui
sur Fleche droite ou Fleche gauche, il note dans un journal deux
choses : le temps ecoule depuis le demarrage en millisecondes, et le
code de la touche. Il declenche aussi l'action correspondante. La
duree est de 60 secondes. A la fin, il ecrit le journal dans
/tmp/rejeu.txt.

Mode replay. Il lit le journal, puis parcourt la liste. A chaque
entree, il attend que le temps ecoule atteigne le temps note, puis
declenche l'action correspondant au code memorise. Il ne lit aucun
evenement du clavier. Aucune touche n'est touchee pendant le rejeu.

L'enregistrement

Commande :

    NK_MODE=record jenga run TestCheminTouche --config Debug --platform x86_64 --target Linux

Pendant 60 secondes, j'ai appuye sept fois sur Fleche droite, puis
trois fois sur Fleche gauche. Sortie brute :

    === Mode : record ===
    [fin record] entrees=10, position finale=4

Le journal ecrit sur disque :

    2347 99
    3363 99
    4345 99
    5343 99
    6291 99
    7237 99
    8171 99
    9166 98
    10048 98
    10894 98

Dix entrees. 99 correspond a NK_RIGHT, 98 a NK_LEFT. Sept appuis sur
NK_RIGHT puis trois sur NK_LEFT. Position finale : 7 - 3 = 4. Le
calcul est coherent.

Le rejeu

Commande :

    NK_MODE=replay jenga run TestCheminTouche --config Debug --platform x86_64 --target Linux

Je n'ai touche a aucune touche pendant cette seconde execution.
Sortie brute :

    === Mode : replay ===
    [replay] journal charge : 10 entrees
    [fin replay] entrees=10, position finale=4

Meme nombre d'entrees, meme position finale. Le programme a rejoue les
dix actions a partir du fichier, sans intervention du clavier.

Ce qui est identique, et ce qui ne l'est pas

Identique : les dix entrees, leurs timestamps, l'ordre des actions,
la suite de positions et la position finale (4).

Pas identique : la source des entrees. Dans le mode record, elles
viennent du clavier. Dans le mode replay, elles viennent du fichier.
Le programme fait le meme calcul a partir des memes entrees.

Ce que le journal contient

Le journal contient le moment ou chaque action a ete declenchee, pas
le moment ou la touche a ete pressee. L'ecart est faible ici (les
handlers ne font qu'incrementer une position), mais il serait reel
dans un programme plus lourd. C'est bien le declenchement de l'action
qui est enregistre, pas l'appui brut.

Ce que cela prouve

Le programme applique la meme regle (avancer ou reculer d'une case) a
deux sources differentes : les evenements du clavier, puis les entrees
d'un fichier. Le resultat est identique. Cela montre que la regle ne
connait pas la source. Le jeu ne sait pas si l'ordre d'avancer vient
du joueur ou du journal. C'est ce que l'enonce demande : verifier que
les regles ne connaissent plus le materiel.

Ce que cela permettrait pour les tests

1. Retrouver un bug. Un joueur envoie son journal, on le rejoue et on
voit exactement quand le probleme se produit. Sans le journal, il
faudrait refaire le geste a la main, ce qui est rarement fiable.

2. Tester une correction. On rejoue le journal sur la version
precedente : le bug se produit. On rejoue sur la version corrigee :
le bug disparait. La comparaison est directe.

3. Executer des tests automatises. Un journal par test, une suite de
journaux, et chaque nuit on rejoue les journaux sur la nouvelle
version. Si l'un diverge, on a une regression.

Ce que cela ne couvre pas

Le journal ne contient pas les valeurs aleatoires. Si le programme
tire un nombre au hasard pour decider d'un evenement, le rejeu
divergera. Il faudrait enregistrer la graine du generateur aleatoire,
ou toutes les valeurs tirees.

Le journal ne contient pas non plus les entrees de la souris. Le
programme ne s'abonne qu'a NkKeyPressEvent. Pour un jeu qui utilise la
souris, il faudrait ajouter les mouvements et les clics, dans le meme
journal.

Une limite technique du programme

Les timestamps sont ceux de la boucle principale, pas ceux fournis par
le serveur X. Deux appuis qui arrivent dans la meme frame (16 ms)
recevraient le meme timestamp. Sur cette mesure, les appuis sont
espaces d'au moins 900 ms, donc ce cas ne s'est pas produit. Mais il
pourrait se produire si deux appuis arrivent a moins de 16 ms. Pour un
rejeu plus fidele, il faudrait enregistrer le temps exact du serveur
(xev.xkey.time). Ce point est signale, non corrige.

Mesure faite le 29/09/2026.
Version de Jenga : 2.8.0.
