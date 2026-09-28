Demonstration 1 - Le chemin d'une touche

J'ai suivi une seule touche, depuis le systeme jusqu'a mon code, en
lisant le moteur et en instrumentant un petit programme.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Le programme de test

Un programme qui s'abonne a NkKeyPressEvent et affiche le code a
chaque appui. Sa boucle appelle NkEvents().PollEvents() toutes les
16 ms.

Sortie brute, apres avoir appuye sur plusieurs touches :

    [touche] 41
    [touche] 72
    [touche] 70
    [touche] 60
    [touche] 82
    [touche] 49
    [touche] 63
    [touche] 50
    [touche] 57
    [touche] 59

Le chemin, en cinq etapes

Etape 1. Le serveur X envoie un evenement clavier.

Fichier : Kernel/Runtime/NKWindow/src/NKWindow/Platform/XLib/NkXLibEventSystem.cpp
Ligne : 135

    XNextEvent(display, &xev);

C'est Xlib qui recoit. Le backend X11 est en train de vider la file
d'evenements du serveur X, et l'evenement courant est une touche.

Etape 2. Le backend reconnait le type et traduit le code.

Fichier : NkXLibEventSystem.cpp
Lignes : 168 et 173

    case KeyPress:
        ...
        KeySym ks = XLookupKeysym(&xev.xkey, 0);

Le switch lit le type. La ligne 173 convertit le code natif X11
(keysym) en un code NkKey, qui est l'enumeration du moteur.

Etape 3. L'evenement du moteur est cree et range dans la file.

Fichier : NkXLibEventSystem.cpp
Lignes : 182 et 183

    NkKeyPressEvent e(key, sc, mods, nativeKey);
    Enqueue(e, winId);

Le backend instancie un NkKeyPressEvent, qui porte la touche, le
scancode, les modificateurs (Ctrl, Maj...), et l'identifiant de la
fenetre concernee. Puis il le depose dans la file d'evenements, en
indiquant la fenetre d'origine.

Etape 4. La file est videe.

Fichier : Kernel/Runtime/NKEvent/src/NKEvent/NkEventSystem.cpp
Ligne : 367

    void NkEventSystem::PollEvents() {

Chaque appel de PollEvents (fait par notre boucle) parcourt les
evenements en attente. Pour chaque evenement du type auquel on s'est
abonne, il invoque le callback correspondant.

Etape 5. Mon code recoit l'appel.

Fichier : le programme de test (main.cpp)

    ev.AddEventCallback<NkKeyPressEvent>([](NkKeyPressEvent *e) {
        fprintf(stderr, "[touche] %d\n", (int)e->GetKey());
    });

La ligne [touche] 41 dans la sortie est cette ligne-la. Elle s'execute
parce que les quatre etapes precedentes ont eu lieu.

Ce qui se passe a chaque etape

Etape 1 : le systeme envoie. Xlib lit.
Etape 2 : le backend traduit. Le code natif X11 devient un NkKey.
Etape 3 : le backend range. L'evenement NkKeyPressEvent entre dans la
file, avec son type et sa fenetre d'origine.
Etape 4 : le moteur distribue. PollEvents parcourt la file et appelle
les abonnes.
Etape 5 : mon programme recoit. La ligne s'execute.

Ce que cela montre

Le chemin d'une touche traverse trois couches qui ont chacune un
role distinct. Le systeme fournit un code brut. Le backend le
traduit en un langage commun au moteur (NkKey). Le moteur le range
et le distribue aux abonnes. L'application ne connait ni XLookupKeysym
ni Enqueue : elle voit seulement NkKeyPressEvent et son GetKey().

C'est ce qui permet a la meme ligne AddEventCallback<NkKeyPressEvent>
de marcher sur X11, sur Windows, sur Wayland, sans changer. La
traduction est faite une fois, dans chaque backend. Le code commun
n'en sait rien.

Mesure faite le 28/09/2026.
Version de Jenga : 2.8.0.
