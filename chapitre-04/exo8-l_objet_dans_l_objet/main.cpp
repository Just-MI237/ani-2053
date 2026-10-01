// Exercice 8 du chapitre 4 - L'objet dans l'objet.
//
// Ce programme place des objets dans le monde, en composant les
// transformations le long d'une chaine de parents. Il retrouve le
// parent par son nom, emploie son angle et son echelle dans le monde,
// et ramene l'angle affiche entre 0 et 270.
//
// Entree :
//   N
//   nom1 parent1 tx1 ty1 angle1 echelle1
//   nom2 ...
//   ...
//
// Sortie :
//   une ligne par objet : nom x y angle echelle
//   puis PROFONDEUR n

#include <cstdio>
#include <cstring>

struct Objet
{
    char nom[64];
    long long wx, wy;
    int wangle;
    long long wscale;
    int niveau;
};

static const int MAX_OBJETS = 1000;

static int normalise(long long angle)
{
    long long r = angle % 360;
    if (r < 0)
    {
        r += 360;
    }
    return (int)r;
}

static void rotation(long long angle, long long ax, long long ay,
                     long long &rx, long long &ry)
{
    long long c, s;
    int a = normalise(angle);
    switch (a)
    {
        case 0:
            c = 1;
            s = 0;
            break;
        case 90:
            c = 0;
            s = 1;
            break;
        case 180:
            c = -1;
            s = 0;
            break;
        default:
            c = 0;
            s = -1;
            break;
    }
    rx = ax * c - ay * s;
    ry = ax * s + ay * c;
}

static int trouver(Objet *objets, int n, const char *nom)
{
    for (int i = 0; i < n; ++i)
    {
        if (strcmp(objets[i].nom, nom) == 0)
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    int N = 0;
    if (scanf("%d", &N) != 1)
    {
        return 0;
    }
    if (N > MAX_OBJETS)
    {
        return 0;
    }

    Objet objets[MAX_OBJETS];
    int profond = 0;

    for (int i = 0; i < N; ++i)
    {
        char nom[64];
        char parent[64];
        long long tx, ty, angle, echelle;
        if (scanf("%63s %63s %lld %lld %lld %lld",
                  nom, parent, &tx, &ty, &angle, &echelle) != 6)
        {
            return 0;
        }

        strcpy(objets[i].nom, nom);

        if (strcmp(parent, "-") == 0)
        {
            objets[i].wx = tx;
            objets[i].wy = ty;
            objets[i].wangle = normalise(angle);
            objets[i].wscale = echelle;
            objets[i].niveau = 1;
        }
        else
        {
            int pi = trouver(objets, i, parent);
            if (pi < 0)
            {
                return 0;
            }

            long long ax = tx * objets[pi].wscale;
            long long ay = ty * objets[pi].wscale;
            long long rx, ry;
            rotation(objets[pi].wangle, ax, ay, rx, ry);

            objets[i].wx = objets[pi].wx + rx;
            objets[i].wy = objets[pi].wy + ry;
            objets[i].wangle = normalise(objets[pi].wangle + angle);
            objets[i].wscale = objets[pi].wscale * echelle;
            objets[i].niveau = objets[pi].niveau + 1;
        }

        if (objets[i].niveau > profond)
        {
            profond = objets[i].niveau;
        }

        printf("%s %lld %lld %d %lld\n",
               objets[i].nom, objets[i].wx, objets[i].wy,
               objets[i].wangle, objets[i].wscale);
    }

    printf("PROFONDEUR %d\n", profond);
    return 0;
}
