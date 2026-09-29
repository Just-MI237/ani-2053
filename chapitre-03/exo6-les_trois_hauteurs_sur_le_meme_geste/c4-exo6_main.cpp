#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkKeyboardEvent.h"
#include "NKEvent/NkEventDispatcher.h"
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

int nkmain(const NkEntryState &state) {
    const char *mode = getenv("NK_MODE");
    if (!mode) mode = "event";

    NkWindowConfig cfg;
    cfg.title = "Trois hauteurs";
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
    NkActionManager actions;
    int x = 50;
    auto t0 = std::chrono::steady_clock::now();

    fprintf(stderr, "=== Mode : %s ===\n", mode);
    fflush(stderr);

    if (strcmp(mode, "event") == 0) {
        ev.AddEventCallback<NkKeyPressEvent>([&](NkKeyPressEvent *e) {
            if (e->GetKey() == NkKey::NK_RIGHT) {
                x += 100;
                fprintf(stderr, "[event] Fleche droite, x=%d\n", x);
                fflush(stderr);
            }
        });
    } else if (strcmp(mode, "state") == 0) {
        actions.CreateAction("Avancer",
            [&](const NkString &, const NkInputCode &, bool p, bool) {
                if (p) { x += 100; }
            });
        actions.AddCommand(NkActionCommand("Avancer", NkInputCode::Key(NkKey::NK_RIGHT)));
    } else {
        actions.CreateAction("Avancer",
            [&](const NkString &, const NkInputCode &, bool p, bool) {
                if (p) { x += 100; }
            });
        actions.AddCommand(NkActionCommand("Avancer", NkInputCode::Key(NkKey::NK_RIGHT)));
    }

    bool last = false;

    while (w.IsOpen()) {
        ev.PollEvents();

        if (strcmp(mode, "state") == 0) {
            bool now = NkInput.IsKeyDown(NkKey::NK_RIGHT);
            if (now && !last) {
                x += 100;
                fprintf(stderr, "[state] front montant, x=%d\n", x);
                fflush(stderr);
            }
            last = now;
        } else if (strcmp(mode, "action") == 0) {
            bool now = NkInput.IsKeyDown(NkKey::NK_RIGHT);
            if (now && !last) {
                actions.TriggerAction(NkInputCode::Key(NkKey::NK_RIGHT), true);
                fprintf(stderr, "[action] Avancer, x=%d\n", x);
                fflush(stderr);
            }
            last = now;
        }

        if (gfx->BeginFrame()) {
            r2d->Clear({30, 30, 40, 255});
            r2d->Begin();
            NkView2D view;
            view.center = {400.f, 200.f};
            view.size = {800.f, 400.f};
            r2d->SetView(view);
            r2d->DrawFilledRect({(float)x, 175.f, 50.f, 50.f}, {80, 180, 220, 255});
            r2d->End();
            gfx->EndFrame();
            gfx->Present();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        auto s = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - t0).count();
        if (s >= 20) break;
    }

    fprintf(stderr, "[fin] mode=%s, position finale x=%d\n", mode, x);
    fflush(stderr);
    NkRenderer2DFactory::Destroy(r2d);
    NkContextFactory::Destroy(gfx);
    return 0;
}
