// Exercice 5 du chapitre 4 - Le cercle qui n'en est pas un.
//
// Ce programme calcule, pour un cercle dessine comme un polygone, le
// plus grand ecart entre le vrai cercle et le polygone, puis a partir
// de quel agrandissement cet ecart atteint un pixel.
//
// Entree :
//   N
//   r1 n1
//   r2 n2
//   ...
//
// Sortie :
//   une ligne par cercle : r n ecart zoom verdict
//   puis VISIBLES n et REFUSES n
//
// La regle :
//   pi = 3.141592653589793
//   moins de 3 segments : REFUSE
//   ecart en pixels : g = r * (1 - cos(pi / n))
//   ecart en milliemes : g * 1000, arrondi vers le bas
//   si g vaut 0 : JAMAIS
//   sinon zoom : 100 / g, arrondi vers le haut
//   verdict VISIBLE si zoom <= 100, sinon INVISIBLE
//   VISIBLES compte les VISIBLE, REFUSES compte les refus

#include <cstdio>
#include <cmath>

static const double PI = 3.141592653589793;

int main()
{
    int N = 0;
    if (scanf("%d", &N) != 1)
    {
        return 0;
    }

    long visibles = 0;
    long refuses = 0;

    for (int i = 0; i < N; ++i)
    {
        long r, n;
        if (scanf("%ld %ld", &r, &n) != 2)
        {
            return 0;
        }

        if (n < 3)
        {
            printf("%ld %ld REFUSE\n", r, n);
            refuses++;
            continue;
        }

        double g = (double)r * (1.0 - cos(PI / (double)n));

        if (g == 0.0)
        {
            printf("%ld %ld ecart JAMAIS\n", r, n);
            continue;
        }

        long ecart = (long)floor(g * 1000.0);
        long zoom = (long)ceil(100.0 / g);

        const char *verdict = (zoom <= 100) ? "VISIBLE" : "INVISIBLE";
        if (zoom <= 100)
        {
            visibles++;
        }

        printf("%ld %ld %ld %ld %s\n", r, n, ecart, zoom, verdict);
    }

    printf("VISIBLES %ld\n", visibles);
    printf("REFUSES %ld\n", refuses);
    return 0;
}
