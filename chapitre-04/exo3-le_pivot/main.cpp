// Exercice 3 du chapitre 4 - Le pivot.
//
// Ce programme place les quatre coins d'un rectangle tourne autour de
// son origine, puis calcule la boite qui les contient. Un rectangle est
// refuse si son angle n'est pas un multiple de 90.
//
// Entree :
//   N
//   nom1 w1 h1 px1 py1 ox1 oy1 sx1 sy1 angle1
//   nom2 ...
//   ...
//
// Sortie :
//   nom COINS x1 y1 x2 y2 x3 y3 x4 y4  (si accepte)
//   nom BOITE minx miny maxx maxy       (si accepte)
//   nom ANGLE REFUSE                    (si refuse)
//   REFUSES n
//
// La regle :
//   coins locaux dans l'ordre haut-gauche (0,0), haut-droit (w,0),
//   bas-droit (w,h), bas-gauche (0,h)
//   pour chaque coin (x,y) :
//     ax = (x - ox) * sx
//     ay = (y - oy) * sy
//   puis rotation selon l'angle (0, 90, 180 ou 270) :
//     rx = ax * c - ay * s
//     ry = ax * s + ay * c
//   enfin position dans le monde : (px + rx, py + ry)
//   la boite prend le plus petit et le plus grand x, puis y, des 4 coins

#include <cstdio>
#include <cstring>

struct Rect
{
    char nom[64];
    long w, h, px, py, ox, oy, sx, sy, angle;
};

// Ramene l'angle entre 0 et 270 et verifie qu'il est un multiple de 90.
// Retourne false si l'angle est refuse.
static bool normalise(long angle, int &a)
{
    long r = angle % 360;
    if (r < 0)
    {
        r += 360;
    }
    if (r != 0 && r != 90 && r != 180 && r != 270)
    {
        return false;
    }
    a = (int)r;
    return true;
}

// Calcule les coordonnees monde d'un coin du rectangle.
static void coin(long x, long y, const Rect &r, int angle,
                 long &rx, long &ry)
{
    long ax = (x - r.ox) * r.sx;
    long ay = (y - r.oy) * r.sy;

    long c, s;
    switch (angle)
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

    rx = r.px + ax * c - ay * s;
    ry = r.py + ax * s + ay * c;
}

int main()
{
    int N = 0;
    if (scanf("%d", &N) != 1)
    {
        return 0;
    }

    long refuses = 0;

    for (int i = 0; i < N; ++i)
    {
        Rect r;
        if (scanf("%63s %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                  r.nom, &r.w, &r.h, &r.px, &r.py,
                  &r.ox, &r.oy, &r.sx, &r.sy, &r.angle) != 10)
        {
            return 0;
        }

        int a;
        if (!normalise(r.angle, a))
        {
            printf("%s ANGLE REFUSE\n", r.nom);
            refuses++;
            continue;
        }

        long x[4], y[4];
        coin(0, 0, r, a, x[0], y[0]);
        coin(r.w, 0, r, a, x[1], y[1]);
        coin(r.w, r.h, r, a, x[2], y[2]);
        coin(0, r.h, r, a, x[3], y[3]);

        printf("%s COINS %ld %ld %ld %ld %ld %ld %ld %ld\n",
               r.nom, x[0], y[0], x[1], y[1], x[2], y[2], x[3], y[3]);

        long minx = x[0], maxx = x[0], miny = y[0], maxy = y[0];
        for (int j = 1; j < 4; ++j)
        {
            if (x[j] < minx)
            {
                minx = x[j];
            }
            if (x[j] > maxx)
            {
                maxx = x[j];
            }
            if (y[j] < miny)
            {
                miny = y[j];
            }
            if (y[j] > maxy)
            {
                maxy = y[j];
            }
        }
        printf("%s BOITE %ld %ld %ld %ld\n", r.nom, minx, miny, maxx, maxy);
    }

    printf("REFUSES %ld\n", refuses);
    return 0;
}
