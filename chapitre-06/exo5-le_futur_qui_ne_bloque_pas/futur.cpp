// Datation de la mesure
//
// Commande : git log -1 --format="%h %ad" --date=short
// Sortie brute : 9c3fad33 2026-09-13
// Version Jenga : 2.8.0.
//
// Note : le clone Nkentseu a ete mis a jour a 039421b1 du 2026-10-15
// apres la premiere mesure. Les valeurs sont reproductibles.
#include "NKWindow/NKMain.h"
#include "NKCanvas/App/NkCanvasApp.h"
#include "NKCanvas/Renderer/Core/NkIRenderer2D.h"
#include "NKImage/Core/NkImage.h"
#include "NKThreading/NkThreadPool.h"
#include "NKThreading/NkFuture.h"
#include "NKThreading/NkPromise.h"
#include "NKTime/NkChrono.h"

#include <cstdio>

using namespace nkentseu;
using namespace nkentseu::renderer;

// Choisir la version : 0 pour Get, 1 pour IsReady.
static const int VERSION = 1;

class TestFutur : public NkCanvasApp
{
    public:
        TestFutur()
        {
            Config().title = "TestFutur - le futur qui ne bloque pas";
            Config().width = 800;
            Config().height = 400;
            Config().clearColor = NkColor2D{18, 18, 24, 255};
        }

    protected:
        bool OnInit() override
        {
            mTrame = 0;
            mLancementFait = false;
            mChargementTermine = false;
            mTramePlusLongue = 0.0;
            mTramesPendantChargement = 0;
            mDebutChargement = 0.0;
            mFinChargement = 0.0;
            mTempsChargement = 0.0;
            mImageChargee = false;
            mChronoGlobal.Reset();
            return true;
        }

        void OnUpdate(float32 deltaTime) override
        {
            NkChrono chronoTrame;

            mX += 100.f * deltaTime;
            if (mX > 800.f)
            {
                mX = -50.f;
            }

            mTrame++;

            if (mTrame == 300)
            {
                Quit();
            }

            if (mTrame == 60 && !mLancementFait)
            {
                mLancementFait = true;
                mChronoChargement.Reset();
                mDebutChargement = mChronoGlobal.Elapsed().milliseconds;

                mPool.Enqueue([this]() {
                    NkImage img;
                    img.Load("images/grande-photo.jpg");
                    mTempsChargement = mChronoChargement.Elapsed().milliseconds;
                    mImageChargee = true;
                    mPromesse.SetValue(nkentseu::traits::NkMove(img));
                });

                if (VERSION == 0)
                {
                    while (!mFuture.IsReady())
                    {
                        nkentseu::NkChrono::SleepMilliseconds(1);
                    }
                    mChargementTermine = true;
                    mFinChargement = mChronoGlobal.Elapsed().milliseconds;
                }
            }

            if (VERSION == 1 && mLancementFait && !mChargementTermine)
            {
                if (mFuture.IsReady())
                {
                    mChargementTermine = true;
                    mFinChargement = mChronoGlobal.Elapsed().milliseconds;
                }
                else
                {
                    mTramesPendantChargement++;
                }
            }

            double trameMs = chronoTrame.Elapsed().milliseconds;
            if (trameMs > mTramePlusLongue)
            {
                mTramePlusLongue = trameMs;
            }
        }

        void OnRender(NkRenderWindow &target) override
        {
            NkRenderer2D &r = target.GetRenderer2D();
            r.DrawFilledRect({mX, 175.f, 50.f, 50.f}, NkColor2D{200, 60, 60, 255});
        }

        void OnShutdown() override
        {
            mPool.Join();
            std::printf("Trame plus longue : %.3f ms\n", mTramePlusLongue);
            std::printf("Trames pendant chargement : %lld\n", mTramesPendantChargement);
            std::printf("Temps de chargement : %.3f ms\n", mTempsChargement);
            std::printf("Debut chargement : %.3f ms\n", mDebutChargement);
            std::printf("Fin chargement : %.3f ms\n", mFinChargement);
        }

    private:
        float mX = 0.f;
        long long mTrame = 0;
        bool mLancementFait = false;
        bool mChargementTermine = false;
        bool mImageChargee = false;
        double mTramePlusLongue = 0.0;
        long long mTramesPendantChargement = 0;
        double mDebutChargement = 0.0;
        double mFinChargement = 0.0;
        double mTempsChargement = 0.0;
        nkentseu::NkChrono mChronoGlobal;
        nkentseu::NkChrono mChronoChargement;
        nkentseu::threading::NkThreadPool mPool;
        nkentseu::threading::NkPromise<NkImage> mPromesse;
        nkentseu::threading::NkFuture<NkImage> mFuture = mPromesse.GetFuture();
};

int nkmain(const NkEntryState &state)
{
    return NkCanvasApp::Run<TestFutur>(state);
}
