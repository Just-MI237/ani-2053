#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkMouseEvent.h"
#include "NKLogger/NkLog.h"

#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstring>

using namespace nkentseu;

int nkmain(const NkEntryState &state) {
    const char *mode = getenv("NK_MODE");
    if (!mode) {
        mode = "none";
    }

    NkWindowConfig cfg;
    cfg.title = "Le glisser qui sort";
    cfg.width = 640;
    cfg.height = 480;
    NkWindow window(cfg);
    if (!window.IsOpen()) {
        return -1;
    }

    logger.Infof("[mode] %s", mode);

    if (strcmp(mode, "capture") == 0) {
        window.CaptureMouse(true);
        logger.Info("[mode] CaptureMouse(true) appele");
    } else if (strcmp(mode, "clip") == 0) {
        window.ClipMouseToClient(true);
        logger.Info("[mode] ClipMouseToClient(true) appele");
    } else {
        logger.Info("[mode] aucune capture");
    }

    NkEventSystem &events = NkEvents();
    bool dragging = false;
    int moveCount = 0;
    bool hasExited = false;

    events.AddEventCallback<NkMouseButtonPressEvent>([&](NkMouseButtonPressEvent *e) {
        if (!e->IsLeft()) {
            return;
        }
        dragging = true;
        moveCount = 0;
        hasExited = false;
        logger.Infof("[appui] client=(%d,%d) ecran=(%d,%d)",
            e->GetX(), e->GetY(), e->GetScreenX(), e->GetScreenY());
    });

    events.AddEventCallback<NkMouseButtonReleaseEvent>([&](NkMouseButtonReleaseEvent *e) {
        if (!e->IsLeft()) {
            return;
        }
        logger.Infof("[relache] client=(%d,%d) ecran=(%d,%d)",
            e->GetX(), e->GetY(), e->GetScreenX(), e->GetScreenY());
        logger.Infof("[bilan] deplacements=%d, sortie de fenetre=%s",
            moveCount, hasExited ? "oui" : "non");
        dragging = false;
    });

    events.AddEventCallback<NkMouseMoveEvent>([&](NkMouseMoveEvent *e) {
        if (!dragging) {
            return;
        }
        moveCount++;
        int x = e->GetX();
        int y = e->GetY();
        auto size = window.GetSize();
        bool outside = (x < 0 || y < 0 || (uint32)x >= size.x || (uint32)y >= size.y);
        if (outside && !hasExited) {
            hasExited = true;
            logger.Infof("[sortie] premier mouvement hors fenetre : client=(%d,%d)", x, y);
        }
        if (moveCount <= 3 || moveCount % 20 == 0 || outside) {
            logger.Infof("[deplacement %d] client=(%d,%d) ecran=(%d,%d) hors=%d",
                moveCount, x, y, e->GetScreenX(), e->GetScreenY(), (int)outside);
        }
    });

    auto start = std::chrono::steady_clock::now();
    while (window.IsOpen()) {
        events.PollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (elapsed >= 30) break;
    }

    logger.Infof("[fin] mode=%s, deplacements=%d, sortie=%s",
        mode, moveCount, hasExited ? "oui" : "non");
    return 0;
}
