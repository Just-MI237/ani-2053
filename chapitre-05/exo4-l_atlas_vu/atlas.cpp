// Datation de la mesure
//
// Commande : git log -1 --format="%h %ad" --date=short
// Sortie brute : 9c3fad33 2026-09-13
// Version Jenga : 2.8.0.
//
// atlas.cpp
// Construit un atlas de police avec NKFont a deux tailles,
// enregistre la texture de la taille 18 en PNG, et rapporte
// le nombre de glyphes et la place perdue dans la texture.

#include "NKFont/NkFont.h"
#include "NKImage/Core/NkImage.h"
#include "NKWindow/NKMain.h"

#include <cstdio>

using namespace nkentseu;

static void construire(long long taille, bool rendre)
{
    NkFontAtlas atlas;

    NkFontConfig cfg;
    cfg.glyphRanges = NkFontAtlas::GetGlyphRangesDefault();

    NkFont* police = atlas.AddFontFromFile("fonts/police.ttf", (float)taille, &cfg);
    if (police == nullptr)
    {
        std::printf("police introuvable pour taille %lld\n", taille);
        return;
    }

    if (!atlas.Build())
    {
        std::printf("Build echoue pour taille %lld\n", taille);
        return;
    }

    nkft_uint8* pixels = nullptr;
    nkft_int32 largeur = 0;
    nkft_int32 hauteur = 0;
    atlas.GetTexDataAsRGBA32(&pixels, &largeur, &hauteur);

    std::printf("taille : %lld, glyphes : %u, texture : %d %d\n",
                taille, police->glyphCount, largeur, hauteur);

    if (rendre)
    {
        NkImage vue = NkImage::Wrap(pixels, largeur, hauteur,
                                    NkImagePixelFormat::NK_RGBA32);
        bool ok = vue.SavePNG("atlas.png");
        std::printf("atlas.png ecrit : %s\n", ok ? "oui" : "non");
    }
}

int nkmain(const NkEntryState &state)
{
    (void)state;

    construire(18, true);
    construire(48, false);

    return 0;
}
