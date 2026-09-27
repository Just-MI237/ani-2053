Exercice 5 - Le titre qui informe

J'ai ecrit un programme qui affiche dans le titre de la fenetre trois
informations : le nom du document, un asterisque quand il est modifie,
et la taille courante de la fenetre. Le titre est mis a jour seulement
quand quelque chose change, pas a chaque image.

Le programme est depose a cote sous le nom c3-exo5_main.cpp.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que fait le programme

Le programme cree une fenetre dont le titre initial est "document.txt".
Il enregistre trois callbacks sur le gestionnaire d'evenements :

- un callback pour NkWindowResizeEvent, qui met a jour le titre quand
  la fenetre change de taille
- un callback pour NkKeyPressEvent, qui bascule l'etat modifie quand
  on appuie sur la barre espace, puis met a jour le titre
- un callback pour NkWindowCloseEvent, qui note la demande de fermeture

La fonction updateTitle construit le titre en concatenant trois
elements : le nom du document, un asterisque si modifie, et la taille
courante de la fenetre lue par window.GetSize().

La boucle principale ne touche pas au titre. Elle se contente de vider
la file d'evenements, puis dort 16 millisecondes pour reguler la
cadence a environ 60 images par seconde. Le programme s'arrete tout
seul apres 15 secondes.

Mesures reelles

Commande :

    jenga run TestTitre --config Debug --platform x86_64 --target Linux

Pendant les 15 secondes, j'ai redimensionne la fenetre trois fois et
j'ai appuye deux fois sur la barre espace.

Ligne finale du journal :

    [fin] images=862, mises a jour titre=35, resize=32, espace=2

Tableau recapitulatif :

| Mesure | Valeur |
|--------|--------|
| Images parcourues | 862 |
| Mises a jour du titre | 35 |
| Evenements de resize | 32 |
| Appuis sur espace | 2 |
| Ratio mises a jour / images | 4,06 pour cent |

Le titre a ete mis a jour 35 fois pendant que la boucle a parcouru 862
images. Autrement dit, dans 96 pour cent des tours de boucle, le titre
n'a pas ete touche. C'est la preuve chiffree que le titre n'est pas
mis a jour a chaque image.

Pourquoi 32 evenements pour trois redimensionnements

Le gestionnaire de fenetres envoie un evenement a chaque changement de
taille, meme d'un seul pixel. Un seul glissement de souris sur le bord
de la fenetre produit des dizaines d'evenements successifs. C'est
visible dans le journal :

    [resize] evenement 6 : 1280x671
    [resize] evenement 7 : 1280x659
    [resize] evenement 8 : 1280x651
    [resize] evenement 9 : 1280x651

La taille varie d'un pixel ou deux a chaque fois. Le titre est mis a
jour a chaque evenement, donc il affiche chaque valeur intermediaire.
C'est ce que veut dire "au bon moment" : a chaque changement reel, pas
a chaque image.

Les trois etats du titre

Etat 1, au lancement :

    document.txt - 1280x720

Etat 2, apres redimensionnement :

    document.txt - 1280x479

La taille affichee correspond exactement a la taille reelle de la
fenetre apres le dernier glissement.

Etat 3, apres un appui sur la barre espace :

    document.txt * - 1280x479

L'asterisque apparait apres le nom du document. Un second appui le fait
disparaitre.

Tableau recapitulatif des etats :

| Etat | Titre affiche |
|------|---------------|
| Au lancement | document.txt - 1280x720 |
| Apres redimensionnement | document.txt - 1280x479 |
| Apres barre espace (modifie) | document.txt * - 1280x479 |
| Apres second espace | document.txt - 1280x479 |

Ou le titre est mis a jour dans le code

La fonction updateTitle n'est appelee qu'a quatre endroits :

- Une fois au demarrage, pour poser le titre initial.
- Dans le callback NkWindowResizeEvent, a chaque changement de taille.
- Dans le callback NkKeyPressEvent, a chaque appui sur la barre espace.
- Aucune autre.

La boucle principale ne touche pas au titre. Elle se contente de vider
la file et de dormir.

Ce que cela montre

Le titre est un etat derive. Il depend de deux choses : la taille de la
fenetre et l'etat modifie du document. Ces deux choses changent lors
d'evenements precis. Recalculer le titre a chaque image serait du
travail inutile : la plupart du temps, rien n'a change. Les chiffres le
montrent : 35 mises a jour pour 862 images.

Le comportement a la fermeture

La boucle se termine quand window.IsOpen() retourne faux. Le systeme
envoie un evenement NkWindowCloseEvent quand l'utilisateur clique sur
la croix. Nous n'enregistrons pas de callback pour arreter le
programme parce que NkWindow passe IsOpen a faux tout seul. La boucle
principale voit le changement et sort proprement.

Mesure faite le 27/09/2026.
Version de Jenga : 2.8.0.
