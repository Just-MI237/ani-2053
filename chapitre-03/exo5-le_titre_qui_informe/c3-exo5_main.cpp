#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkWindowEvent.h"
#include "NKEvent/NkKeyboardEvent.h"
#include "NKLogger/NkLog.h"

#include <cstdio>
#include <thread>
#include <chrono>

using namespace nkentseu;

int nkmain(const NkEntryState &state) {
    NkWindowConfig cfg;
    cfg.title = "document.txt";
    cfg.width = 1280;
    cfg.height = 720;
    NkWindow window(cfg);
    if (!window.IsOpen()) {
        return -1;
    }

    bool modified = false;
    NkEventSystem& events = NkEvents();
    int updateCount = 0;
    int resizeCount = 0;
    int spaceCount = 0;

    auto updateTitle = [&window, &modified, &updateCount]() {
        auto size = window.GetSize();
        char buf[128];
        if (modified) {
            snprintf(buf, sizeof(buf), "document.txt * - %ux%u", size.x, size.y);
        } else {
            snprintf(buf, sizeof(buf), "document.txt - %ux%u", size.x, size.y);
        }
        window.SetTitle(NkString(buf));
        updateCount++;
        logger.Infof("[titre] appel %d : %s", updateCount, buf);
    };

    events.AddEventCallback<NkWindowResizeEvent>([&](NkWindowResizeEvent* e) {
        resizeCount++;
        logger.Infof("[resize] evenement %d : %ux%u", resizeCount, e->GetWidth(), e->GetHeight());
        updateTitle();
    });

    events.AddEventCallback<NkKeyPressEvent>([&](NkKeyPressEvent* e) {
        if (e->GetKey() == NkKey::NK_SPACE) {
            spaceCount++;
            modified = !modified;
            logger.Infof("[espace] appui %d, modifie=%d", spaceCount, (int)modified);
            updateTitle();
        }
    });

    events.AddEventCallback<NkWindowCloseEvent>([&](NkWindowCloseEvent* e) {
        (void)e;
        logger.Info("[close] demande de fermeture recue");
    });

    updateTitle();

    int frameCount = 0;
    auto start = std::chrono::steady_clock::now();
    while (window.IsOpen()) {
        events.PollEvents();
        frameCount++;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (elapsed >= 15) break;
    }

    logger.Infof("[fin] images=%d, mises a jour titre=%d, resize=%d, espace=%d",
        frameCount, updateCount, resizeCount, spaceCount);

    return 0;
}
