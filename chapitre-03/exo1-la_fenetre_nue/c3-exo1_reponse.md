Exercice 1 - La fenetre nue

J'ai ecrit le plus petit programme qui ouvre une fenetre, la garde
ouverte et se termine proprement. Il fait 17 lignes. Chaque ligne
est retrouvee dans le chapitre 3.

Datation de la mesure

Commande :

```
git log -1 --format="%h %ad" --date=short
```

Sortie brute :

```
9c3fad3 2026-09-13
```

Version de Jenga : 2.8.0.

Le programme

Fichier depose a cote sous le nom c3-exo1_main.cpp. Contenu :

```
     1	#include "NKWindow/NKWindow.h"
     2	#include "NKWindow/NKMain.h"
     3	
     4	int nkmain(const NkEntryState &state) {
     5	    NkWindowConfig cfg;
     6	    cfg.title  = "Ma fenetre";
     7	    cfg.width  = 1280;
     8	    cfg.height = 720;
     9	
    10	    NkWindow window(cfg);
    11	    if (!window.IsOpen()) {
    12	        logger.Error("[app] creation fenetre echouee");
    13	        return -1;
    14	    }
    15	    while (window.IsOpen()) { }
    16	    return 0;
    17	}
```

Comptage des lignes

Commande :

```
wc -l c3-exo1_main.cpp
```

Sortie brute :

```
17 c3-exo1_main.cpp
```

Le programme fait 17 lignes, en comptant les deux lignes vides
(lignes 3 et 9) et les accolades.

Ou chaque ligne se retrouve dans le chapitre

Ligne 1 : #include "NKWindow/NKWindow.h"
  Le chapitre dit que ce fichier fournit le type NkWindow et la
  structure NkWindowConfig. Sans lui, le compilateur ne connait ni
  la classe ni ses options.

Ligne 2 : #include "NKWindow/NKMain.h"
  Le chapitre consacre un paragraphe entier a ce fichier. Il ecrit :
  oublier cet include donne undefined reference to WinMain. C'est
  le piege du chapitre. Ce fichier fournit le point d'entree natif
  (WinMain sur Windows, android_main sur Android, UIApplicationMain
  sur iOS) et le relie a la fonction nkmain que j'ecris.

Ligne 4 : int nkmain(const NkEntryState &state) {
  Le chapitre dit : vous n'ecrivez pas de main. Vous ecrivez nkmain.
  Le parametre NkEntryState porte l'etat d'entree fourni par la
  plateforme (ligne de commande, fichiers ouverts, etc.).

Ligne 5 : NkWindowConfig cfg;
  Le chapitre dit que la configuration se lit par familles :
  identite et taille, droits de l'utilisateur, apparence et rendu.
  La variable cfg regroupe tout cela avant la creation.

Lignes 6 a 8 : cfg.title, cfg.width, cfg.height
  La famille identite et taille du chapitre correspond exactement a
  ces trois champs. Le titre affiche dans la barre de la fenetre, la
  largeur et la hauteur en pixels logiques au demarrage.

Ligne 10 : NkWindow window(cfg);
  Le chapitre dit que la configuration se donne au constructeur.
  C'est a cette ligne que la fenetre est creee. Avant, il n'y a rien.
  Apres, on peut la piloter par methodes.

  Le chapitre dit : on verifie IsOpen. Une creation peut echouer,
  pilote absent, serveur graphique inaccessible, permissions. Un
  programme qui continue apres cela travaille dans le vide.

Ligne 12 : logger.Error(...)
  Message dans le journal quand la creation echoue. Le chapitre
  ne detaille pas ce point, mais c'est la contrepartie naturelle du
  test de la ligne 11.

Ligne 13 : return -1;
  Sortie en erreur. On rend un code non nul pour que l'appelant
  sache que la fenetre n'a pas pu etre creee.

Ligne 15 : while (window.IsOpen()) { }
  C'est la boucle principale. Le chapitre dit que les evenements
  arrivent ici. Je n'en traite aucun pour rester au minimum, mais
  la boucle tourne tant que la fenetre est ouverte. La condition
  IsOpen passe a faux quand l'utilisateur ferme la fenetre.

Ligne 16 : return 0;
  Le programme se termine proprement, avec un code de succes.

Ligne 17 : }
  Fermeture de la fonction nkmain.

Ce que le programme ne fait pas

Il n'ouvre pas de contexte graphique. Il ne dessine rien. Il ne
traite aucun evenement. Il ne lit ni clavier ni souris. C'est le
minimum du chapitre 3 : la fenetre nue.

Ce que le programme prouve

Il prouve trois choses que le chapitre met en avant. La premiere :
nkmain remplace main sans que je m'occupe du point d'entree natif.
La deuxieme : la configuration passe au constructeur, pas apres.
La troisieme : IsOpen est obligatoire avant de continuer.

Mesure faite le 26/09/2026.
Version de Jenga : 2.8.0.
