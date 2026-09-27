#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"
#include "NKCanvas/Factory/NkContextFactory.h"
#include "NKCanvas/Core/NkContextDesc.h"
#include "NKCanvas/Core/NkOpenGLDesc.h"

using namespace nkentseu;

int nkmain(const NkEntryState &state) {
    NkWindowConfig cfg;
    cfg.title = "Le facteur d'echelle";
    cfg.width = 1280;
    cfg.height = 720;
    cfg.resizable = true;
    NkWindow window(cfg);
    if (!window.IsOpen()) {
        return -1;
    }

    NkContextDesc desc;
    desc.api = NkGraphicsApi::NK_GFX_API_OPENGL;
    desc.opengl.majorVersion = 4;
    desc.opengl.minorVersion = 6;
    desc.opengl.profile = NkGLProfile::Core;

    auto ctx = NkContextFactory::Create(window, desc);
    if (!ctx) {
        logger.Error("Contexte graphique echoue");
        window.Close();
        return -2;
    }

    auto size = window.GetSize();
    auto scale = window.GetDpiScale();
    auto info = ctx->GetInfo();

    logger.Infof("Taille fenetre     : %u x %u", size.x, size.y);
    logger.Infof("Taille cible rendu : %u x %u", info.windowWidth, info.windowHeight);
    logger.Infof("Facteur d'echelle  : %.4f", scale);

    while (window.IsOpen()) { }

    ctx->Shutdown();
    return 0;
}
