Exercice 6 - Les trois hauteurs, sur le meme geste

J'ai ecrit un programme qui fait avancer un carre a la fleche droite,
trois fois, par trois mecanismes differents. Le carre est dessine dans
une fenetre 800x400. Chaque appui le fait avancer de 100 pixels. Meme
fichier source, choix par la variable d'environnement NK_MODE.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que fait le programme

Le programme cree une fenetre, un contexte OpenGL et un renderer 2D.
A chaque image, il dessine un rectangle bleu de 50 pixels de cote, a la
position horizontale courante. La position commence a 50 et augmente de
100 a chaque appui utile sur la fleche droite.

Les trois modes

Mode event. Le programme s'abonne a NkKeyPressEvent. A chaque appui sur
fleche droite, il ajoute 100 a la position.

    ev.AddEventCallback<NkKeyPressEvent>([&](NkKeyPressEvent *e) {
        if (e->GetKey() == NkKey::NK_RIGHT) {
            x += 100;
        }
    });

Mode state. Le programme interroge NkInput.IsKeyDown(NK_RIGHT) a chaque
frame, et ne fait avancer que sur le front montant (passage de faux a
vrai).

    bool now = NkInput.IsKeyDown(NkKey::NK_RIGHT);
    if (now && !last) { x += 100; }
    last = now;

Mode action. Le programme lie une action nommee "Avancer" a la fleche
droite, puis declenche l'action sur le front montant.

    actions.CreateAction("Avancer", [&](...){ if (p) x += 100; });
    actions.AddCommand(NkActionCommand("Avancer", NkInputCode::Key(NkKey::NK_RIGHT)));

Les trois mesures

Pour chaque mode, j'ai appuye trois fois sur fleche droite.

Sortie brute event :

    === Mode : event ===
    [event] Fleche droite, x=150
    [event] Fleche droite, x=250
    [event] Fleche droite, x=350
    [fin] mode=event, position finale x=350

Sortie brute state :

    === Mode : state ===
    [state] front montant, x=150
    [state] front montant, x=250
    [state] front montant, x=350
    [fin] mode=state, position finale x=350

Sortie brute action :

    === Mode : action ===
    [action] Avancer, x=150
    [action] Avancer, x=250
    [action] Avancer, x=350
    [fin] mode=action, position finale x=350

Les trois mecanismes produisent le meme resultat : trois appuis, le
carre part de 50 et arrive a 350. A l'ecran, le carre avance de la meme
facon dans les trois cas. L'utilisateur ne voit pas la difference.

Ce que chacun connait du clavier

Le mode event connait la touche exacte. Il compare e->GetKey() avec
NkKey::NK_RIGHT. Si on veut changer la touche, il faut changer cette
comparaison, dans le callback.

Le mode state connait aussi la touche exacte. Il interroge
NkInput.IsKeyDown(NkKey::NK_RIGHT). Si on veut changer la touche, il
faut changer l'argument.

Le mode action ne connait pas la touche. Il connait un nom, "Avancer".
Le lien entre le nom et la touche est ailleurs, dans la ligne
AddCommand. Pour changer la touche, il faut changer cette ligne, pas
le handler.

Tableau recapitulatif

| Mode | Ou se trouve la touche | Ce que le handler connait |
|------|------------------------|---------------------------|
| event | dans la comparaison GetKey() | la touche |
| state | dans l'appel IsKeyDown() | la touche |
| action | dans la ligne AddCommand | le nom de l'action |

Ce que cela change pour le code

Dans les modes event et state, le code de la regle (avancer de 100
pixels) est melange avec le code de l'entree (c'est la fleche droite).
Si on veut que la fleche haut avance aussi, il faut modifier la regle.

Dans le mode action, le code de la regle ne mentionne aucune touche.
Il dit seulement : quand l'action "Avancer" est declenchee, avancer de
100 pixels. On peut ajouter la fleche haut comme commande de la meme
action, la regle ne change pas.

C'est la difference entre une regle qui connait le clavier et une
regle qui ne le connait pas.

Ce que cela montre

Les trois hauteurs font la meme chose pour l'utilisateur : trois
appuis, le carre arrive au meme endroit. Mais elles ne le font pas de
la meme maniere pour le programmeur. Plus on s'eloigne du clavier (de
event vers action), plus la regle devient independante du
peripherique.

C'est le sujet du chapitre : separer les regles de l'entree. Une
regle qui dit "avancer de 100 pixels quand Avancer est declenchee"
peut etre declenchee par un clavier, une manette, un ecran tactile, ou
un journal de rejeu, sans changement.

Mesure faite le 29/09/2026.
Version de Jenga : 2.8.0.
