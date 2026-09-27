Exercice 5 - Le titre qui informe

J'ai ecrit un programme qui affiche dans le titre de la fenetre trois
informations : le nom du document, un asterisque quand il est modifie,
et la taille courante de la fenetre. Le titre est mis a jour seulement
quand quelque chose change, pas a chaque image.

Le programme est depose a cote sous le nom c3-exo5_main.cpp. Il fait
54 lignes.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que fait le programme

Le programme cree une fenetre dont le titre initial est "document.txt".
Il enregistre deux callbacks sur le gestionnaire d'evenements :

- un callback pour NkWindowResizeEvent, qui met a jour le titre quand
  la fenetre change de taille
- un callback pour NkKeyPressEvent, qui bascule l'etat modifie quand
  on appuie sur la barre espace, puis met a jour le titre

La fonction updateTitle construit le titre en concatenant trois
elements : le nom du document, un asterisque si modifie, et la taille
courante de la fenetre lue par window.GetSize().

La boucle principale ne touche pas au titre. Elle se contente de
vider la file d'evenements :

    while (window.IsOpen()) {
        events.PollEvents();
    }

C'est la bonne maniere de faire. Le titre n'est pas recalcule a chaque
image. Il est mis a jour uniquement quand un evenement qui le concerne
arrive : redimensionnement ou touche espace.

Les trois etats observes

Etat 1, au lancement :

    document.txt - 1280x720

Etat 2, apres redimensionnement de la fenetre :

    document.txt - 1024x600

La taille affichee change pour refleter la nouvelle taille reelle de
la fenetre. Si on redimensionne plusieurs fois, chaque nouvelle taille
apparait.

Etat 3, apres avoir appuye sur la barre espace :

    document.txt * - 1024x600

L'asterisque apparait apres le nom du document. C'est la convention
classique d'un editeur de texte : le document a ete modifie depuis la
derniere sauvegarde.

Un quatrieme appui sur espace fait disparaitre l'asterisque. Le cycle
est reversible.

Tableau recapitulatif :

| Etat | Titre affiche |
|------|---------------|
| Au lancement | document.txt - 1280x720 |
| Apres redimensionnement | document.txt - 1024x600 |
| Apres barre espace | document.txt * - 1024x600 |
| Apres un second espace | document.txt - 1024x600 |

Comment le titre est mis a jour au bon moment

L'enonce demande de ne pas mettre a jour le titre a chaque image. Notre
programme respecte cette consigne. Voici la preuve, lue dans le code :

- La fonction updateTitle n'est appelee qu'a trois endroits.
- Au demarrage, une fois, pour poser le titre initial.
- Dans le callback NkWindowResizeEvent, a chaque changement de taille.
- Dans le callback NkKeyPressEvent, a chaque appui sur la barre espace.

La boucle principale appelle uniquement events.PollEvents(). Elle ne
touche pas au titre. Sans les callbacks, le titre ne changerait jamais.
Sans la boucle, les callbacks ne seraient jamais appeles.

Ce que cela montre

Le titre est un etat derive. Il depend de deux choses : la taille de
la fenetre et l'etat modifie du document. Ces deux choses changent lors
d'evenements precis. Recalculer le titre a chaque image serait du
travail inutile : la plupart du temps, rien n'a change. C'est pour cela
que l'enonce insiste sur "au bon moment".

Les evenements de redimensionnement sont emis par le systeme quand la
fenetre change de taille. Notre programme ne fait que repondre. Il ne
surveille rien activement.

Le comportement a la fermeture

La boucle se termine quand window.IsOpen() retourne faux. Le systeme
envoie un evenement NkWindowCloseEvent quand l'utilisateur clique sur
la croix. Nous n'enregistrons pas de callback pour cet evenement parce
que NkWindow passe IsOpen a faux tout seul. La boucle principale voit
le changement et sort proprement.

Mesure faite le 27/09/2026.
Version de Jenga : 2.8.0.
