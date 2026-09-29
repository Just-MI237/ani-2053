#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkKeyboardEvent.h"
#include "NKEvent/NkEventDispatcher.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <chrono>
#include <vector>
using namespace nkentseu;

struct Entree { long ms; int code; };

static long ElapsedMs(std::chrono::steady_clock::time_point t0) {
    return (long)std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - t0).count();
}

int nkmain(const NkEntryState &state) {
    const char *mode = getenv("NK_MODE");
    if (!mode) mode = "record";
    bool recording = (strcmp(mode, "record") == 0);

    NkWindowConfig cfg;
    cfg.title = "Le rejeu";
    cfg.width = 400;
    cfg.height = 200;
    NkWindow w(cfg);
    if (!w.IsOpen()) return -1;

    NkEventSystem &ev = NkEvents();
    NkActionManager actions;
    std::vector<Entree> journal;
    int position = 0;
    auto t0 = std::chrono::steady_clock::now();

    fprintf(stderr, "=== Mode : %s ===\n", mode);
    fflush(stderr);

    actions.CreateAction("Avancer", [&](const NkString &n, const NkInputCode &, bool p, bool) {
        if (!p) return;
        position += 1;
        fprintf(stderr, "[%s] %s, position=%d\n", mode, n.CStr(), position);
        fflush(stderr);
    });
    actions.CreateAction("Reculer", [&](const NkString &n, const NkInputCode &, bool p, bool) {
        if (!p) return;
        position -= 1;
        fprintf(stderr, "[%s] %s, position=%d\n", mode, n.CStr(), position);
        fflush(stderr);
    });

    actions.AddCommand(NkActionCommand("Avancer", NkInputCode::Key(NkKey::NK_RIGHT)));
    actions.AddCommand(NkActionCommand("Reculer", NkInputCode::Key(NkKey::NK_LEFT)));

    if (recording) {
        ev.AddEventCallback<NkKeyPressEvent>([&](NkKeyPressEvent *e) {
            int k = (int)e->GetKey();
            if (k != (int)NkKey::NK_RIGHT && k != (int)NkKey::NK_LEFT) return;
            long ms = ElapsedMs(t0);
            journal.push_back({ms, k});
            actions.TriggerAction(NkInputCode::Key(e->GetKey()), true);
        });

        while (w.IsOpen()) {
            ev.PollEvents();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            if (ElapsedMs(t0) >= 60000) break;
        }

        FILE *f = fopen("/tmp/rejeu.txt", "w");
        for (auto &e : journal) fprintf(f, "%ld %d\n", e.ms, e.code);
        fclose(f);
        fprintf(stderr, "[fin record] entrees=%d, position finale=%d\n",
            (int)journal.size(), position);
    } else {
        FILE *f = fopen("/tmp/rejeu.txt", "r");
        if (!f) { fprintf(stderr, "aucun journal\n"); return -1; }
        long ms; int k;
        while (fscanf(f, "%ld %d", &ms, &k) == 2) journal.push_back({ms, k});
        fclose(f);
        fprintf(stderr, "[replay] journal charge : %d entrees\n", (int)journal.size());

        usize i = 0;
        while (w.IsOpen() && i < journal.size()) {
            long elapsed = ElapsedMs(t0);
            while (i < journal.size() && journal[i].ms <= elapsed) {
                actions.TriggerAction(NkInputCode::Key((NkKey)journal[i].code), true);
                i++;
            }
            ev.PollEvents();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        fprintf(stderr, "[fin replay] entrees=%d, position finale=%d\n",
            (int)journal.size(), position);
    }
    return 0;
}
