// Exercice 1 du chapitre 4 - La fenetre nue.
//
// Le plus petit programme NKCanvas : une classe derivee de
// NkCanvasApp, la configuration posee dans le constructeur, et le
// point d'entree nkmain qui appelle NkCanvasApp::Run.

#include "NKWindow/NKMain.h"
#include "NKCanvas/App/NkCanvasApp.h"

using namespace nkentseu;
using namespace nkentseu::renderer;

class FenetreNue : public NkCanvasApp
{
    public:
        FenetreNue()
        {
            Config().title = "La fenetre nue";
            Config().width = 800;
            Config().height = 600;
            Config().clearColor = NkColor2D{18, 18, 24, 255};
        }
};

int nkmain(const NkEntryState &state)
{
    return NkCanvasApp::Run<FenetreNue>(state);
}
