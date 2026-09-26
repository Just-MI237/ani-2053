#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"

int nkmain(const NkEntryState &state) {
    NkWindowConfig cfg;
    cfg.width = 400;
    cfg.height = 300;

    // Fenetre 1 : resizable desactive
    cfg.title = "resizable=false";
    cfg.x = 50; cfg.y = 50;
    cfg.resizable = false;
    NkWindow w0(cfg);

    // Fenetre 2 : movable desactive
    cfg.title = "movable=false";
    cfg.x = 100; cfg.y = 100;
    cfg.resizable = true;
    cfg.movable = false;
    NkWindow w1(cfg);

    // Fenetre 3 : closable desactive
    cfg.title = "closable=false";
    cfg.x = 150; cfg.y = 150;
    cfg.movable = true;
    cfg.closable = false;
    NkWindow w2(cfg);

    // Fenetre 4 : minimizable desactive
    cfg.title = "minimizable=false";
    cfg.x = 200; cfg.y = 200;
    cfg.closable = true;
    cfg.minimizable = false;
    NkWindow w3(cfg);

    // Fenetre 5 : maximizable desactive
    cfg.title = "maximizable=false";
    cfg.x = 250; cfg.y = 250;
    cfg.minimizable = true;
    cfg.maximizable = false;
    NkWindow w4(cfg);

    // Fenetre 6 : canFullscreen desactive
    cfg.title = "canFullscreen=false";
    cfg.x = 300; cfg.y = 300;
    cfg.maximizable = true;
    cfg.canFullscreen = false;
    NkWindow w5(cfg);

    // Fenetre 7 : frame desactive
    cfg.title = "frame=false";
    cfg.x = 350; cfg.y = 350;
    cfg.canFullscreen = true;
    cfg.frame = false;
    NkWindow w6(cfg);

    while (w0.IsOpen() || w1.IsOpen() || w2.IsOpen() || w3.IsOpen() ||
           w4.IsOpen() || w5.IsOpen() || w6.IsOpen()) { }
    return 0;
}
