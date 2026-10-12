#include <iostream>
#include <vector>

int main()
{
    long long D = 0;
    long long C = 0;
    std::cin >> D >> C;

    int n = 0;
    std::cin >> n;

    std::vector<long long> t;
    t.resize(n);
    for (int i = 0; i < n; ++i)
    {
        std::cin >> t[i];
    }

    int voixMax = 0;
    long long retardMax = 0;
    int coupes = 0;

    long long finChargementPrecedent = 0;

    for (int i = 0; i < n; ++i)
    {
        int voix = 0;
        for (int j = 0; j <= i; ++j)
        {
            if (t[j] + D > t[i])
            {
                voix++;
            }
        }

        long long debut = t[i];
        if (finChargementPrecedent > debut)
        {
            debut = finChargementPrecedent;
        }
        long long fin = debut + C;
        long long retard = fin - t[i];
        finChargementPrecedent = fin;

        bool coupe = false;
        if (i + 1 < n && t[i + 1] < t[i] + D)
        {
            coupe = true;
        }

        std::cout << t[i] << " " << voix << " " << retard << " ";
        if (coupe)
        {
            std::cout << "COUPE\n";
            coupes++;
        }
        else
        {
            std::cout << "ENTIER\n";
        }

        if (voix > voixMax)
        {
            voixMax = voix;
        }
        if (retard > retardMax)
        {
            retardMax = retard;
        }
    }

    std::cout << "VOIX_MAX " << voixMax << "\n";
    std::cout << "RETARD_MAX " << retardMax << "\n";
    std::cout << "COUPES " << coupes << "\n";

    return 0;
}
