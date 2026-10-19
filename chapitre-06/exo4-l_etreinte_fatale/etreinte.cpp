// Datation de la mesure
//
// Commande : git log -1 --format="%h %ad" --date=short
// Sortie brute : 9c3fad33 2026-09-13
// Version Jenga : 2.8.0.

// Version qui se fige, reproduite ligne par ligne. Les numeros de
// ligne cites dans etreinte.txt renvoient a ce bloc, en comptant a
// partir de la premiere ligne de code (include "NKWindow/NKMain.h").
//
//  1  #include "NKWindow/NKMain.h"
//  2  #include "NKThreading/NkThread.h"
//  3  #include "NKThreading/NkMutex.h"
//  4  #include "NKThreading/NkScopedLock.h"
//  5  #include "NKTime/NkChrono.h"
//  6
//  7  #include <cstdio>
//  8
//  9  static nkentseu::threading::NkMutex m1;
// 10  static nkentseu::threading::NkMutex m2;
// 11
// 12  static void filA()
// 13  {
// 14      nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> v1(m1);
// 15      std::printf("A prend m1\n");
// 16      nkentseu::NkChrono::SleepMilliseconds(100);
// 17      nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> v2(m2);
// 18      std::printf("A prend m2\n");
// 19  }
// 20
// 21  static void filB()
// 22  {
// 23      nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> v2(m2);
// 24      std::printf("B prend m2\n");
// 25      nkentseu::NkChrono::SleepMilliseconds(100);
// 26      nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> v1(m1);
// 27      std::printf("B prend m1\n");
// 28  }
// 29
// 30  int nkmain(const nkentseu::NkEntryState &state)
// 31  {
// 32      (void)state;
// 33
// 34      nkentseu::threading::NkThread a;
// 35      nkentseu::threading::NkThread b;
// 36      a.Start([&](void*) { filA(); });
// 37      b.Start([&](void*) { filB(); });
// 38      a.Join();
// 39      b.Join();
// 40      std::printf("Fin\n");
// 41      return 0;
// 42  }

// Version corrigee. Les deux fils prennent les verrous dans le meme
// ordre : m1 d'abord, m2 ensuite. Les deux verrous et les deux
// attentes de 100 millisecondes sont gardes.

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
