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

    // Positions a tester dans l'ordre. La troisieme est hors de l'ecran.
    struct TestPos { int x, y; const char *label; };
    TestPos positions[] = {
        { 2500,  100, "au-dela du bord droit de l'ecran (2500,100)" },
        {  100,  100, "retour en haut-gauche (100,100)" },
        { -500,  100, "au-dela du bord gauche (-500,100)" },
        {  100,  100, "retour en haut-gauche (100,100)" },
    };
    int nextPos = 0;

    auto start = std::chrono::steady_clock::now();
    int lastMoveCheck = 0;
    int lastMonX = -99999, lastMonY = -99999;

    while (window.IsOpen()) {
        NkEvents().PollEvents();

        auto elapsedSec = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - start).count();

        if (nextPos < 4 && elapsedSec >= (nextPos + 1) * 4) {
            const TestPos &p = positions[nextPos];
            fprintf(stderr, "[SetPosition] appel %d : (%d,%d) %s\n",
                nextPos + 1, p.x, p.y, p.label);
            fflush(stderr);
            window.SetPosition(p.x, p.y);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            nextPos++;
        }

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
        if (elapsed >= 25) break;
    }

    fprintf(stderr, "[fin] deplacements detectes : %d\n", lastMoveCheck);
    fflush(stderr);
    return 0;
}
