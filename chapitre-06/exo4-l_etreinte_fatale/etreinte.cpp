// Datation de la mesure
//
// Commande : git log -1 --format="%h %ad" --date=short
// Sortie brute : 9c3fad33 2026-09-13
// Version Jenga : 2.8.0.
//
// Version qui se fige, gardee en commentaire pour reference.
// Le fil a prend m1 puis m2. Le fil b prend m2 puis m1. L'ordre
// croise produit l'etreinte.
//
// static void filA_qui_se_fige()
// {
//     nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> v1(m1);
//     std::printf("A prend m1\n");
//     nkentseu::NkChrono::SleepMilliseconds(100);
//     nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> v2(m2);
//     std::printf("A prend m2\n");
// }
//
// static void filB_qui_se_fige()
// {
//     nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> v2(m2);
//     std::printf("B prend m2\n");
//     nkentseu::NkChrono::SleepMilliseconds(100);
//     nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> v1(m1);
//     std::printf("B prend m1\n");
// }
//
// Version corrigee. Les deux fils prennent les verrous dans le meme
// ordre : m1 d'abord, m2 ensuite. Les deux verrous et les deux
// attentes sont gardes.

#include "NKWindow/NKMain.h"
#include "NKThreading/NkThread.h"
#include "NKThreading/NkMutex.h"
#include "NKThreading/NkScopedLock.h"
#include "NKTime/NkChrono.h"

#include <cstdio>

static nkentseu::threading::NkMutex m1;
static nkentseu::threading::NkMutex m2;

static void filA()
{
    nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> v1(m1);
    std::printf("A prend m1\n");
    nkentseu::NkChrono::SleepMilliseconds(100);
    nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> v2(m2);
    std::printf("A prend m2\n");
}

static void filB()
{
    nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> v1(m1);
    std::printf("B prend m1\n");
    nkentseu::NkChrono::SleepMilliseconds(100);
    nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> v2(m2);
    std::printf("B prend m2\n");
}

int nkmain(const nkentseu::NkEntryState &state)
{
    (void)state;

    nkentseu::threading::NkThread a;
    nkentseu::threading::NkThread b;
    a.Start([&](void*) { filA(); });
    b.Start([&](void*) { filB(); });
    a.Join();
    b.Join();
    std::printf("Fin\n");
    return 0;
}
