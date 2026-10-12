// Datation de la mesure
//
// Commande : git log -1 --format="%h %ad" --date=short
// Sortie brute : 9c3fad33 2026-09-13
// Version Jenga : 2.8.0.
//
#include <iostream>
#include <string>
#include <vector>

struct Groupe
{
    std::string nom;
    long long n;
    long long w;
    long long h;
    long long x1;
    long long y1;
    long long x2;
    long long y2;
};

static bool ranger(std::vector<Groupe>& groupes, long long P, long long W, long long H,
                   long long& occupe)
{
    long long x = P;
    long long y = P;
    long long e = 0;

    for (Groupe& g : groupes)
    {
        long long rw = g.w + P;
        long long rh = g.h + P;

        g.x1 = 0;
        g.y1 = 0;
        g.x2 = 0;
        g.y2 = 0;

        for (long long k = 0; k < g.n; ++k)
        {
            if (x + rw > W - P)
            {
                x = P;
                y = y + e + P;
                e = 0;
                if (x + rw > W - P)
                {
                    return false;
                }
            }
            if (y + rh > H - P)
            {
                return false;
            }

            if (k == 0)
            {
                g.x1 = x;
                g.y1 = y;
            }
            g.x2 = x;
            g.y2 = y;

            x = x + rw;
            if (rh > e)
            {
                e = rh;
            }
        }
    }

    occupe = 0;
    for (const Groupe& g : groupes)
    {
        occupe += g.n * g.w * g.h;
    }
    return true;
}

int main()
{
    long long P = 0;
    long long L = 0;
    std::cin >> P >> L;

    int G = 0;
    std::cin >> G;

    if (G == 0)
    {
        std::cout << "AUCUN\n";
        return 0;
    }

    std::vector<Groupe> groupes;
    groupes.resize(G);
    for (int i = 0; i < G; ++i)
    {
        std::cin >> groupes[i].nom >> groupes[i].n
                 >> groupes[i].w >> groupes[i].h;
    }

    long long W = L;
    if (W == 0)
    {
        long long besoin = 0;
        for (const Groupe& g : groupes)
        {
            besoin += g.n * (g.w + P) * (g.h + P);
        }
        W = 512;
        while (W * W < 2 * besoin && W < 4096)
        {
            W = W * 2;
        }
    }

    long long H = W;
    long long occupe = 0;
    int essai = 0;

    for (essai = 1; essai <= 8; ++essai)
    {
        if (ranger(groupes, P, W, H, occupe))
        {
            break;
        }
        if (W == H)
        {
            W = W * 2;
        }
        else
        {
            H = W;
        }
    }

    if (essai > 8)
    {
        std::cout << "ESSAIS 8\n";
        std::cout << "ECHEC\n";
        return 0;
    }

    std::cout << "ESSAIS " << essai << "\n";
    std::cout << "TEXTURE " << W << " " << H << "\n";

    for (const Groupe& g : groupes)
    {
        std::cout << g.nom << " " << g.x1 << " " << g.y1 << " "
                  << g.x2 << " " << g.y2 << "\n";
    }

    long long surface = W * H;
    long long perdu = (surface - occupe) * 100 / surface;

    std::cout << "OCCUPE " << occupe << "\n";
    std::cout << "PERDU " << perdu << "\n";

    return 0;
}
