#include <iostream>
#include <string>

int main()
{
    long long seuil = 0;
    std::cin >> seuil;

    int n = 0;
    std::cin >> n;

    long long totalMemoire = 0;
    int nombreFlux = 0;
    int refuses = 0;

    for (int i = 0; i < n; ++i)
    {
        std::string nom;
        long long frequence = 0;
        long long canaux = 0;
        long long bits = 0;
        long long duree = 0;
        long long fichier = 0;

        std::cin >> nom >> frequence >> canaux >> bits >> duree >> fichier;

        if (bits != 8 && bits != 16 && bits != 24 && bits != 32)
        {
            std::cout << nom << " REFUSE\n";
            refuses++;
            continue;
        }

        long long brut = frequence * canaux * (bits / 8) * duree / 1000;
        long long pourcent = fichier * 100 / brut;

        if (brut > seuil)
        {
            std::cout << nom << " " << brut << " " << pourcent << " FLUX\n";
            nombreFlux++;
        }
        else
        {
            std::cout << nom << " " << brut << " " << pourcent << " MEMOIRE\n";
            totalMemoire += brut;
        }
    }

    std::cout << "MEMOIRE " << totalMemoire << "\n";
    std::cout << "FLUX " << nombreFlux << "\n";
    std::cout << "REFUSES " << refuses << "\n";

    return 0;
}
