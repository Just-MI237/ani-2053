#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkMouseEvent.h"

#include "NKCanvas/Factory/NkContextFactory.h"
#include "NKCanvas/Core/NkContextDesc.h"
#include "NKCanvas/Core/NkIGraphicsContext.h"
#include "NKCanvas/Renderer/Core/NkIRenderer2D.h"
#include "NKCanvas/Renderer/Core/NkRenderer2DFactory.h"
#include "NKCanvas/Renderer/Core/NkRenderer2DTypes.h"

#include <cstdio>
#include <thread>
#include <chrono>

using namespace nkentseu;
using namespace nkentseu::renderer;

int nkmain(const NkEntryState &state) {
    NkWindowConfig cfgA;
    cfgA.title = "Fenetre A";
    cfgA.x = 100;
    cfgA.y = 100;
    cfgA.width = 400;
    cfgA.height = 300;
    NkWindow wa(cfgA);

    NkWindowConfig cfgB;
    cfgB.title = "Fenetre B";
    cfgB.x = 600;
    cfgB.y = 100;
    cfgB.width = 400;
    cfgB.height = 300;
    NkWindow wb(cfgB);

    if (!wa.IsOpen() || !wb.IsOpen()) {
        fprintf(stderr, "Une des deux fenetres ne s'est pas ouverte\n");
        return -1;
    }

    NkWindowId idA = wa.GetId();
    NkWindowId idB = wb.GetId();
    fprintf(stderr, "idA=%llu idB=%llu\n", (unsigned long long)idA, (unsigned long long)idB);
    fflush(stderr);

    NkContextDesc desc;
    desc.api = NkGraphicsApi::NK_GFX_API_OPENGL;
    desc.opengl.majorVersion = 3;
    desc.opengl.minorVersion = 3;
    desc.opengl.profile = NkGLProfile::Core;

    NkIGraphicsContext *gfxA = NkContextFactory::Create(wa, desc);
    NkIGraphicsContext *gfxB = NkContextFactory::Create(wb, desc);

    if (!gfxA || !gfxB) {
        fprintf(stderr, "Un des contextes a echoue (gfxA=%p gfxB=%p)\n",
            (void*)gfxA, (void*)gfxB);
        fflush(stderr);
        if (gfxA) NkContextFactory::Destroy(gfxA);
        if (gfxB) NkContextFactory::Destroy(gfxB);
        return -2;
    }

    NkIRenderer2D *r2dA = NkRenderer2DFactory::Create(gfxA);
    NkIRenderer2D *r2dB = NkRenderer2DFactory::Create(gfxB);

    if (!r2dA || !r2dB) {
        fprintf(stderr, "Un des renderers a echoue (r2dA=%p r2dB=%p)\n",
            (void*)r2dA, (void*)r2dB);
        fflush(stderr);
        if (r2dA) NkRenderer2DFactory::Destroy(r2dA);
        if (r2dB) NkRenderer2DFactory::Destroy(r2dB);
        NkContextFactory::Destroy(gfxA);
        NkContextFactory::Destroy(gfxB);
        return -3;
    }

    fprintf(stderr, "Deux contextes et deux renderers crees\n"); fflush(stderr);

    NkEventSystem &events = NkEvents();

    events.AddEventCallback<NkMouseButtonPressEvent>([&](NkMouseButtonPressEvent *e) {
        if (!e->IsLeft()) return;
        uint64 id = e->GetWindowId();
        const char *qui = "inconnue";
        if (id == idA) qui = "A";
        else if (id == idB) qui = "B";
        fprintf(stderr, "[clic] fenetre %s (id=%llu) at client=(%d,%d)\n",
            qui, (unsigned long long)id, e->GetX(), e->GetY());
        fflush(stderr);
    });

    auto start = std::chrono::steady_clock::now();
    int frame = 0;

    while (wa.IsOpen() || wb.IsOpen()) {
        events.PollEvents();

        // Rendu dans A
        if (wa.IsOpen()) {
            auto szA = wa.GetSize();
            gfxA->MakeCurrent();
            if (gfxA->BeginFrame()) {
                r2dA->Clear({80, 30, 30, 255});
                r2dA->Begin();
                NkView2D vA;
                vA.center = {(float)szA.x * 0.5f, (float)szA.y * 0.5f};
                vA.size = {(float)szA.x, (float)szA.y};
                r2dA->SetView(vA);
                r2dA->DrawFilledRect({100.f, 100.f, 200.f, 100.f}, {220, 80, 80, 255});
                r2dA->End();
                gfxA->EndFrame();
                gfxA->Present();
            }
            gfxA->ReleaseCurrent();
        }

        // Rendu dans B
        if (wb.IsOpen()) {
            auto szB = wb.GetSize();
            gfxB->MakeCurrent();
            if (gfxB->BeginFrame()) {
                r2dB->Clear({30, 30, 80, 255});
                r2dB->Begin();
                NkView2D vB;
                vB.center = {(float)szB.x * 0.5f, (float)szB.y * 0.5f};
                vB.size = {(float)szB.x, (float)szB.y};
                r2dB->SetView(vB);
                r2dB->DrawFilledRect({100.f, 100.f, 200.f, 100.f}, {80, 80, 220, 255});
                r2dB->End();
                gfxB->EndFrame();
                gfxB->Present();
            }
            gfxB->ReleaseCurrent();
        }

        frame++;
        if (frame % 60 == 0) {
            fprintf(stderr, "[frame] %d\n", frame); fflush(stderr);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (elapsed >= 30) break;
    }

    NkRenderer2DFactory::Destroy(r2dA);
    NkRenderer2DFactory::Destroy(r2dB);
    NkContextFactory::Destroy(gfxA);
    NkContextFactory::Destroy(gfxB);

    fprintf(stderr, "[fin]\n"); fflush(stderr);
    return 0;
}
