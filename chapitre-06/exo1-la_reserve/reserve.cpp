// Datation de la mesure
//
// Commande : git log -1 --format="%h %ad" --date=short
// Sortie brute : 9c3fad33 2026-09-13
// Version Jenga : 2.8.0.
//
#include "NKWindow/NKMain.h"
#include "NKThreading/NkThreadPool.h"
#include "NKThreading/NkThread.h"
#include "NKTime/NkChrono.h"

#include <cstdio>
#include <vector>

static long long tacheCourte()
{
    long long somme = 0;
    for (long long i = 0; i < 1000; ++i)
    {
        somme += i;
    }
    return somme;
}

static long long tacheLongue()
{
    long long somme = 0;
    for (long long i = 0; i < 100000; ++i)
    {
        somme += i;
    }
    return somme;
}

int nkmain(const nkentseu::NkEntryState &state)
{
    (void)state;

    const int nombre = 200;

    std::printf("Tache courte\n");

    nkentseu::threading::NkThreadPool pool;
    std::printf("Ouvriers %u\n", (unsigned)pool.GetNumWorkers());

    for (int essai = 0; essai < 3; ++essai)
    {
        std::vector<long long> resultats(nombre, 0);
        nkentseu::NkChrono chrono;
        for (int i = 0; i < nombre; ++i)
        {
            pool.Enqueue([&resultats, i]() {
                resultats[i] = tacheCourte();
            });
        }
        pool.Join();
        double temps = chrono.Elapsed().milliseconds;
        long long somme = 0;
        for (long long r : resultats) somme += r;
        std::printf("Reserve %d %.3f somme %lld\n", essai + 1, temps, somme);
    }

    for (int essai = 0; essai < 3; ++essai)
    {
        std::vector<long long> resultats(nombre, 0);
        std::vector<nkentseu::threading::NkThread> fils(nombre);
        nkentseu::NkChrono chrono;
        for (int i = 0; i < nombre; ++i)
        {
            fils[i].Start([&resultats, i](void*) {
                resultats[i] = tacheCourte();
            });
        }
        for (int i = 0; i < nombre; ++i)
        {
            fils[i].Join();
        }
        double temps = chrono.Elapsed().milliseconds;
        long long somme = 0;
        for (long long r : resultats) somme += r;
        std::printf("Fils %d %.3f somme %lld\n", essai + 1, temps, somme);
    }

    std::printf("Tache longue\n");

    for (int essai = 0; essai < 3; ++essai)
    {
        std::vector<long long> resultats(nombre, 0);
        nkentseu::NkChrono chrono;
        for (int i = 0; i < nombre; ++i)
        {
            pool.Enqueue([&resultats, i]() {
                resultats[i] = tacheLongue();
            });
        }
        pool.Join();
        double temps = chrono.Elapsed().milliseconds;
        long long somme = 0;
        for (long long r : resultats) somme += r;
        std::printf("Reserve longue %d %.3f somme %lld\n", essai + 1, temps, somme);
    }

    for (int essai = 0; essai < 3; ++essai)
    {
        std::vector<long long> resultats(nombre, 0);
        std::vector<nkentseu::threading::NkThread> fils(nombre);
        nkentseu::NkChrono chrono;
        for (int i = 0; i < nombre; ++i)
        {
            fils[i].Start([&resultats, i](void*) {
                resultats[i] = tacheLongue();
            });
        }
        for (int i = 0; i < nombre; ++i)
        {
            fils[i].Join();
        }
        double temps = chrono.Elapsed().milliseconds;
        long long somme = 0;
        for (long long r : resultats) somme += r;
        std::printf("Fils longue %d %.3f somme %lld\n", essai + 1, temps, somme);
    }

    return 0;
}
