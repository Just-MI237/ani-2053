// mesure.cpp
// Charge cinq images avec NkImage, calcule la memoire attendue
// (largeur fois hauteur fois octets par pixel), la mesure avec mallinfo2,
// et la compare a la taille du fichier sur disque.

#include "NKImage/Core/NkImage.h"
#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"

#include <cstdio>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <malloc.h>

using namespace nkentseu;

// Memoire reellement allouee par le programme, en octets.
// mallinfo2 rend le total des blocs vivants du tas (uordblks).
static long long memoireAllouee()
{
    struct mallinfo2 mi = mallinfo2();
    return (long long)mi.uordblks;
}

static long long tailleFichier(const std::string& chemin)
{
    struct stat st;
    if (stat(chemin.c_str(), &st) != 0)
    {
        return -1;
    }
    return (long long)st.st_size;
}

int nkmain(const NkEntryState &state)
{
    (void)state;

    const char* noms[] = {
        "images/icone.png",
        "images/capture.png",
        "images/aplats.png",
        "images/photo1.jpg",
        "images/photo2.jpg"
    };

    // Premiere passe, jetee. Elle paie la mise en route du decodeur PNG
    // et l'allocation des tampons internes. Sans elle, la premiere image
    // porterait un cout qui ne lui appartient pas.
    {
        std::vector<NkImage> chauffe;
        chauffe.resize(5);
        for (int i = 0; i < 5; ++i)
        {
            chauffe[i].Load(noms[i]);
        }
    }

    // Deuxieme passe, mesuree. Toutes les images restent en vie
    // jusqu'a la fin, comme l'enonce le demande.
    std::vector<NkImage> images;
    images.resize(5);

    for (int i = 0; i < 5; ++i)
    {
        long long avant = memoireAllouee();
        images[i].Load(noms[i]);
        long long apres = memoireAllouee();

        const NkImage& im = images[i];
        int w = im.Width();
        int h = im.Height();
        int bpp = im.BytesPP();
        long long calcul = (long long)w * h * bpp;
        long long mesure = apres - avant;
        long long fichier = tailleFichier(noms[i]);

        char journal[256];
        std::snprintf(journal, sizeof(journal),
                      "[mesure] %s %dx%d bpp %d", noms[i], w, h, bpp);
        logger.Info(journal);

        std::printf("image : %s, calcul : %lld, mesure : %lld, fichier : %lld\n",
                    noms[i], calcul, mesure, fichier);
    }

    return 0;
}
