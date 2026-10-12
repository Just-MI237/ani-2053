// Datation de la mesure
//
// Commande : git log -1 --format="%h %ad" --date=short
// Sortie brute : 9c3fad33 2026-09-13
// Version Jenga : 2.8.0.
//
#include <iostream>
#include <string>

struct Format
{
    const char* nom;
    long long octetsParPixel;
    bool couleur;
    bool transparence;
    bool flottants;
};

static const Format formats[] = {
    {"GRAY8",    1,  false, false, false},
    {"GRAY_A16", 2,  false, true,  false},
    {"RGB24",    3,  true,  false, false},
    {"RGBA32",   4,  true,  true,  false},
    {"RGB96F",   12, true,  false, true},
    {"RGBA128F", 16, true,  true,  true}
};

static const Format* trouver(const std::string& nom)
{
    for (const Format& f : formats)
    {
        if (nom == f.nom)
        {
            return &f;
        }
    }
    return nullptr;
}

int main()
{
    long long w = 0;
    long long h = 0;
    std::cin >> w >> h;

    int n = 0;
    std::cin >> n;

    long long total = 0;
    int sansPerte = 0;
    int refuses = 0;

    for (int i = 0; i < n; ++i)
    {
        std::string source;
        std::string cible;
        std::cin >> source >> cible;

        const Format* fs = trouver(source);
        const Format* fc = trouver(cible);

        if (fs == nullptr || fc == nullptr)
        {
            std::cout << source << " " << cible << " REFUSE\n";
            refuses++;
            continue;
        }

        long long octetsSource = w * h * fs->octetsParPixel;
        long long octetsCible = w * h * fc->octetsParPixel;

        std::string pertes;
        if (fs->transparence && !fc->transparence)
        {
            pertes += "TRANSPARENCE";
        }
        if (fs->couleur && !fc->couleur)
        {
            if (!pertes.empty())
            {
                pertes += "+";
            }
            pertes += "COULEUR";
        }
        if (fs->flottants && !fc->flottants)
        {
            if (!pertes.empty())
            {
                pertes += "+";
            }
            pertes += "ETENDUE";
        }
        if (pertes.empty())
        {
            pertes = "AUCUNE";
            sansPerte++;
        }

        std::cout << source << " " << cible << " " << octetsSource << " "
                  << octetsCible << " " << pertes << "\n";

        total += octetsCible;
    }

    std::cout << "TOTAL " << total << "\n";
    std::cout << "SANS_PERTE " << sansPerte << "\n";
    std::cout << "REFUSES " << refuses << "\n";

    return 0;
}
