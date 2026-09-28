#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkSystemEvent.h"

#include <cstdio>
#include <thread>
#include <chrono>

using namespace nkentseu;

static void AfficheEcran(const NkDisplayInfo &d, int index) {
    fprintf(stderr, "Ecran %d :\n", index);
    fprintf(stderr, "  nom          = \"%s\"\n", d.name);
    fprintf(stderr, "  taille       = %u x %u (logique)\n", d.width, d.height);
    fprintf(stderr, "  taille phy   = %u x %u\n", d.physWidth, d.physHeight);
    fprintf(stderr, "  position     = (%d, %d)\n", d.posX, d.posY);
    fprintf(stderr, "  dpiScale     = %f\n", d.dpiScale);
    fprintf(stderr, "  dpiX/dpiY    = %f / %f\n", d.dpiX, d.dpiY);
    fprintf(stderr, "  refresh      = %u Hz\n", d.refreshRate);
    fprintf(stderr, "  primaire     = %d\n", (int)d.isPrimary);
    fflush(stderr);
}

int nkmain(const NkEntryState &state) {
    NkWindowConfig cfg;
    cfg.title = "Inventaire des ecrans";
    cfg.width = 500;
    cfg.height = 400;
    cfg.x = 200;
    cfg.y = 200;
    NkWindow window(cfg);
    if (!window.IsOpen()) {
        fprintf(stderr, "Fenetre non ouverte\n");
        return -1;
    }

    fprintf(stderr, "=== Inventaire des ecrans ===\n");
    fflush(stderr);

    NkVector<NkDisplayInfo> mons = window.EnumerateMonitors();
    fprintf(stderr, "Nombre d'ecrans : %d\n", (int)mons.Size());
    fflush(stderr);

    for (usize i = 0; i < mons.Size(); ++i) {
        AfficheEcran(mons[i], (int)i);
    }

    NkDisplayInfo cur = window.GetCurrentMonitor();
    fprintf(stderr, "=== Ecran portant la fenetre ===\n");
    AfficheEcran(cur, -1);

    auto start = std::chrono::steady_clock::now();
    int lastMoveCheck = 0;
    int lastMonX = -99999, lastMonY = -99999;

    while (window.IsOpen()) {
        NkEvents().PollEvents();

        NkDisplayInfo mon = window.GetCurrentMonitor();
        if (mon.posX != lastMonX || mon.posY != lastMonY) {
            lastMonX = mon.posX;
            lastMonY = mon.posY;
            auto pos = window.GetPosition();
            fprintf(stderr, "[changement d'ecran] fenetre=(%d,%d), nouvel ecran : nom=\"%s\" pos=(%d,%d) dpiScale=%f\n",
                (int)pos.x, (int)pos.y, mon.name, mon.posX, mon.posY, mon.dpiScale);
            fflush(stderr);
            lastMoveCheck++;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(32));

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (elapsed >= 45) break;
    }

    fprintf(stderr, "[fin] deplacements detectes : %d\n", lastMoveCheck);
    fflush(stderr);
    return 0;
}
