// Exercice 4 du chapitre 4 - La coquille et la main.
// Version a la main, sans NkCanvasApp.
//
// Meme programme : un carre rouge avance de 100 pixels par seconde,
// sur un fond sombre. La boucle est montee a la main.

#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKWindow/Core/NkWindowConfig.h"
#include "NKWindow/Core/NkWESystem.h"
#include "NKEvent/NkWindowEvent.h"
#include "NKTime/NkTime.h"
#include "NKLogger/NkLog.h"
#include "NKMath/NkColor.h"
#include "NKMath/NKMath.h"
#include "NKCanvas/Core/NkContextDesc.h"
#include "NKCanvas/Core/NkGraphicsApi.h"
#include "NKCanvas/Renderer/Targets/NkRenderWindow.h"
#include "NKCanvas/Renderer/Core/NkRenderer2D.h"

using namespace nkentseu;
using namespace nkentseu::renderer;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName = "La coquille et la main";
    d.appVersion = "1.0.0";
    return d;
})());

int nkmain(const NkEntryState &state)
{
    (void)state;

    NkWindowConfig cfg;
    cfg.title = "La coquille et la main - main";
    cfg.width = 800;
    cfg.height = 400;
    cfg.centered = true;
    cfg.resizable = true;

    NkWindow window;
    if (!window.Create(cfg))
    {
        return -1;
    }

    {
        NkContextDesc desc;
        desc.api = NkGraphicsApi::NK_GFX_API_OPENGL;
        NkRenderWindow target(window, desc);
        if (!target.IsValid())
        {
            window.Close();
            return -2;
        }

        bool running = true;
        NkEventSystem &events = NkEvents();
        events.AddEventCallback<NkWindowCloseEvent>([&](NkWindowCloseEvent *) {
            running = false;
        });

        NkClock clock;
        float32 x = 0.f;

        while (running && window.IsOpen())
        {
            float32 dt = clock.Tick().delta;
            if (dt > 0.1f)
            {
                dt = 1.f / 60.f;
            }

            while (NkEvent *ev = events.PollEvent())
            {
                (void)ev;
            }

            if (!running)
            {
                break;
            }

            x += 100.f * dt;
            if (x > 800.f)
            {
                x = -50.f;
            }

            target.Clear(NkColor2D{18, 18, 24, 255});
            NkRenderer2D &r = target.GetRenderer2D();
            r.DrawFilledRect({x, 175.f, 50.f, 50.f}, NkColor2D{200, 60, 60, 255});
            target.Display();
        }
    }

    window.Close();
    return 0;
}
