// Datation de la mesure
//
// Commande : git log -1 --format="%h %ad" --date=short
// Sortie brute : 9c3fad33 2026-09-13
// Version Jenga : 2.8.0.
//
#include "NKWindow/NKMain.h"
#include "NKThreading/NkThread.h"

#include <cstdio>
#include <vector>

static long long compteur = 0;

static void incrementerUnMillion()
{
    for (long long j = 0; j < 1000000; ++j)
    {
        compteur += 1;
    }
}

int nkmain(const nkentseu::NkEntryState &state)
{
    (void)state;

    const int huit = 8;
    const long long attendu = huit * 1000000LL;

    compteur = 0;
    std::vector<nkentseu::threading::NkThread> fils(huit);
    for (int i = 0; i < huit; ++i)
    {
        fils[i].Start([&](void*) {
            incrementerUnMillion();
        });
    }
    for (int i = 0; i < huit; ++i)
    {
        fils[i].Join();
    }
    std::printf("Huit fils : %lld (attendu %lld)\n", compteur, attendu);

    compteur = 0;
    nkentseu::threading::NkThread seul;
    seul.Start([&](void*) {
        for (long long j = 0; j < huit * 1000000LL; ++j)
        {
            compteur += 1;
        }
    });
    seul.Join();
    std::printf("Un seul fil : %lld (attendu %lld)\n", compteur, attendu);

    return 0;
}
