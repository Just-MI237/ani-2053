// Exercice 4 du chapitre 4 - La coquille et la main.
// Version avec NkCanvasApp.
//
// Le programme fait avancer un carre rouge de 100 pixels par seconde,
// sur un fond sombre. Le carre revient a gauche quand il sort a droite.

#include "NKWindow/NKMain.h"
#include "NKCanvas/App/NkCanvasApp.h"
#include "NKCanvas/Renderer/Core/NkIRenderer2D.h"

using namespace nkentseu;
using namespace nkentseu::renderer;

class CoquilleCarre : public NkCanvasApp
{
    public:
        CoquilleCarre()
        {
            Config().title = "La coquille et la main - coquille";
            Config().width = 800;
            Config().height = 400;
            Config().clearColor = NkColor2D{18, 18, 24, 255};
        }

    protected:
        void OnUpdate(float32 deltaTime) override
        {
            mX += 100.f * deltaTime;
            if (mX > 800.f)
            {
                mX = -50.f;
            }
        }

        void OnRender(NkRenderWindow &target) override
        {
            NkRenderer2D &r = target.GetRenderer2D();
            r.DrawFilledRect({mX, 175.f, 50.f, 50.f}, NkColor2D{200, 60, 60, 255});
        }

    private:
        float32 mX = 0.f;
};

int nkmain(const NkEntryState &state)
{
    return NkCanvasApp::Run<CoquilleCarre>(state);
}
