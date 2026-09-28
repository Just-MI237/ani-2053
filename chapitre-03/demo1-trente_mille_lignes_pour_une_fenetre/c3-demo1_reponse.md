Demonstration 1 - Trente mille lignes pour une fenetre

J'ai ouvert l'arborescence du module NKWindow et j'ai mesure trois
choses : sa taille, ses backends, et la difference entre deux
implementations d'un meme appel.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Un. Taille du module

Les commandes :

    cd ~/Projets/Nkentseu/Kernel/Runtime/NKWindow
    find . -type f | wc -l
    find . -name "*.cpp" | wc -l
    find . -name "*.h" | wc -l
    find . -type f \( -name "*.cpp" -o -name "*.h" \) -exec wc -l {} + | tail -1

Sortie brute :

    === Nombre de fichiers ===
    127
    === Nombre de fichiers .cpp ===
    35
    === Nombre de fichiers .h ===
    74
    === Nombre de lignes total ===
      30621 total

Le module NKWindow fait 30621 lignes, reparties sur 35 fichiers .cpp
et 74 fichiers .h. Il y a 127 fichiers en tout, ce qui inclut les
fichiers Java, TypeScript et autres.

Deux. Les backends de plateforme

Commande :

    cd ~/Projets/Nkentseu/Kernel/Runtime/NKWindow/src/NKWindow/Platform
    find . -name "*Window.cpp" -o -name "*Window.mm" | sort

Sortie brute :

    ./Android/NkAndroidWindow.cpp
    ./Cocoa/NkCocoaWindow.mm
    ./Emscripten/NkEmscriptenWindow.cpp
    ./HarmonyOS/NkHarmonyWindow.cpp
    ./Noop/NkNoopWindow.cpp
    ./UIKit/NkUIKitWindow.mm
    ./UWP/NkUWPWindow.cpp
    ./Wayland/NkWaylandWindow.cpp
    ./Win32/NkWin32Window.cpp
    ./XCB/NkXCBWindow.cpp
    ./XLib/NkXLibWindow.cpp
    ./Xbox/NkXboxWindow.cpp

Il y a douze backends de fenetre. Chacun a un fichier NkXxxWindow
dedie. Le dossier Platform contient aussi trois dossiers qui ne sont
pas des backends de fenetre : Common (un seul fichier d'en-tete
NkSystemMemory.h), Linux (un seul fichier NkLinuxGamepadBackend.h), et
les sous-dossiers Java de Android.

Trois. Un appel, deux implementations

J'ai choisi NkWindow::SetTitle. C'est un appel court, present dans tous
les backends, et sa difference entre XLib et Win32 est lisible en
quelques lignes.

Implementation XLib, ligne 776 de NkXLibWindow.cpp :

    void NkWindow::SetTitle(const NkString &title) {
            mConfig.title = title;
            if (mData.mDisplay && mData.mXid) {
                    XStoreName(mData.mDisplay, mData.mXid, title.CStr());
            }
    }

Implementation Win32, ligne 724 de NkWin32Window.cpp :

    void NkWindow::SetTitle(const NkString &t) {
            mConfig.title = t;
            if (mData.mHwnd) {
                    SetWindowTextW(mData.mHwnd, NkUtf8ToWide(t).CStr());
                    // La synchronisation est deja faite via la modification de mConfig
            }
    }

Qu'est-ce qui est identique

Le contrat est le meme dans les deux cas. Le titre recu est stocke
dans mConfig.title, ce qui met a jour l'etat interne du moteur. Ensuite
le code verifie que la ressource native existe avant d'agir dessus :
mData.mDisplay et mData.mXid pour X11, mData.mHwnd pour Windows. Puis
il appelle l'API systeme pour changer le titre de la fenetre reelle.
La fonction retourne void dans les deux cas : elle ne signale ni
succes ni echec, et ne peut pas en signaler.

Qu'est-ce qui change

Trois choses. La premiere est l'appel systeme : XStoreName pour X11,
SetWindowTextW pour Windows. La deuxieme est la conversion de chaine :
aucune cote X11, parce que XStoreName accepte une chaine UTF-8
directement, mais une conversion UTF-8 vers UTF-16 cote Win32, parce
que SetWindowTextW travaille en wide char et que NkString est en
UTF-8. La troisieme est le nombre de verifications prealables : deux
conditions cote X11 (le display et la fenetre native), une seule cote
Win32 (le handle de fenetre).

Ce que le module absorbe

Le module absorbe la difference entre les API natives de gestion de
fenetre. L'appelant ecrit SetTitle une seule fois, et le module choisit
l'appel systeme selon la plateforme, fait les conversions de chaine
necessaires, et verifie que la ressource native existe avant d'agir.
C'est ce qui permet au meme code d'application de fonctionner sur X11
et sur Windows sans connaitre ni XStoreName ni SetWindowTextW.

Mesure faite le 28/09/2026.
Version de Jenga : 2.8.0.
