#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkWindowEvent.h"
#include "NKLogger/NkLog.h"

#include <thread>
#include <chrono>

using namespace nkentseu;

int nkmain(const NkEntryState &state) {
    NkWindowConfig cfgA;
    cfgA.title = "A-min-400x300";
    cfgA.x = 50;
    cfgA.y = 50;
    cfgA.width = 400;
    cfgA.height = 300;
    cfgA.minWidth = 400;
    cfgA.minHeight = 300;
    NkWindow wa(cfgA);

    NkWindowConfig cfgB;
    cfgB.title = "B-min-1x1";
    cfgB.x = 500;
    cfgB.y = 50;
    cfgB.width = 400;
    cfgB.height = 300;
    cfgB.minWidth = 1;
    cfgB.minHeight = 1;
    NkWindow wb(cfgB);

    NkWindowId idA = wa.GetId();
    NkWindowId idB = wb.GetId();

    NkEventSystem& events = NkEvents();
    int resizeA = 0;
    int resizeB = 0;
    uint32 minWA = 9999, minHA = 9999;
    uint32 minWB = 9999, minHB = 9999;

    events.AddEventCallback<NkWindowResizeEvent>([&](NkWindowResizeEvent* e) {
        uint32 w = e->GetWidth();
        uint32 h = e->GetHeight();
        NkWindowId wid = e->GetWindowId();
        if (wid == idA) {
            resizeA++;
            if (w < minWA) minWA = w;
            if (h < minHA) minHA = h;
            logger.Infof("[A] resize %d : %ux%u (min vu : %ux%u)", resizeA, w, h, minWA, minHA);
        } else if (wid == idB) {
            resizeB++;
            if (w < minWB) minWB = w;
            if (h < minHB) minHB = h;
            logger.Infof("[B] resize %d : %ux%u (min vu : %ux%u)", resizeB, w, h, minWB, minHB);
        }
    });

    auto start = std::chrono::steady_clock::now();
    while (wa.IsOpen() || wb.IsOpen()) {
        events.PollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (elapsed >= 20) break;
    }

    logger.Infof("[fin] A : %d resizes, plus petite taille %ux%u", resizeA, minWA, minHA);
    logger.Infof("[fin] B : %d resizes, plus petite taille %ux%u", resizeB, minWB, minHB);

    return 0;
}
