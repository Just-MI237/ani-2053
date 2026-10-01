// Exercice 7 du chapitre 4 - La planche de sprites.
//
// Ce programme choisit la case d'une planche de sprites a partir du
// temps ecoule entre les images. Il plafonne dt pour resister au
// retour de veille, et revient a la case 0 apres la derniere case.
//
// Entree :
//   C R W H F D P
//   N
//   dt1 dt2 ... dtN
//
// Sortie :
//   une ligne par dt : case x y w h
//   puis AVANCES n et PLAFONNES n
//
// La regle :
//   l'animation commence a la case 0, temps accumule 0
//   pour chaque dt :
//     si dt > P, dt = P, PLAFONNES++
//     accumule += dt
//     tant que accumule >= D :
//       accumule -= D
//       case = (case + 1) % F
//       AVANCES++
//   la case c est en colonne c % C et ligne c / C
//   x = (c % C) * W, y = (c / C) * H, w = W, h = H

#include <cstdio>

int main()
{
    long long C = 0, R = 0, W = 0, H = 0, F = 0, D = 0, P = 0;
    if (scanf("%lld %lld %lld %lld %lld %lld %lld",
              &C, &R, &W, &H, &F, &D, &P) != 7)
    {
        return 0;
    }

    int N = 0;
    if (scanf("%d", &N) != 1)
    {
        return 0;
    }

    long long accumule = 0;
    long long caseCourante = 0;
    long long avances = 0;
    long long plafonnes = 0;

    for (int i = 0; i < N; ++i)
    {
        long long dt = 0;
        if (scanf("%lld", &dt) != 1)
        {
            return 0;
        }

        if (dt > P)
        {
            dt = P;
            plafonnes++;
        }

        accumule += dt;

        while (accumule >= D)
        {
            accumule -= D;
            caseCourante = (caseCourante + 1) % F;
            avances++;
        }

        long long col = caseCourante % C;
        long long lig = caseCourante / C;
        long long x = col * W;
        long long y = lig * H;

        printf("%lld %lld %lld %lld %lld\n", caseCourante, x, y, W, H);
    }

    printf("AVANCES %lld\n", avances);
    printf("PLAFONNES %lld\n", plafonnes);

    return 0;
}
