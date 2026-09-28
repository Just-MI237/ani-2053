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
#include "NKCanvas/Renderer/Resources/NkFont.h"
#include "NKCanvas/Renderer/Resources/NkSprite.h"

#include <cstdio>
#include <thread>
#include <chrono>

using namespace nkentseu;
using namespace nkentseu::renderer;

static const int TITLEBAR_H = 32;
static const int BUTTON_W = 40;
static const int MAX_W = 1200;
static const int MAX_H = 800;

int nkmain(const NkEntryState &state) {
    NkWindowConfig cfg;
    cfg.title = "Fenetre sans bordure";
    cfg.width = 800;
    cfg.height = 600;
    cfg.resizable = true;
    NkWindow window(cfg);
    if (!window.IsOpen()) {
        return -1;
    }

    window.SetDecorated(false);

    NkContextDesc desc;
    desc.api = NkGraphicsApi::NK_GFX_API_OPENGL;
    desc.opengl.majorVersion = 3;
    desc.opengl.minorVersion = 3;
    desc.opengl.profile = NkGLProfile::Core;

    NkIGraphicsContext *gfx = NkContextFactory::Create(window, desc);
    if (!gfx) {
        fprintf(stderr, "Contexte graphique indisponible\n");
        window.Close();
        return -2;
    }

    NkIRenderer2D *r2d = NkRenderer2DFactory::Create(gfx);
    if (!r2d) {
        fprintf(stderr, "Renderer 2D indisponible\n");
        NkContextFactory::Destroy(gfx);
        window.Close();
        return -3;
    }

    renderer::NkFont font;
    bool hasFont = font.LoadFromFile(*r2d, "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    fprintf(stderr, "Police chargee : %d\n", (int)hasFont);
    fflush(stderr);

    NkText titleText;
    if (hasFont) {
        titleText.SetFont(font);
        titleText.SetString("Fenetre sans bordure");
        titleText.SetCharacterSize(18);
        titleText.SetFillColor({240, 240, 240, 255});
        titleText.SetPosition({14.f, 8.f});
    }

    uint32 winW = cfg.width;
    uint32 winH = cfg.height;

    NkEventSystem &events = NkEvents();

    events.AddEventCallback<NkMouseButtonPressEvent>([&](NkMouseButtonPressEvent *e) {
        if (!e->IsLeft()) return;
        fprintf(stderr, "[press] client=(%d,%d) ecran=(%d,%d) clickCount=%d\n", e->GetX(), e->GetY(), e->GetScreenX(), e->GetScreenY(), (int)e->GetClickCount()); fflush(stderr);
        int x = e->GetX();
        int y = e->GetY();
        int w = (int)winW;
        if (y < 0 || y >= TITLEBAR_H) return;

        int minX = w - 3 * BUTTON_W;
        int maxX = w - 2 * BUTTON_W;
        int closeX = w - BUTTON_W;

        if (x >= minX && x < maxX) {
            fprintf(stderr, "[clic] Minimize\n"); fflush(stderr);
            window.Minimize();
        } else if (x >= maxX && x < closeX) {
            fprintf(stderr, "[clic] Maximize borne\n"); fflush(stderr);
            window.SetSize(MAX_W, MAX_H);
        } else if (x >= closeX) {
            fprintf(stderr, "[clic] Close\n"); fflush(stderr);
            window.Close();
        } else {
            fprintf(stderr, "[clic] barre de titre (%d,%d), drag\n", x, y); fflush(stderr);
            window.BeginDragMove();
        }
    });

    events.AddEventCallback<NkMouseDoubleClickEvent>([&](NkMouseDoubleClickEvent *e) {
        fprintf(stderr, "[double-clic recu] x=%d y=%d\n", e->GetX(), e->GetY()); fflush(stderr);
        int y = e->GetY();
        if (y < 0 || y >= TITLEBAR_H) return;
        fprintf(stderr, "[double-clic] Maximize borne\n"); fflush(stderr);
        window.SetSize(MAX_W, MAX_H);
    });

    auto start = std::chrono::steady_clock::now();

    while (window.IsOpen()) {
        events.PollEvents();

        {
            auto sz = window.GetSize();
            if (sz.x != winW || sz.y != winH) {
                winW = sz.x;
                winH = sz.y;
                gfx->OnResize(winW, winH);
                r2d->SetViewport({0, 0, (int32)winW, (int32)winH});
                fprintf(stderr, "[taille] %ux%u\n", winW, winH); fflush(stderr);
            }
        }

        if (!gfx->BeginFrame()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            continue;
        }

        r2d->Clear({30, 30, 40, 255});
        r2d->Begin();
        NkView2D view;
        view.center = {(float)winW * 0.5f, (float)winH * 0.5f};
        view.size = {(float)winW, (float)winH};
        r2d->SetView(view);

        int w = (int)winW;

        r2d->DrawFilledRect({0.f, 0.f, (float)winW, (float)TITLEBAR_H}, {55, 55, 70, 255});
        r2d->DrawFilledRect({(float)(w - 3*BUTTON_W), 0.f, (float)BUTTON_W, (float)TITLEBAR_H}, {70, 70, 90, 255});
        r2d->DrawFilledRect({(float)(w - 2*BUTTON_W), 0.f, (float)BUTTON_W, (float)TITLEBAR_H}, {70, 70, 90, 255});
        r2d->DrawFilledRect({(float)(w - BUTTON_W),   0.f, (float)BUTTON_W, (float)TITLEBAR_H}, {170, 60, 60, 255});

        float cy = TITLEBAR_H * 0.5f;
        r2d->DrawFilledRect({(float)(w - 3*BUTTON_W + 12), cy - 1.f, (float)(BUTTON_W - 24), 2.f}, {230, 230, 230, 255});
        r2d->DrawFilledRect({(float)(w - 2*BUTTON_W + 12), cy - 8.f, (float)(BUTTON_W - 24), 16.f}, {230, 230, 230, 255});
        r2d->DrawFilledRect({(float)(w - 2*BUTTON_W + 14), cy - 6.f, (float)(BUTTON_W - 28), 12.f}, {70, 70, 90, 255});
        r2d->DrawFilledRect({(float)(w - BUTTON_W + 13), cy - 7.f, 2.f, 14.f}, {230, 230, 230, 255});
        r2d->DrawFilledRect({(float)(w - BUTTON_W + 25), cy - 7.f, 2.f, 14.f}, {230, 230, 230, 255});

        if (hasFont) {
            r2d->Draw(titleText);
        }

        r2d->End();
        gfx->EndFrame();
        gfx->Present();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (elapsed >= 60) break;
    }

    NkRenderer2DFactory::Destroy(r2d);
    NkContextFactory::Destroy(gfx);
    window.Close();

    fprintf(stderr, "[fin]\n"); fflush(stderr);
    return 0;
}
