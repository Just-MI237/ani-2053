// Exercice 10 du chapitre 4 - L'interface qui ne defile pas.
//
// Le monde defile sous une barre d'interface fixe. La barre est
// dessinee apres ResetView, qui remet la vue par defaut.
//
// Le mode est choisi par la variable d'environnement NK_RESET.
// NK_RESET=1 (defaut) : ResetView est appele, la barre est fixe.
// NK_RESET=0 : ResetView n'est pas appele, la barre suit le monde.
//
// La version fautive (sans ResetView) reste dans le code, sous la
// condition sur mFaireReset. C'est le comportement decrit par
// l'enonce : garder la version fautive en commentaire, ou ici sous
// condition.

#include "NKWindow/NKMain.h"
#include "NKCanvas/App/NkCanvasApp.h"
#include "NKCanvas/Renderer/Core/NkIRenderer2D.h"
#include "NKCanvas/Renderer/Resources/NkFont.h"
#include "NKCanvas/Renderer/Resources/NkSprite.h"
#include "NKLogger/NkLog.h"

#include <cstdlib>
#include <cstring>

using namespace nkentseu;
using namespace nkentseu::renderer;

class Interface : public NkCanvasApp
{
    public:
        Interface()
        {
            Config().title = "L'interface qui ne defile pas";
            Config().width = 800;
            Config().height = 400;
            Config().clearColor = NkColor2D{18, 18, 24, 255};

            const char *mode = getenv("NK_RESET");
            mFaireReset = (mode == nullptr) || (strcmp(mode, "0") != 0);
        }

    protected:
        bool OnInit() override
        {
            mHasFont = mFont.LoadFromFile(*Target().GetRenderer(),
                "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
            if (mHasFont)
            {
                mTexte.SetFont(mFont);
                mTexte.SetString("Barre fixe");
                mTexte.SetCharacterSize(20);
                mTexte.SetFillColor({240, 240, 240, 255});
                mTexte.SetPosition({10.f, 6.f});
            }
            logger.Infof("[interface] mode reset = %s",
                mFaireReset ? "oui" : "non");
            return true;
        }

        void OnUpdate(float32 deltaTime) override
        {
            mCentreX += 200.f * deltaTime;
        }

        void OnRender(NkRenderWindow &target) override
        {
            NkRenderer2D &r = target.GetRenderer2D();

            NkView2D vue;
            vue.center = {mCentreX, 200.f};
            vue.size = {800.f, 400.f};
            r.SetView(vue);

            for (int i = 0; i < 30; ++i)
            {
                float x = (float)i * 120.f;
                NkColor2D c = (i % 2 == 0)
                    ? NkColor2D{60, 100, 160, 255}
                    : NkColor2D{80, 130, 200, 255};
                r.DrawFilledRect({x, 100.f, 100.f, 100.f}, c);
            }

            if (mFaireReset)
            {
                r.ResetView();
                mBarreX = 0.f;
            }
            else
            {
                mBarreX = 400.f - mCentreX;
            }

            // Version fautive, gardee en commentaire :
            // ne pas appeler ResetView avant de dessiner la barre.
            // La barre herite alors de la vue du monde et sort de
            // l'ecran des que le monde a defile de plus de 400 pixels.
            //
            // r.DrawFilledRect({mBarreX, 0.f, 800.f, 36.f}, {30, 30, 40, 220});
            // if (mHasFont)
            // {
            //     mTexte.SetPosition({mBarreX + 10.f, 6.f});
            //     r.Draw(mTexte);
            // }

            logger.Infof("reset : %s, centre_x : %.0f, barre_x : %.0f",
                mFaireReset ? "oui" : "non", mCentreX, mBarreX);

            r.DrawFilledRect({mBarreX, 0.f, 800.f, 36.f}, {30, 30, 40, 220});
            if (mHasFont)
            {
                mTexte.SetPosition({mBarreX + 10.f, 6.f});
                r.Draw(mTexte);
            }
        }

    private:
        float32 mCentreX = 400.f;
        float32 mBarreX = 0.f;
        bool mFaireReset = true;
        bool mHasFont = false;
        renderer::NkFont mFont;
        renderer::NkText mTexte;
};

int nkmain(const NkEntryState &state)
{
    return NkCanvasApp::Run<Interface>(state);
}
