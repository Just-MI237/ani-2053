Demonstration 2 - Les trois hauteurs

Le chapitre distingue trois hauteurs de traitement d'un geste. Je les
ai identifiees dans le moteur, puis j'ai choisi pour cinq usages celle
qui convient. Deux choix ont ete essayes reellement.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Les trois hauteurs dans le moteur

Hauteur 1, l'evenement brut.

C'est un callback sur un evenement ponctuel. Le moteur produit un
NkKeyPressEvent, NkKeyReleaseEvent ou NkTextInputEvent a chaque fois
que le clavier bouge. Le callback est appele une fois par evenement.

Fichier : Kernel/Runtime/NKEvent/src/NKEvent/NkKeyboardEvent.h
Lignes 384 et suivantes pour la declaration.

Code typique :

    events.AddEventCallback<NkKeyPressEvent>([](NkKeyPressEvent *e) {
        if (e->GetKey() == NkKey::NK_SPACE) { /* sauter une fois */ }
    });

Cette hauteur voit le geste au moment ou il arrive. Elle voit aussi le
relachement, l'auto-repeat, et les modificateurs. Elle ne voit rien
entre deux evenements.

Hauteur 2, l'etat interroge.

C'est une question posee a tout moment : la touche est-elle enfoncee
maintenant ? Le moteur tient un etat courant, et NkInputQuery le
consulte.

Fichier : Kernel/Runtime/NKEvent/src/NKEvent/NkEventDispatcher.h
Ligne 273 pour IsKeyDown.

Code typique :

    if (NkInput.IsKeyDown(NkKey::NK_W)) { avancer(); }

Cette hauteur voit l'etat a chaque image, mais ne sait pas quand il a
change. Elle ne distingue pas un premier appui d'un maintien.

Hauteur 3, l'action nommee.

C'est une abstraction : un nom (Sauter, Avancer, OuvrirMenu) est lie a
une ou plusieurs touches ou boutons. Le code de jeu ne parle plus du
clavier, il parle des actions.

Fichier : NkEventDispatcher.h, ligne 819 pour NkActionManager.

Le vrai API, aux lignes 819 et suivantes de NkEventDispatcher.h :

    void CreateAction(const NkString &name, NkActionSubscriber handler);
    void AddCommand(const NkActionCommand &cmd);
    void TriggerAction(const NkInputCode &code, bool isPressed);

Et NkActionCommand, ligne 601 :

    NkActionCommand(NkString name, NkInputCode code, bool repeatable = true);

Code typique :

    NkActionManager actions;
    actions.CreateAction("Sauter", handler);
    actions.AddCommand(NkActionCommand("Sauter", NkInputCode(NkKey::NK_SPACE)));
    ...
    actions.TriggerAction(code, true);

Le nom "Sauter" est lie a une ou plusieurs commandes. Quand un input
arrive, TriggerAction cherche quelle action il declenche et appelle le
handler correspondant. Le code de jeu n'a plus besoin de connaitre la
touche.

Les cinq usages

1. Ouvrir un menu. Hauteur 3 (action nommee). Le menu est un instant
qui peut etre declenche par plusieurs touches (Echap, M, ou un bouton
de manette). Nommer l'action fait que le code du menu n'a pas besoin
de connaitre ces touches.

2. Deplacer un personnage. Hauteur 2 (etat interroge). Le deplacement
dure tant que la touche est enfoncee, il faut lire l'etat a chaque
image. C'est le cas typique du polling.

3. Sauter. Hauteur 1 (evenement) ou hauteur 3 (action). C'est le
premier des deux cas qui se discutent. Si on veut qu'un appui donne un
saut, un evenement suffit et evite de repeter le saut pendant que la
touche reste enfoncee. Mais si on veut que le saut soit aussi
declenchable par un bouton de manette, l'action nommee est plus
propre. Mon choix : action nommee, parce qu'un jeu vise rarement un
seul peripherique.

4. Raccourci clavier. Hauteur 1 (evenement). Un raccourci est lie a
une combinaison de touches precise (Ctrl+S). L'evenement porte les
modificateurs, l'etat interroge ne les distingue pas aussi
directement.

5. Viser. Hauteur 2 (etat interroge), ou hauteur 1 pour la souris
brute. C'est le deuxieme cas qui se discute. Avec une souris
classique, l'etat suffit. Avec une souris capturee ou un viseur
analogique, l'evenement brut est plus fidele parce qu'il conserve tous
les mouvements, meme ceux qui sortent du cadre.

Les deux cas qui se discutent

Sauter et viser. Pour sauter, l'evenement garantit un seul saut par
appui, l'action garantit la portabilite entre peripheriques. Pour
viser, l'etat suffit pour un curseur visuel, l'evenement brut est
necessaire pour un FPS qui utilise toute la plage de mouvement.

Ce qui fait pencher d'un cote ou de l'autre

Pour sauter : le nombre de peripheriques a supporter. Un seul (le
clavier) et l'evenement suffit. Plusieurs (clavier, manette, ecran
tactile) et l'action devient necessaire.

Pour viser : le type de souris. Libre, l'etat suffit. Captivee, le
brut est necessaire.

Verification reelle de deux choix

Test reel des trois hauteurs, avec un seul appui sur Espace.

J'ai ecrit un programme qui enregistre les trois hauteurs en meme
temps et qui affiche ce que chacune voit a chaque instant :

- hauteur 1 : un callback NkKeyPressEvent sur NK_SPACE,
- hauteur 2 : un test NkInput.IsKeyDown(NK_SPACE) a chaque frame,
- hauteur 3 : une action "Sauter" liee a NK_SPACE, declenchee a
  chaque frame ou l'etat est vrai.

J'ai appuye sur Espace une seule fois, brievement. Sortie brute :

    [H1-evenement] NK_SPACE appuyee, total=1
    [H2-etat] IsKeyDown(NK_SPACE) = 1 (frame 143)
    [H3-action] Sauter declenchee, total=1
    [H3-action] Sauter declenchee, total=2
    [H3-action] Sauter declenchee, total=3
    [H3-action] Sauter declenchee, total=4
    [H3-action] Sauter declenchee, total=5
    [H2-etat] IsKeyDown(NK_SPACE) = 0 (frame 148)

Ce que chacune voit pour le meme geste

Hauteur 1 : un seul evenement. total=1. Un appui, une ligne.
Hauteur 2 : deux transitions. Le passage de 0 a 1 a la frame 143, le
passage de 1 a 0 a la frame 148. Entre les deux, l'etat est vrai, mais
la ligne n'apparait qu'au moment du changement.
Hauteur 3 : cinq declenchements. L'action a ete appelee a chaque frame
ou l'etat etait vrai. Entre 143 et 148, il y a cinq frames. Chacune a
produit un declenchement de l'action.

La lecon de ce test

Le meme geste est vu une fois, deux fois, ou cinq fois selon la
hauteur. Aucune des trois n'est fausse. Elles repondent a des
questions differentes :

- l'evenement dit quand le geste a eu lieu (une fois),
- l'etat dit si le geste est en cours (vrai entre 143 et 148),
- l'action, telle que je l'ai branchee, dit combien de fois l'etat a
  ete vrai. C'est la consequence du branchement, pas une propriete de
  l'action.

Ce dernier point est le vrai sujet. Une action nommee n'est pas en soi
repétable ou non. C'est le code qui appelle TriggerAction qui decide.
Branchee a chaque frame sur l'etat, elle repete. Branchee seulement
au front montant, elle ne repete pas. C'est ce que la demonstration
montre.

Choix 3 (sauter) : pour un saut, un declenchement par appui suffit.
Il faut donc brancher l'action sur le front montant de l'etat, ou sur
l'evenement NkKeyPressEvent, pas sur l'etat continu. La mesure
ci-dessus montre ce qui se passerait sinon : cinq sauts pour un appui.

Choix 4 (raccourci Ctrl+S) : essaye dans un callback
NkKeyPressEvent qui lit les modificateurs avec HasCtrl() et compare
la touche avec NK_S. Le raccourci se declenche une seule fois par
appui. Le meme probleme ne se pose pas parce que le branchement est
deja sur l'evenement, pas sur l'etat.

Ce que cela montre

Les trois hauteurs ne sont pas concurrentes. Elles repondent a trois
questions differentes. L'evenement dit quand. L'etat dit si. L'action
dit quoi. Une application reelle emploie les trois : l'etat pour le
deplacement continu, l'action pour les decisions de jeu, l'evenement
pour les entrees de menu et les raccourcis.

Mesure faite le 28/09/2026.
Version de Jenga : 2.8.0.
