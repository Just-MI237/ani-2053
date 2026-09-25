Exercice 9 - L'utilisateur avant le lecteur

J'ai lance NkRef et je m'en suis servi pendant une dizaine de minutes
sans lire une ligne de son code. Voici deux listes : ce qu'il fait,
et ce que j'aurais voulu qu'il fasse. Elles sont datees et je les
relis au chapitre 16.

Datation

Commande :

```
git log -1 --format="%h %ad" --date=short
```

Sortie brute :

```
9c3fad3 2026-09-13
```

Version de Jenga : 2.8.0.
Date de la session : 25/09/2026.

Ce que NkRef fait

- Il ouvre une fenetre 1280x800 avec un fond sombre quadrille.
- Il affiche un panneau Proprietes sur le cote droit avec plusieurs
  sections : Theme sombre, Fenetre, Planche, Crayon, Gestes.
- Il propose un mode crayon active par la touche D. On peut dessiner
  des traits a main levee dans la zone centrale.
- Il propose huit couleurs de crayon : Orange, Petrole, Rouge, Vert,
  Bleu, Blanc, Noir.
- Il propose un reglage d'epaisseur du trait, exprime en pixels.
- Il propose deux boutons : Annuler le dernier trait, et Effacer tous
  les traits.
- Il affiche une grille qu'on peut activer ou desactiver avec la
  touche G.
- Il propose un theme sombre qu'on peut decocher.
- Il propose une option Toujours devant pour garder la fenetre au-
  dessus des autres.
- Il propose un reglage d'opacite de la fenetre.
- Il propose l'option Glisser le fond = fenetre pour deplacer la
  fenetre en cliquant sur le fond.
- Il propose des gestes de navigation : molette pour le zoom,
  milieu ou Espace+glisser pour le deplacement, Ctrl+glisser pour
  selectionner un rectangle, les coins pour l'echelle, le rond pour
  la rotation, PgUp/PgDn pour l'ordre des calques.
- Il indique dans le panneau Planche combien il y a d'images, de
  selections et de traits en cours.
- Il gere un atlas de police embarque : la police DroidSans est
  chargee automatiquement a deux tailles differentes.

Ce que j'aurais voulu que NkRef fasse

- J'aurais voulu qu'il y ait un menu Fichier visible avec des
  entrees comme Ouvrir, Enregistrer, Exporter. Je n'en ai pas vu.
  Peut-etre qu'il existe mais je ne l'ai pas trouve.

- J'aurais voulu pouvoir importer une image depuis mon disque. Le
  panneau Planche affiche 0 image et je n'ai pas trouve comment en
  ajouter une.

- J'aurais voulu une barre d'outils avec des icones visibles pour
  les outils principaux (crayon, gomme, selection, forme). La, tout
  passe par le panneau Proprietes.

- J'aurais voulu pouvoir dessiner des formes geometriques simples
  (cercle, rectangle, ligne droite). Le mode crayon ne fait que du
  trait a main levee.

- J'aurais voulu une gomme separee du crayon. Pour l'instant, pour
  effacer, il faut utiliser Annuler le dernier trait ou Effacer tous
  les traits. On ne peut pas effacer un trait precis.

- J'aurais voulu un raccourci clavier pour chaque couleur, ou un
  selecteur de couleur libre. La, il faut cliquer sur un des huit
  boutons pre-definis.

- J'aurais voulu voir les dimensions de la zone de dessin affichees
  quelque part (largeur x hauteur), pour savoir ou je travaille.

- J'aurais voulu que le panneau Proprietes puisse se replier pour
  laisser plus de place au dessin.

- J'aurais voulu une indication visuelle de l'outil actif. Le mode
  crayon a une case a cocher, mais rien ne dit si d'autres outils
  existent.

- J'aurais voulu pouvoir faire defiler la zone de dessin avec une
  barre de defilement visible, au lieu de devoir utiliser
  Espace+glisser.

Ce que j'ai remarque pendant la session

Au lancement, NkRef affiche plusieurs messages dans le terminal. Il
cherche a ouvrir un contexte OpenGL 4.6, en obtient un 4.5, et
continue malgre tout. Il signale que llvmpipe (le rendu logiciel de
Mesa) est utilise, ce qui est normal sous WSL. Il charge la police
DroidSans en deux tailles et construit un atlas de 1024x1024 puis
un de 512x512.

Un message d'avertissement apparait : texture 1024x1024, AUCUN pixel
visible dans toute l'image. Le rendu de la police semble donc poser
probleme sur cette configuration. C'est peut-etre pour ca que
certains textes de l'interface sont difficiles a lire.

Ce que je retiens

NkRef est une application de dessin minimaliste, utilisable, avec
une interface reduite a un panneau de proprietes. Il fait peu de
choses, mais il les fait. Le mode crayon, la grille, le zoom et le
pan fonctionnent. Pour le reste, il manque beaucoup de ce qu'on
attend d'un outil de dessin : import d'image, formes geometriques,
gomme precise, export. C'est une version de developpement.


Preuves brutes de la session

Commande :

    jenga run NkRef --config Debug --platform x86_64 --target Linux

Extrait du terminal, au lancement :

    [NkRef] police en-tete : chargee
    [NkFontEmbedded] Police 'DroidSans' chargee : 17px
    [NkFontAtlas] Build(): 723 glyphes rasterises, atlas 1024x1024,
      745 points non nuls
    [NkOpenGLRenderer2D] texture 1024x1024 : AUCUN pixel visible dans
      toute l'image
    [NkFontEmbedded] Police 'DroidSans' chargee : 15px
    [NkFontAtlas] Build(): 190 glyphes rasterises, atlas 512x512,
      120 points non nuls

Extrait du terminal, pendant la session, quand j'ai essaye de coller
une image trois fois de suite :

    [NkRef] presse-papiers : pas d'image
    [NkRef] presse-papiers : pas d'image
    [NkRef] presse-papiers : pas d'image

Extrait du terminal, a la fin de la session :

    [NkOpenGLRenderer2D] DeleteGLTexture id=2
    [NkOpenGLRenderer2D] DeleteGLTexture id=3
    [NkOpenGLRenderer2D] Shutdown
    [NkOpenGL] Shutdown OK

    ◀  FIN D'EXECUTION — termine normalement  (545.63s)

Duree de la session : 545.63 secondes, soit environ 9 minutes et 6
secondes. C'est la duree totale de la fenetre NkRef ouverte, pas le
temps passe a dessiner.

Contexte OpenGL, extrait du terminal :

    [NkOpenGL] Using legacy GLX context path (WSL)
    [NkOpenGL] GLX OK (GL 4.6)
    [NkOpenGL] GL 4.6 demande, 4.5 obtenu - on continue
    [NkOpenGL] Ready - llvmpipe (LLVM 15.0.7, 256 bits) |
      4.5 (Compatibility Profile) Mesa 23.2.1-1ubuntu3.1~22.04.3 | Mesa

Ces lignes viennent de la sortie du terminal au lancement. Elles
montrent que la session a bien eu lieu, que la police a ete chargee
et que l'application s'est terminee normalement.

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
