#include <iostream>
#include <string>
#include <vector>

struct Glyphe
{
    char c;
    long long avance;
    long long x0;
    long long y0;
    long long x1;
    long long y1;
};

int main()
{
    int G = 0;
    std::cin >> G;

    std::vector<Glyphe> glyphes;
    glyphes.resize(G);
    for (int i = 0; i < G; ++i)
    {
        std::cin >> glyphes[i].c >> glyphes[i].avance
                 >> glyphes[i].x0 >> glyphes[i].y0
                 >> glyphes[i].x1 >> glyphes[i].y1;
    }

    int K = 0;
    std::cin >> K;

    std::vector<std::string> couples;
    std::vector<long long> corrections;
    couples.resize(K);
    corrections.resize(K);
    for (int i = 0; i < K; ++i)
    {
        std::cin >> couples[i] >> corrections[i];
    }

    std::string texte;
    long long ox = 0;
    long long oy = 0;
    std::cin >> texte >> ox >> oy;

    long long x = ox;
    long long minx = 0;
    long long miny = 0;
    long long maxx = 0;
    long long maxy = 0;
    bool boite = false;
    int absents = 0;

    for (std::size_t i = 0; i < texte.size(); ++i)
    {
        char c = texte[i];

        const Glyphe* g = nullptr;
        for (const Glyphe& gg : glyphes)
        {
            if (gg.c == c)
            {
                g = &gg;
                break;
            }
        }

        if (g == nullptr)
        {
            std::cout << c << " ABSENT\n";
            absents++;
            continue;
        }

        std::cout << c << " " << x << "\n";

        bool dessine = (g->x1 > g->x0) && (g->y1 > g->y0);
        if (dessine)
        {
            long long rx1 = x + g->x0;
            long long ry1 = oy + g->y0;
            long long rx2 = x + g->x1;
            long long ry2 = oy + g->y1;

            if (!boite)
            {
                minx = rx1;
                miny = ry1;
                maxx = rx2;
                maxy = ry2;
                boite = true;
            }
            else
            {
                if (rx1 < minx) minx = rx1;
                if (ry1 < miny) miny = ry1;
                if (rx2 > maxx) maxx = rx2;
                if (ry2 > maxy) maxy = ry2;
            }
        }

        x = x + g->avance;

        if (i + 1 < texte.size())
        {
            std::string paire;
            paire += c;
            paire += texte[i + 1];
            for (int k = 0; k < K; ++k)
            {
                if (couples[k] == paire)
                {
                    x = x + corrections[k];
                    break;
                }
            }
        }
    }

    std::cout << "CURSEUR " << x << "\n";

    if (!boite)
    {
        std::cout << "BOITE AUCUNE\n";
        std::cout << "MONTE 0\n";
        std::cout << "DESCEND 0\n";
        std::cout << "ECRAN RIEN\n";
        std::cout << "ABSENTS " << absents << "\n";
        return 0;
    }

    long long monte = 0;
    if (miny < oy)
    {
        monte = oy - miny;
    }
    long long descend = 0;
    if (maxy > oy)
    {
        descend = maxy - oy;
    }

    std::string ecran;
    if (maxy <= 0)
    {
        ecran = "HORS";
    }
    else if (miny < 0)
    {
        ecran = "COUPE";
    }
    else
    {
        ecran = "VISIBLE";
    }

    std::cout << "BOITE " << minx << " " << miny << " " << maxx << " " << maxy << "\n";
    std::cout << "MONTE " << monte << "\n";
    std::cout << "DESCEND " << descend << "\n";
    std::cout << "ECRAN " << ecran << "\n";
    std::cout << "ABSENTS " << absents << "\n";

    return 0;
}
