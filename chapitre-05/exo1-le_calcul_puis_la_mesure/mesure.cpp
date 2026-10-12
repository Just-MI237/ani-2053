// mesure.cpp
// Charge cinq images avec NkImage, calcule la memoire attendue
// (largeur fois hauteur fois octets par pixel), et la compare a la taille du
// fichier sur disque.
// Les cinq images : une icone PNG, une capture PNG, un dessin a aplats PNG,
// deux photographies d'appareil en JPEG.

#include "NKImage/Core/NkImage.h"
#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <malloc.h>

using namespace nkentseu;

// Memoire residente du processus, en octets. Compte le code, les
// bibliotheques, les tas internes et les pages allouees par le noyau.
// C'est la vue du gestionnaire des taches.
static long long memoireResidente()
{
    std::ifstream f("/proc/self/statm");
    long long total = 0;
    long long resident = 0;
    f >> total >> resident;
    long pageSize = sysconf(_SC_PAGESIZE);
    return resident * pageSize;
}

// Memoire reellement allouee par le programme, en octets.
// mallinfo2 rend le total des blocs vivants du tas (uordblks).
// Cet outil isole la memoire de l'image de celle du decodeur.
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
    // et l'allocation des tampons internes.
    for (int i = 0; i < 5; ++i)
    {
        NkImage chauffe;
        chauffe.Load(noms[i]);
    }

    // Les trois mesures. La premiere et la deuxieme sont des vues du
    // processus, la troisieme isole le tas.
    for (int methode = 0; methode < 3; ++methode)
    {
        if (methode == 0)
        {
            std::printf("Mesure 1 : RSS processus principal, apres echauffement\n");
        }
        else if (methode == 1)
        {
            std::printf("Mesure 2 : RSS processus enfant, isole\n");
        }
        else
        {
            std::printf("Mesure 3 : tas malloc avec mallinfo2\n");
        }

        std::vector<NkImage> images;
        images.resize(5);

        for (int i = 0; i < 5; ++i)
        {
            if (methode == 1)
            {
                std::fflush(stdout);
                pid_t pid = fork();
                if (pid == 0)
                {
                    long long avant = memoireResidente();
                    NkImage img;
                    img.Load(noms[i]);
                    long long apres = memoireResidente();

                    const NkImage& im = img;
                    int w = im.Width();
                    int h = im.Height();
                    int bpp = im.BytesPP();
                    long long calcul = (long long)w * h * bpp;
                    long long mesure = apres - avant;
                    long long fichier = tailleFichier(noms[i]);

                    std::printf("image : %s, calcul : %lld, mesure : %lld, fichier : %lld\n",
                                noms[i], calcul, mesure, fichier);
                    std::fflush(stdout);
                    _exit(0);
                }
                waitpid(pid, nullptr, 0);
                continue;
            }

            long long avant = 0;
            if (methode == 0)
            {
                avant = memoireResidente();
            }
            else
            {
                avant = memoireAllouee();
            }

            images[i].Load(noms[i]);

            long long apres = 0;
            if (methode == 0)
            {
                apres = memoireResidente();
            }
            else
            {
                apres = memoireAllouee();
            }

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
    }

    return 0;
}
