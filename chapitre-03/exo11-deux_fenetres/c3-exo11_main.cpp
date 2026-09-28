#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkMouseEvent.h"

#include <cstdio>
#include <thread>
#include <chrono>

using namespace nkentseu;

int nkmain(const NkEntryState &state) {
    NkWindowConfig cfgA;
    cfgA.title = "Fenetre A";
    cfgA.x = 100;
    cfgA.y = 100;
    cfgA.width = 400;
    cfgA.height = 300;
    NkWindow wa(cfgA);

    NkWindowConfig cfgB;
    cfgB.title = "Fenetre B";
    cfgB.x = 600;
    cfgB.y = 100;
    cfgB.width = 400;
    cfgB.height = 300;
    NkWindow wb(cfgB);

    if (!wa.IsOpen() || !wb.IsOpen()) {
        fprintf(stderr, "Une des deux fenetres ne s'est pas ouverte\n");
        return -1;
    }

    NkWindowId idA = wa.GetId();
    NkWindowId idB = wb.GetId();
    fprintf(stderr, "idA=%llu idB=%llu\n", (unsigned long long)idA, (unsigned long long)idB);
    fflush(stderr);

    NkEventSystem &events = NkEvents();

    events.AddEventCallback<NkMouseButtonPressEvent>([&](NkMouseButtonPressEvent *e) {
        if (!e->IsLeft()) return;
        uint64 id = e->GetWindowId();
        const char *qui = "inconnue";
        if (id == idA) qui = "A";
        else if (id == idB) qui = "B";
        fprintf(stderr, "[clic] fenetre %s (id=%llu) at client=(%d,%d)\n",
            qui, (unsigned long long)id, e->GetX(), e->GetY());
        fflush(stderr);
    });

    auto start = std::chrono::steady_clock::now();
    while (wa.IsOpen() || wb.IsOpen()) {
        events.PollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (elapsed >= 30) break;
    }

    fprintf(stderr, "[fin]\n"); fflush(stderr);
    return 0;
}
