Fonctions de l'interface publique sans implementation sur certaines
plateformes

Ce document regroupe huit observations faites au cours du Sprint 3.
Chacune a la meme forme : une fonction de l'interface publique du
moteur promet quelque chose qu'une plateforme donnee ne tient pas, et
rien ne le signale. Pas d'erreur, pas d'avertissement, pas de valeur
de retour fausse. Le silence.

Pour chacune, la fonction, la plateforme concernee, le fichier source
et la ligne exacte. Toutes les observations ont ete faites sur le
commit 9c3fad33 du 2026-09-13, avec Jenga 2.8.0.

1. XSetWMNormalHints : l'indice de taille minimale

Fonction concernee : NkWindow::Create, qui pose les tailles minimales
d'une fenetre avec cfg.minWidth et cfg.minHeight.
Plateforme : WSLg (X11, gestionnaire de fenetres minimal).
Fichier : Kernel/Runtime/NKWindow/src/NKWindow/Platform/XLib/NkXLibWindow.cpp
Ligne : 384

    XSetWMNormalHints(sDisplay, mData.mXid, &hints);

XSetWMNormalHints envoie un indice au gestionnaire de fenetres. Le mot
hints veut dire indice, pas ordre. Le gestionnaire de WSLg l'ignore.
Resultat mesure : une fenetre dont la taille minimale est fixee a
400x300 descend jusqu'a 1x1. Sur GNOME ou KDE, qui respectent l'indice,
la fenetre refuserait de descendre sous 400x300.

2. DisplayWidthMM : le facteur d'echelle quand XRandR renvoie 0

Fonction concernee : NkWindow::GetDpiScale, qui calcule le facteur
d'echelle depuis la taille physique de l'ecran.
Plateforme : tout serveur X ou XRandR ne renseigne pas la taille
physique (cas observe sur Xvfb :98 et sur WSLg).
Fichier : Kernel/Runtime/NKWindow/src/NKWindow/Platform/XLib/NkXLibWindow.cpp
Lignes : 610 et 643

Ligne 610, dans XLibFillDisplayInfoFromCrtc (fonction principale) :

    int fbW = DisplayWidthMM(display, screen);

Ligne 643, dans XLibFallbackDisplayInfo (fonction de secours) :

    int mmW = DisplayWidthMM(display, screen);

La fonction principale interroge XRandR en premier. XRandR renvoie 0mm.
Elle garde alors 96 dpi et un facteur d'echelle de 1.0. La fonction de
secours interroge Xlib (DisplayWidthMM) et obtient la vraie valeur.
J'ai propose un correctif qui ajoute la chute sur DisplayWidthMM dans
la fonction principale. Sur Xvfb a 192 dpi, le facteur passe de
1.000000 a 2.000000. Sur WSLg, il reste a 1.000000 parce que WSLg
declare 96 dpi, ce qui est coherent.

3. SetCursor : le curseur qui ne change pas

Fonction concernee : NkWindow::SetCursor, qui pose la forme du curseur
dans la zone client.
Plateforme : Linux/X11 (et les autres plateformes non Windows).
Fichier : Kernel/Runtime/NKWindow/src/NKWindow/Core/NkWindowCursor.cpp
Ligne : 56

    void NkWindow::SetCursor(NkCursorType /*cursor*/) {
        // Plateformes sans curseur souris (Android, iOS, Web tactile, headless).
    }

Le parametre est commente. La fonction ne fait rien. La version Win32,
lignes 14 a 52 du meme fichier, traduit NkCursorType en IDC_* et
appelle ::SetCursor. Resultat mesure : dans les sept zones d'un
programme qui appelle SetCursor a chaque changement, le curseur reste
une fleche sur WSLg. Le commentaire cite Android, iOS, Web tactile et
headless. Linux avec X11 n'est pas dans cette liste. Le backend XLib
n'implemente pas le curseur, alors que X11 fournit XCreateFontCursor
et XDefineCursor.

4. CaptureMouse : la capture de souris vide

Fonction concernee : NkWindow::CaptureMouse, qui demande a recevoir les
evenements souris meme quand le curseur sort de la fenetre.
Plateforme : Linux/X11.
Fichier : Kernel/Runtime/NKWindow/src/NKWindow/Platform/XLib/NkXLibWindow.cpp
Ligne : 1188

    void NkWindow::CaptureMouse(bool) {
    }

Le parametre est commente (bool sans nom). La fonction ne fait rien.
Juste en dessous, lignes 1189 a 1204 du meme fichier, une fonction
voisine fait le travail :

    void NkWindow::ClipMouseToClient(bool clip) {
        ...
        XGrabPointer(mData.mDisplay, mData.mXid, True, ...);
        ...
    }

L'appel XGrabPointer avec confine_to=window empeche le pointeur de
sortir de la fenetre. Resultat mesure : sans clip, la souris sort
librement en continuant d'alimenter le programme. Avec clip, elle bute
au bord de la fenetre. CaptureMouse, elle, ne produit rien.

5. NkMouseDoubleClickEvent : le double-clic non detecte

Fonction concernee : NkMouseDoubleClickEvent, la classe d'evenement qui
signale un double-clic.
Plateforme : Linux/X11.
Fichier : Kernel/Runtime/NKEvent/src/NKEvent/NkMouseEvent.h
Ligne : 684

    class NKENTSEU_EVENT_CLASS_EXPORT NkMouseDoubleClickEvent final : public NkMouseButtonEvent {

La classe existe. Elle est declaree finale et a son GetStaticType. On
peut s'y abonner avec AddEventCallback. Mais le backend X11 ne la
produit jamais. Le champ GetClickCount() de NkMouseButtonEvent reste
a 1 apres deux appuis consecutifs. Resultat mesure : dans un programme
qui enregistre un callback sur NkMouseDoubleClickEvent et un log de
clickCount sur chaque appui, le callback n'est jamais appele et le
clickCount ne passe jamais a 2.

6. Presse-papiers interne hors Win32

Fonctions concernees : NkWindow::SetClipboardText, GetClipboardText,
SetClipboardImage, GetClipboardImage, HasClipboardImage.
Plateforme : Linux/X11.
Fichier : Kernel/Runtime/NKWindow/src/NKWindow/Core/NkWindow.h
Lignes : 155 et 225

Ligne 155 :

    // plateformes = fallback interne a l'application (copier/coller intra-app).

Ligne 225 :

    // quand la source le fournit) ; autres plateformes = fallback interne

Hors du bureau Win32, le presse-papiers de NkWindow n'est pas le
presse-papiers du systeme. C'est un tampon interne a l'application.
Resultat mesure : avec le texte "test externe" et une capture d'ecran
copies dans le presse-papiers Windows avant de lancer le programme, le
programme lit une chaine vide et aucune image. Le cycle ecriture-lecture
fonctionne a l'interieur du programme, mais rien ne vient du dehors et
rien ne sort.

7. MakeCurrent : un seul contexte rend sans lui

Fonctions concernees : NkWindow::MakeCurrent et ReleaseCurrent, qui
designent le contexte graphique courant.
Plateforme : tout systeme ou deux contextes graphiques coexistent dans
la meme boucle.
Fichier : Kernel/Runtime/NKCanvas/src/NKCanvas/Core/NkIGraphicsContext.h
Lignes : 49 et 53

Ligne 49 :

    virtual bool MakeCurrent() { return true; }

Ligne 53 :

    virtual void ReleaseCurrent() { }

Un contexte OpenGL est une ressource par thread. A un instant donne,
un seul contexte est courant. Resultat mesure : avec deux fenetres et
deux contextes, sans appel a MakeCurrent, une seule fenetre rend (la
derniere dont le contexte a ete cree). Avec MakeCurrent avant chaque
BeginFrame et ReleaseCurrent apres chaque Present, les deux fenetres
rendent. Il faut un MakeCurrent explicite par fenetre et par frame.

8. GetClipboardImage : deux surcharges, dont une seulement sur Win32

Fonction concernee : NkWindow::GetClipboardImage.
Plateforme : la version du commit 9c3fad33 que j'utilise, et celle
plus recente de origin/main (465b791c).
Fichier : Kernel/Runtime/NKWindow/src/NKWindow/Core/NkWindow.h

Dans le commit 9c3fad33 que j'utilise, il n'y a qu'une version :

    Ligne 240 :
    bool GetClipboardImage(NkClipboardImage &out) const;

Commande :

    grep -rn "GetClipboardImage" Kernel/Runtime/NKWindow/src/

Sortie brute :

    NkWindowClipboardImage.cpp:36:
    bool NkWindow::GetClipboardImage(NkClipboardImage &out) const {
    NkWindow.h:240:
    bool GetClipboardImage(NkClipboardImage &out) const;
    NkWin32Window.cpp:1283:
    bool NkWindow::GetClipboardImage(NkClipboardImage &out) const;

Une seule signature.

Dans la version plus recente du depot, 465b791c, il y en a deux :

    Ligne 176 :
    bool GetClipboardImage(NkVector<uint8> &rgba, int32 &w, int32 &h,
                           NkString &motif) const;

    Ligne 292 :
    bool GetClipboardImage(NkClipboardImage &out) const;

Ma reponse a la question posee

J'ai appele la version qui prend NkClipboardImage&. Sur le commit que
j'utilise, c'est la seule qui existe. Sur la version plus recente du
depot, la seconde (celle a quatre arguments) est implementee dans
NkWin32Window.cpp, ligne 1264. Son code fait de vraies operations :
IsClipboardFormatAvailable, OpenClipboard, GetClipboardData, lecture
du BITMAPINFOHEADER, et rend les pixels.

Mais dans la version 465b791c, cette version a quatre arguments n'est
pas dans le fichier de repli multiplateforme
NkWindowClipboardImage.cpp. Elle n'est compilee que sur Win32. Le
professeur l'a signalee comme un piege : une fonction qui rend faux
sans rien essayer. C'est exact pour cette surcharge : sur les
plateformes non-Windows, elle n'existe pas.

Dans mon commit 9c3fad33, le piege n'existe pas encore, parce que la
surcharge a quatre arguments n'y est pas presente.

Ce que ces huit cas ont en commun

Les huit fonctions ont la meme structure. L'interface publique du
moteur promet quelque chose. Une plateforme donnee ne le tient pas. Et
rien ne le signale. Pas d'erreur au demarrage, pas d'avertissement a
l'appel, pas de valeur de retour fausse. L'appelant croit avoir
demande quelque chose. Il n'a rien demande du tout.

La question qui reste ouverte

Quand une fonction de l'interface publique d'un moteur n'a pas
d'implementation sur la plateforme courante, trois choix sont
possibles.

Le premier est de se taire, comme aujourd'hui. L'appelant ne sait rien.
C'est simple a implementer, mais c'est ce qui produit les huit cas
ci-dessus.

Le deuxieme est de rendre faux. SetCursor retournerait un bool, et
l'appelant saurait que l'appel n'a pas eu lieu. Mais SetCursor est
declaree void dans l'interface actuelle. Changer le type de retour
casserait tous les appelants existants.

Le troisieme est de rediriger. CaptureMouse pourrait appeler
ClipMouseToClient sur les plateformes ou la premiere n'existe pas.
C'est ce qui marcherait le mieux pour l'utilisateur, mais c'est un
choix qui n'a pas toujours de sens (le comportement n'est pas le
meme), et il faut le prendre fonction par fonction.

Aucun des trois n'est evidemment bon. Chacun a un cout. C'est une
decision d'architecture, pas de code.

Mesure faite le 28/09/2026.
Version de Jenga : 2.8.0.
Commit Nkentseu : 9c3fad33.
