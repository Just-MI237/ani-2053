// Datation de la mesure
//
// Commande : git log -1 --format="%h %ad" --date=short
// Sortie brute : 9c3fad33 2026-09-13
// Version Jenga : 2.8.0.
//
#include "NKWindow/NKMain.h"
#include "NKThreading/NkThread.h"
#include "NKThreading/NkMutex.h"
#include "NKThreading/NkScopedLock.h"
#include "NKCore/NkAtomic.h"
#include "NKTime/NkChrono.h"

#include <cstdio>
#include <vector>

static long long compteurBrut = 0;
static long long compteurMutex = 0;
static nkentseu::threading::NkMutex mutex;
static nkentseu::NkAtomicInt compteurAtomique(0);

static void sansProtection()
{
    for (long long j = 0; j < 1000000; ++j)
    {
        compteurBrut += 1;
    }
}

static void avecMutex()
{
    for (long long j = 0; j < 1000000; ++j)
    {
        nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> verrou(mutex);
        compteurMutex += 1;
    }
}

static void avecAtomique()
{
    for (long long j = 0; j < 1000000; ++j)
    {
        compteurAtomique.FetchAdd(1);
    }
}

static double mesurerBrut()
{
    compteurBrut = 0;
    std::vector<nkentseu::threading::NkThread> fils(8);
    nkentseu::NkChrono chrono;
    for (int i = 0; i < 8; ++i)
    {
        fils[i].Start([&](void*) { sansProtection(); });
    }
    for (int i = 0; i < 8; ++i) fils[i].Join();
    return chrono.Elapsed().milliseconds;
}

static double mesurerMutex()
{
    compteurMutex = 0;
    std::vector<nkentseu::threading::NkThread> fils(8);
    nkentseu::NkChrono chrono;
    for (int i = 0; i < 8; ++i)
    {
        fils[i].Start([&](void*) { avecMutex(); });
    }
    for (int i = 0; i < 8; ++i) fils[i].Join();
    return chrono.Elapsed().milliseconds;
}

static double mesurerAtomique()
{
    compteurAtomique.Store(0);
    std::vector<nkentseu::threading::NkThread> fils(8);
    nkentseu::NkChrono chrono;
    for (int i = 0; i < 8; ++i)
    {
        fils[i].Start([&](void*) { avecAtomique(); });
    }
    for (int i = 0; i < 8; ++i) fils[i].Join();
    return chrono.Elapsed().milliseconds;
}

static double mesurerLocal(long long &total)
{
    std::vector<long long> totaux(8, 0);
    std::vector<nkentseu::threading::NkThread> fils(8);
    nkentseu::NkChrono chrono;
    for (int i = 0; i < 8; ++i)
    {
        fils[i].Start([&](void*) {
            long long local = 0;
            for (long long j = 0; j < 1000000; ++j) local += 1;
            totaux[i] = local;
        });
    }
    for (int i = 0; i < 8; ++i) fils[i].Join();
    total = 0;
    for (long long t : totaux) total += t;
    return chrono.Elapsed().milliseconds;
}

int nkmain(const nkentseu::NkEntryState &state)
{
    (void)state;

    for (int essai = 0; essai < 3; ++essai)
    {
        double tb = mesurerBrut();
        std::printf("Brut %d %.3f total %lld\n", essai + 1, tb, compteurBrut);
    }
    for (int essai = 0; essai < 3; ++essai)
    {
        double tm = mesurerMutex();
        std::printf("Mutex %d %.3f total %lld\n", essai + 1, tm, compteurMutex);
    }
    for (int essai = 0; essai < 3; ++essai)
    {
        double ta = mesurerAtomique();
        std::printf("Atomique %d %.3f total %d\n", essai + 1, ta, (int)compteurAtomique.Load());
    }
    for (int essai = 0; essai < 3; ++essai)
    {
        long long total = 0;
        double tl = mesurerLocal(total);
        std::printf("Local %d %.3f total %lld\n", essai + 1, tl, total);
    }

    return 0;
}
