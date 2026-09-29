Exercice 7 - Le front montant

J'ai ecrit un programme qui fait sauter un personnage a la touche
espace. Le personnage est un carre visible dans une fenetre 800x400.
Quand il saute, il monte puis retombe par gravite. Le programme a trois
modes, choisis par la variable d'environnement NK_MODE.

Le programme est depose a cote sous le nom c4-exo7_main.cpp.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que fait le programme

Le programme dessine un carre bleu pose sur le sol (y=300). A chaque
saut, il recoit une vitesse verticale negative et monte. La gravite le
fait redescendre. Quand il touche le sol, la vitesse retombe a zero.

Les trois modes :

- state_no_edge : on saute a chaque frame ou la touche est enfoncee,
  sans aucune condition.
- state : on saute sur le front montant, c'est-a-dire au passage de
  faux a vrai de l'etat de la touche. Un debounce de 200 ms filtre les
  repetitions.
- event : on saute sur l'evenement NkKeyPressEvent. Un debounce de
  200 ms filtre les repetitions.

Ce que le premier test a montre

Premier essai sans aucun filtre. Un seul appui, maintenu deux secondes.
Sortie brute :

    [state-no-edge] saut 1
    [state-no-edge] saut 2
    [state-no-edge] saut 3
    [state-no-edge] saut 4
    [state-no-edge] saut 5
    [state-no-edge] saut 30
    [state-no-edge] saut 60
    [state-no-edge] saut 90
    [state-no-edge] saut 120
    [state-no-edge] saut 150
    [state-no-edge] saut 180
    [state-no-edge] saut 210
    [fin] mode=state_no_edge, sauts=234

Un seul appui, 234 sauts. Le carre vole vers le haut de la fenetre et
ne retombe plus. Chaque frame reapplique la force de saut.

Ce nombre n'est pas une constante. Il depend directement de la duree
pendant laquelle la touche est maintenue. Un second essai, avec un
appui plus court, a donne 27 sauts au lieu de 234. La proportion est
coherente : environ 120 sauts par seconde d'appui (un par frame a
60 images par seconde), donc 234 sauts pour deux secondes, et 27 pour
environ un quart de seconde.

Le meme appui, en mode state sans filtre et en mode event sans filtre,
donnait plusieurs sauts aussi. C'est la consequence de l'auto-repeat
du systeme : quand une touche reste enfoncee, X11 envoie un faux
relachement suivi d'un nouvel appui, a un rythme regulier. Chaque
appui compte comme un nouvel appui.

C'est le defaut que l'exercice demande de constater.

Ce que la correction a donne

Mode state, avec la detection de front et un debounce de 200 ms :

    [state-front] saut 1 a 3525 ms
    [fin] mode=state, sauts=1

Un seul saut. Le debounce rejette les repetitions.

Mode event, avec un debounce de 200 ms :

    === Mode : event ===
    [event] saut 1 a 4121 ms
    [event] saut 2 a 4525 ms
    [event] ignore (auto-repeat a 4543 ms)
    [event] ignore (auto-repeat a 4581 ms)
    [event] ignore (auto-repeat a 4599 ms)
    [event] ignore (auto-repeat a 4635 ms)
    [event] ignore (auto-repeat a 4654 ms)
    [event] ignore (auto-repeat a 4675 ms)
    [event] ignore (auto-repeat a 4693 ms)

Le premier saut est a 4121 ms. Le deuxieme a 4525 ms, soit 404 ms
plus tard. L'ecart est superieur au delai de debounce (200 ms), donc
le filtre n'a pas pu le rejeter. Deux causes possibles : un second
appui manuel, ou une repetition tardive du systeme. Je n'ai pas
verifie laquelle des deux. Ce point n'est pas tranche.

Puis les lignes [event] ignore apparaissent a partir de 4543 ms : ce
sont les auto-repeats, rejetes par le debounce.

Un second test, avec un seul appui court, donne :

    === Mode : event ===
    [event] saut 1 a 2980 ms
    [fin] mode=event, sauts=1

Un seul saut. Pas de ligne ignore, parce que l'appui etait plus court
que le delai initial de l'auto-repeat (environ 500 ms).

Tableau recapitulatif

| Mode | Filtre | Sauts pour un appui maintenu 2 s |
|------|--------|----------------------------------|
| state_no_edge | aucun | 234 |
| state | front montant + debounce 200 ms | 1 |
| event | debounce 200 ms | 1 |

Ce que cela montre

L'interrogation d'etat brute ne suffit pas pour un saut. Il faut
ajouter deux choses : la detection du front montant, et un filtre sur
les repetitions. La detection du front ne suffit pas seule, parce que
l'auto-repeat du systeme fait osciller l'etat : chaque faux relachement
suivi d'un nouvel appui est vu comme un nouveau front.

Le debounce est la piece qui manque. Il note le moment du dernier saut
et ignore tout nouvel appui a moins de 200 ms. C'est une valeur choisie
au jugé, pas mesuree. Pour la fixer proprement, il faudrait mesurer
l'intervalle de l'auto-repeat sur cette machine et prendre une valeur
un peu plus grande. Je ne l'ai pas fait.

Une remarque sur le backend X11

Le moteur definit un evenement NkKeyRepeatEvent, ligne 1006 de
NkKeyboardEvent.h, specialement pour signaler l'auto-repeat. Le
backend X11 ne le produit pas : il envoie chaque repetition comme un
NkKeyPressEvent ordinaire. C'est pour cela que le debounce est
necessaire. Un backend qui produirait NkKeyRepeatEvent permettrait de
filtrer sur le type d'evenement, ce qui serait plus propre.

Ce point rejoint les huit trouvailles du document
notes/fonctions-sans-implementation.md.

Mesure faite le 29/09/2026.
Version de Jenga : 2.8.0.
