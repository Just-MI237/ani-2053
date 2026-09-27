#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"

using namespace nkentseu;

int nkmain(const NkEntryState &state) {
    // Fenetre A : taille minimale imposee a 400x300
    NkWindowConfig cfgA;
    cfgA.title = "A - min 400x300";
    cfgA.x = 50;
    cfgA.y = 50;
    cfgA.width = 400;
    cfgA.height = 300;
    cfgA.minWidth = 400;
    cfgA.minHeight = 300;
    NkWindow wa(cfgA);

    // Fenetre B : taille minimale retiree (1x1)
    NkWindowConfig cfgB;
    cfgB.title = "B - min 1x1";
    cfgB.x = 500;
    cfgB.y = 50;
    cfgB.width = 400;
    cfgB.height = 300;
    cfgB.minWidth = 1;
    cfgB.minHeight = 1;
    NkWindow wb(cfgB);

    while (wa.IsOpen() || wb.IsOpen()) { }
    return 0;
}
