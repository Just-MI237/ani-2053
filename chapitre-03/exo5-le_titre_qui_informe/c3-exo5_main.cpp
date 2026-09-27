#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkWindowEvent.h"
#include "NKEvent/NkKeyboardEvent.h"
#include "NKLogger/NkLog.h"

#include <cstdio>

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

    auto updateTitle = [&window, &modified]() {
        auto size = window.GetSize();
        char buf[128];
        if (modified) {
            snprintf(buf, sizeof(buf), "document.txt * - %ux%u", size.x, size.y);
        } else {
            snprintf(buf, sizeof(buf), "document.txt - %ux%u", size.x, size.y);
        }
        window.SetTitle(NkString(buf));
    };

    events.AddEventCallback<NkWindowResizeEvent>([&](NkWindowResizeEvent* e) {
        (void)e;
        updateTitle();
    });

    events.AddEventCallback<NkKeyPressEvent>([&](NkKeyPressEvent* e) {
        if (e->GetKey() == NkKey::NK_SPACE) {
            modified = !modified;
            updateTitle();
        }
    });

    updateTitle();

    while (window.IsOpen()) {
        events.PollEvents();
    }
    return 0;
}
