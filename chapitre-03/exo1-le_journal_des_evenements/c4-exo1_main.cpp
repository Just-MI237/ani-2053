#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkEvent.h"
#include <cstdio>
#include <thread>
#include <chrono>
using namespace nkentseu;

int nkmain(const NkEntryState &state) {
    NkWindowConfig cfg;
    cfg.title = "Journal des evenements";
    cfg.width = 640;
    cfg.height = 480;
    NkWindow w(cfg);
    if (!w.IsOpen()) return -1;

    NkEventSystem &ev = NkEvents();
    int total = 0;
    int countThisSecond = 0;
    auto start = std::chrono::steady_clock::now();
    auto lastSecond = start;

    fprintf(stderr, "=== Journal des evenements ===\n");
    fflush(stderr);

    while (w.IsOpen()) {
        while (NkEvent *e = ev.PollEvent()) {
            total++;
            countThisSecond++;
            fprintf(stderr, "[event] %s\n", e->GetTypeStr());
            fflush(stderr);
        }

        auto now = std::chrono::steady_clock::now();
        auto sec = std::chrono::duration_cast<std::chrono::seconds>(now - lastSecond).count();
        if (sec >= 1) {
            fprintf(stderr, "[seconde] %d evenements\n", countThisSecond);
            fflush(stderr);
            countThisSecond = 0;
            lastSecond = now;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));

        auto s = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (s >= 60) break;
    }

    fprintf(stderr, "[fin] total=%d\n", total);
    fflush(stderr);
    return 0;
}
