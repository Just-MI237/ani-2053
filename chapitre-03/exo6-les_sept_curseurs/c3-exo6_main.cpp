#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkMouseEvent.h"
#include "NKLogger/NkLog.h"

#include <thread>
#include <chrono>

using namespace nkentseu;

struct Zone {
    const char *name;
    int x, y, w, h;
};

int nkmain(const NkEntryState &state) {
    NkWindowConfig cfg;
    cfg.title = "Sept curseurs";
    cfg.width = 840;
    cfg.height = 360;
    NkWindow window(cfg);
    if (!window.IsOpen()) {
        return -1;
    }

    Zone zones[7] = {
        {"Arrow",      0,   0, 120, 360},
        {"TextInput",  120, 0, 120, 360},
        {"Hand",       240, 0, 120, 360},
        {"ResizeNS",   360, 0, 120, 360},
        {"ResizeWE",   480, 0, 120, 360},
        {"ResizeNWSE", 600, 0, 120, 360},
        {"ResizeNESW", 720, 0, 120, 360},
    };

    NkEventSystem &events = NkEvents();
    int lastZone = -1;

    events.AddEventCallback<NkMouseMoveEvent>([&](NkMouseMoveEvent *e) {
        int x = e->GetX();
        int y = e->GetY();
        int found = -1;
        for (int i = 0; i < 7; ++i) {
            const Zone &z = zones[i];
            if (x >= z.x && x < z.x + z.w && y >= z.y && y < z.y + z.h) {
                found = i;
                break;
            }
        }
        if (found >= 0 && found != lastZone) {
            lastZone = found;
            logger.Infof("[zone] %s", zones[found].name);
        }
    });

    // Un seul appel au demarrage.
    window.SetCursor(NkWindow::NkCursorType::Arrow);
    logger.Info("[demarrage] SetCursor(Arrow) appele une seule fois");

    auto start = std::chrono::steady_clock::now();
    while (window.IsOpen()) {
        events.PollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (elapsed >= 20) break;
    }

    logger.Infof("[fin] derniere zone survolee : %d", lastZone);
    return 0;
}
