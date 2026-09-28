Demonstration 3 - La reconfiguration en direct

L'enonce demande trois choses : comment on rend la configuration
modifiable a chaud, ce que la personne de l'entourage a choisi, et ce
qui a casse. Je reponds aux trois, avec les mesures que j'ai pu faire.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Comment la configuration est modifiable a chaud

Le moteur expose cinq methodes sur NkActionManager qui permettent de
changer le mapping sans relancer le programme.

Fichier : Kernel/Runtime/NKEvent/src/NKEvent/NkEventDispatcher.h
Lignes 827, 838, 840, 842 et 844 :

    void Clear() noexcept;
    void CreateAction(const NkString &name, NkActionSubscriber handler);
    void AddCommand(const NkActionCommand &cmd);
    void RemoveAction(const NkString &name);
    void RemoveCommand(const NkActionCommand &cmd);

Pour changer la touche d'une action a chaud, le programme fait :

    actions.RemoveCommand(NkActionCommand("Sauter", NkInputCode::Key(NkKey::NK_SPACE)));
    actions.AddCommand(NkActionCommand("Sauter", NkInputCode::Key(NkKey::NK_J)));

La premiere ligne dissocie l'ancienne touche. La seconde associe la
nouvelle. L'action "Sauter" n'a pas ete recreee, son handler reste
le meme. Seule la touche a change.

Ce qui permet cela : l'action et la commande sont deux objets separes.
L'action porte le nom et le handler. La commande porte le lien entre
une action et un input physique. Changer le lien ne touche pas
l'action.

L'essai avec quelqu'un d'autre

J'ai demande a une personne qui ne connait pas les jeux de choisir
mes touches sans me dire lesquelles. Sa configuration :

- Avancer : Fleche haut
- Reculer : Fleche bas
- Gauche : Fleche gauche
- Droite : Fleche droite
- Sauter : Entree
- Attaquer : Entree

Elle a mis la meme touche pour Sauter et Attaquer, parce que ca lui
semblait plus simple. Et elle a supprime OuvrirInventaire du clavier,
parce qu'elle ne s'en servait jamais.

Ce que ca m'a fait de jouer avec

Rien ne bougeait pour l'inventaire. Appuyer sur Entree declenchait a
la fois un saut et une attaque. Ce n'est pas ce que j'aurais choisi,
mais c'est exactement ce qu'une personne qui ne connait pas le jeu
aurait pu faire en deux minutes.

Verification du premier etat impossible

J'ai reproduit la configuration de la personne dans un programme de
test, avec trois actions et trois commandes :

    actions.CreateAction("Sauter", ...);
    actions.CreateAction("Attaquer", ...);
    actions.CreateAction("OuvrirInventaire", ...);

    actions.AddCommand(NkActionCommand("Sauter",   NkInputCode::Key(NkKey::NK_ENTER)));
    actions.AddCommand(NkActionCommand("Attaquer", NkInputCode::Key(NkKey::NK_ENTER)));
    // OuvrirInventaire recoit zero commande.

Au demarrage, le programme affiche le nombre de commandes par action :

    Actions : Sauter=1 Attaquer=1 OuvrirInventaire=0

Sauter et Attaquer ont chacune une commande (la meme touche).
OuvrirInventaire n'en a aucune.

J'appuie une seule fois sur Entree. Sortie brute :

    [action] Sauter declenchee, total=1
    [action] Attaquer declenchee, total=1

Un appui. Deux actions. Le programme a execute deux handlers
incompatibles (on ne saute pas et on n'attaque pas en meme temps).
C'est le premier etat impossible, mesure.

Verification du second

OuvrirInventaire a zero commande. Aucun appui ne la declenchera. Elle
existe dans le moteur (GetActionCount la compte), mais elle est muette.
Son handler ne sera jamais appele par le clavier. Le programme peut
continuer a croire qu'elle fonctionne. C'est le second etat impossible,
mesure.

Le troisieme etat impossible

Une touche reservee par le systeme. Si l'utilisateur lie une action a
Alt+F4 ou a Ctrl+Alt+Suppr, le systeme intercepte l'evenement avant
que le programme le voie. L'action ne se declenchera jamais.

Je n'ai pas teste ce cas. Le tester voudrait dire lier une action a
Alt+F4, appuyer, et constater que le programme ne la voit pas. Je le
laisse comme analyse.

Ce que je ferais pour empecher ces trois etats

1. Deux actions sur la meme touche. Avant AddCommand, parcourir les
commandes existantes avec ForEachCommand, et refuser si la touche
demandee est deja prise. Afficher un message a l'utilisateur.

2. Une action sans touche. Apres RemoveCommand, verifier avec
GetCommandCount(name) qu'il reste au moins une commande. Si non,
retirer aussi l'action avec RemoveAction, ou avertir l'utilisateur.

3. Touche reservee par le systeme. Tenir une liste des combinaisons
connues comme reservees (Alt+F4, Ctrl+Alt+Suppr, Super+L), et refuser
si l'utilisateur essaie d'y lier une action. La liste est specifique
au systeme d'exploitation.

Aucune de ces protections n'est dans le moteur aujourd'hui. Elles
seraient a la charge de l'application.

Ce que cela montre

NkActionManager permet la reconfiguration a chaud, ce qui est la
premiere condition pour un jeu qui laisse le joueur regler ses
commandes. Mais il ne protege pas contre les etats impossibles. Le
programme appelant doit verifier lui-meme avant chaque modification.

C'est une decision d'interface : soit le moteur valide les operations
et refuse les combinaisons invalides, soit il laisse faire et c'est a
l'application de se defendre. Le moteur a choisi la seconde. Elle est
plus souple, mais elle delegue a chaque programme un travail qui
pourrait etre fait une fois pour toutes.

Mesure faite le 28/09/2026.
Version de Jenga : 2.8.0.
