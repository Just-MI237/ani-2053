#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkKeyboardEvent.h"
#include "NKEvent/NkEventDispatcher.h"
#include <cstdio>
#include <thread>
#include <chrono>
using namespace nkentseu;

int nkmain(const NkEntryState &state) {
    const char *mode = getenv("NK_MODE");
    if (!mode) mode = "event";

    NkWindowConfig cfg;
    cfg.title = "Trois hauteurs";
    cfg.width = 400;
    cfg.height = 200;
    NkWindow w(cfg);
    if (!w.IsOpen()) return -1;

    NkEventSystem &ev = NkEvents();
    NkActionManager actions;
    int position = 0;
    auto t0 = std::chrono::steady_clock::now();

    fprintf(stderr, "=== Mode : %s ===\n", mode);
    fflush(stderr);

    if (strcmp(mode, "event") == 0) {
        ev.AddEventCallback<NkKeyPressEvent>([&](NkKeyPressEvent *e) {
            if (e->GetKey() == NkKey::NK_RIGHT) {
                position += 1;
                fprintf(stderr, "[event] Fleche droite recue, position=%d\n", position);
                fflush(stderr);
            }
        });

        while (w.IsOpen()) {
            ev.PollEvents();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            auto s = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - t0).count();
            if (s >= 15) break;
        }
    } else if (strcmp(mode, "state") == 0) {
        bool last = false;
        while (w.IsOpen()) {
            ev.PollEvents();
            bool now = NkInput.IsKeyDown(NkKey::NK_RIGHT);
            if (now && !last) {
                position += 1;
                fprintf(stderr, "[state] front montant, position=%d\n", position);
                fflush(stderr);
            }
            last = now;
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            auto s = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - t0).count();
            if (s >= 15) break;
        }
    } else {
        actions.CreateAction("Avancer",
            [&](const NkString &n, const NkInputCode &, bool p, bool) {
                if (!p) return;
                position += 1;
                fprintf(stderr, "[action] %s, position=%d\n", n.CStr(), position);
                fflush(stderr);
            });
        actions.AddCommand(NkActionCommand("Avancer", NkInputCode::Key(NkKey::NK_RIGHT)));

        bool last = false;
        while (w.IsOpen()) {
            ev.PollEvents();
            bool now = NkInput.IsKeyDown(NkKey::NK_RIGHT);
            if (now && !last) {
                actions.TriggerAction(NkInputCode::Key(NkKey::NK_RIGHT), true);
            }
            last = now;
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            auto s = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - t0).count();
            if (s >= 15) break;
        }
    }

    fprintf(stderr, "[fin] mode=%s, position finale=%d\n", mode, position);
    fflush(stderr);
    return 0;
}
