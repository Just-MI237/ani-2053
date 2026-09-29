#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkKeyboardEvent.h"
#include "NKCanvas/Factory/NkContextFactory.h"
#include "NKCanvas/Core/NkContextDesc.h"
#include "NKCanvas/Core/NkIGraphicsContext.h"
#include "NKCanvas/Renderer/Core/NkIRenderer2D.h"
#include "NKCanvas/Renderer/Core/NkRenderer2DFactory.h"
#include "NKCanvas/Renderer/Core/NkRenderer2DTypes.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <chrono>
using namespace nkentseu;
using namespace nkentseu::renderer;

static const float SOL = 300.f;
static const float GRAVITE = 900.f;
static const float FORCE_SAUT = -420.f;
static const long DEBOUNCE_MS = 200;

int nkmain(const NkEntryState &state) {
    const char *mode = getenv("NK_MODE");
    if (!mode) mode = "event";

    NkWindowConfig cfg;
    cfg.title = "Le front montant";
    cfg.width = 800;
    cfg.height = 400;
    NkWindow w(cfg);
    if (!w.IsOpen()) return -1;

    NkContextDesc desc;
    desc.api = NkGraphicsApi::NK_GFX_API_OPENGL;
    desc.opengl.majorVersion = 3;
    desc.opengl.minorVersion = 3;
    desc.opengl.profile = NkGLProfile::Core;
    NkIGraphicsContext *gfx = NkContextFactory::Create(w, desc);
    if (!gfx) return -2;
    NkIRenderer2D *r2d = NkRenderer2DFactory::Create(gfx);
    if (!r2d) { NkContextFactory::Destroy(gfx); return -3; }

    NkEventSystem &ev = NkEvents();
    float y = SOL;
    float vy = 0.f;
    int sauts = 0;
    bool lastState = false;
    long dernierSaut = -10000;

    fprintf(stderr, "=== Mode : %s ===\n", mode);
    fflush(stderr);

    auto t0 = std::chrono::steady_clock::now();

    if (strcmp(mode, "event") == 0) {
        ev.AddEventCallback<NkKeyPressEvent>([&](NkKeyPressEvent *e) {
            if (e->GetKey() != NkKey::NK_SPACE) return;
            long ms = (long)std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - t0).count();
            if (ms - dernierSaut < DEBOUNCE_MS) {
                fprintf(stderr, "[event] ignore (auto-repeat a %ld ms)\n", ms);
                fflush(stderr);
                return;
            }
            dernierSaut = ms;
            vy = FORCE_SAUT;
            sauts++;
            fprintf(stderr, "[event] saut %d a %ld ms\n", sauts, ms);
            fflush(stderr);
        });
    }

    auto last = std::chrono::steady_clock::now();

    while (w.IsOpen()) {
        ev.PollEvents();
        auto now = std::chrono::steady_clock::now();
        float dt = std::chrono::duration<float>(now - last).count();
        last = now;
        if (dt > 0.05f) dt = 0.05f;

        if (strcmp(mode, "state") == 0) {
            bool nowDown = NkInput.IsKeyDown(NkKey::NK_SPACE);
            if (nowDown && !lastState) {
                long ms = (long)std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - t0).count();
                if (ms - dernierSaut >= DEBOUNCE_MS) {
                    dernierSaut = ms;
                    vy = FORCE_SAUT;
                    sauts++;
                    fprintf(stderr, "[state-front] saut %d a %ld ms\n", sauts, ms);
                    fflush(stderr);
                }
            }
            lastState = nowDown;
        } else if (strcmp(mode, "state_no_edge") == 0) {
            if (NkInput.IsKeyDown(NkKey::NK_SPACE)) {
                vy = FORCE_SAUT;
                sauts++;
                if (sauts <= 5 || sauts % 30 == 0) {
                    fprintf(stderr, "[state-no-edge] saut %d\n", sauts);
                    fflush(stderr);
                }
            }
        }

        vy += GRAVITE * dt;
        y += vy * dt;
        if (y >= SOL) { y = SOL; vy = 0.f; }

        if (gfx->BeginFrame()) {
            r2d->Clear({30, 30, 40, 255});
            r2d->Begin();
            NkView2D view;
            view.center = {400.f, 200.f};
            view.size = {800.f, 400.f};
            r2d->SetView(view);

            r2d->DrawFilledRect({0.f, 345.f, 800.f, 5.f}, {80, 80, 90, 255});
            r2d->DrawFilledRect({375.f, y, 50.f, 50.f}, {80, 180, 220, 255});

            r2d->End();
            gfx->EndFrame();
            gfx->Present();
        }

        auto s = std::chrono::duration_cast<std::chrono::seconds>(now - t0).count();
        if (s >= 30) break;

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    fprintf(stderr, "[fin] mode=%s, sauts=%d\n", mode, sauts);
    fflush(stderr);
    NkRenderer2DFactory::Destroy(r2d);
    NkContextFactory::Destroy(gfx);
    return 0;
}
