// lecteur.cpp
// Ouvre trois fichiers avec le lecteur video de NKMedia, dont deux
// qu'il refuse. Compte les images lues, interroge la sonde de conteneur,
// et rapporte ce que chaque refus apprend.

#include "NKMedia/Video/NkVideoReader.h"
#include "NKMedia/NkMediaProbe.h"
#include "NKWindow/NKMain.h"

#include <cstdio>

using namespace nkentseu;
using namespace nkentseu::media;

static const char* nomConteneur(NkMediaContainer c)
{
    switch (c)
    {
        case NkMediaContainer::NK_MP4:  return "MP4";
        case NkMediaContainer::NK_WEBM: return "WEBM";
        case NkMediaContainer::NK_WAV:  return "WAV";
        case NkMediaContainer::NK_OGG:  return "OGG";
        case NkMediaContainer::NK_MP3:  return "MP3";
        case NkMediaContainer::NK_FLAC: return "FLAC";
        default: return "INCONNU";
    }
}

static void tester(const char* chemin)
{
    std::printf("=== %s ===\n", chemin);

    NkVideoReader lecteur;
    bool ouvert = lecteur.Open(chemin);

    if (ouvert)
    {
        const NkVideoReaderInfo& info = lecteur.Info();
        std::printf("ouvert : oui, conteneur %s, codec %s, %dx%d\n",
                    info.container.CStr(), info.codec.CStr(),
                    info.width, info.height);
        int images = 0;
        NkVideoFrame frame;
        while (lecteur.ReadFrame(frame))
        {
            images++;
        }
        std::printf("images : %d\n", images);
    }
    else
    {
        std::printf("ouvert : non\n");
    }

    NkMediaInfo info;
    bool sonde = NkMediaProbe::ProbeFile(chemin, info);
    if (sonde)
    {
        std::printf("sonde : %s, pistes %zu\n",
                    nomConteneur(info.container),
                    (std::size_t)info.tracks.Size());
        for (std::size_t i = 0; i < info.tracks.Size(); ++i)
        {
            const NkMediaTrack& t = info.tracks[i];
            const char* type = (t.type == NkMediaTrackType::NK_VIDEO) ? "VIDEO" : "AUDIO";
            std::printf("  piste %zu : %s codec %s %dx%d\n",
                        (std::size_t)(i + 1), type, t.codec.CStr(),
                        t.width, t.height);
        }
    }
    else
    {
        std::printf("sonde : echec\n");
    }
    std::printf("\n");
}

int nkmain(const NkEntryState &state)
{
    (void)state;

    tester("videos/ouvrable.mp4");
    tester("videos/coupe.mp4");
    tester("videos/texte.mp4");

    return 0;
}
