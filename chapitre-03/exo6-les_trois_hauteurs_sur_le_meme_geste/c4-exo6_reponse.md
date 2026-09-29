Exercice 6 - Les trois hauteurs, sur le meme geste

J'ai ecrit un programme qui fait avancer un carre a la fleche droite,
trois fois, par trois mecanismes differents. Meme fichier source,
choix par la variable d'environnement NK_MODE.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Les trois modes

Mode event. Le programme s'abonne a NkKeyPressEvent. A chaque appui
sur fleche droite, il incremente la position.

    ev.AddEventCallback<NkKeyPressEvent>([&](NkKeyPressEvent *e) {
        if (e->GetKey() == NkKey::NK_RIGHT) {
            position += 1;
        }
    });

Mode state. Le programme interroge NkInput.IsKeyDown(NK_RIGHT) a chaque
frame, et ne fait avancer que sur le front montant (passage de faux a
vrai).

    bool now = NkInput.IsKeyDown(NkKey::NK_RIGHT);
    if (now && !last) {
        position += 1;
    }
    last = now;

Mode action. Le programme lie une action nommee "Avancer" a la fleche
droite, puis declenche l'action sur le front montant.

    actions.CreateAction("Avancer", [&](...){ position += 1; });
    actions.AddCommand(NkActionCommand("Avancer", NkInputCode::Key(NkKey::NK_RIGHT)));

Les trois mesures

Commande :

    NK_MODE=event jenga run TestTroisHauteurs --config Debug --platform x86_64 --target Linux

Pour chaque mode, j'ai appuye une seule fois sur fleche droite.

Sortie brute event :

    === Mode : event ===
    [event] Fleche droite recue, position=1
    [fin] mode=event, position finale=1

Sortie brute state :

    === Mode : state ===
    [state] front montant, position=1
    [fin] mode=state, position finale=1

Sortie brute action :

    === Mode : action ===
    [action] Avancer, position=1
    [fin] mode=action, position finale=1

Les trois mecanismes produisent le meme resultat : un appui, un
avancement. Les trois programmes finissent avec position=1.

Ce que chacun connait du clavier

Le mode event connait la touche exacte. Il compare e->GetKey() avec
NkKey::NK_RIGHT. Si on veut changer la touche, il faut changer cette
comparaison, dans le callback.

Le mode state connait aussi la touche exacte. Il interroge
NkInput.IsKeyDown(NkKey::NK_RIGHT). Si on veut changer la touche, il
faut changer l'argument.

Le mode action ne connait pas la touche. Il connait un nom,
"Avancer". Le lien entre le nom et la touche est ailleurs, dans la
ligne AddCommand. Pour changer la touche, il faut changer cette
ligne, pas le handler.

Tableau recapitulatif

| Mode | Ou se trouve la touche | Ce que le handler connait |
|------|------------------------|---------------------------|
| event | dans la comparaison GetKey() | la touche |
| state | dans l'appel IsKeyDown() | la touche |
| action | dans la ligne AddCommand | le nom de l'action |

Ce que cela change pour le code

Dans les modes event et state, le code de la regle (avancer d'une
case) est melange avec le code de l'entree (c'est la fleche droite).
Si on veut que la fleche haut avance aussi, il faut modifier la regle.

Dans le mode action, le code de la regle ne mentionne aucune touche.
Il dit seulement : quand l'action "Avancer" est declenchee, avancer
d'une case. On peut ajouter la fleche haut comme commande de la meme
action, la regle ne change pas.

C'est la difference entre une regle qui connait le clavier et une
regle qui ne le connait pas.

Ce que cela montre

Les trois hauteurs font la meme chose pour l'utilisateur : un appui,
un avancement. Mais elles ne le font pas de la meme maniere pour le
programmeur. Plus on s'eloigne du clavier (de event vers action), plus
la regle devient independante du peripherique.

C'est le sujet du chapitre : separer les regles de l'entree. Une
regle qui dit "avancer d'une case quand Avancer est declenchee" peut
etre declenchee par un clavier, une manette, un ecran tactile, ou un
journal de rejeu, sans changement.

Mesure faite le 29/09/2026.
Version de Jenga : 2.8.0.
