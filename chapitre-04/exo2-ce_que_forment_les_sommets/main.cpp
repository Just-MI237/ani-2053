// Exercice 2 du chapitre 4 - Ce que forment les sommets.
//
// Ce programme lit des listes de sommets et dit ce que chacune forme,
// selon son type de primitive. Pour un type accepte, il donne le nombre
// d'unites (points, segments ou triangles), et le nombre de sommets
// restants qui ne forment rien. Pour un type refuse, il le signale.
//
// Entree :
//   N
//   type1 s1
//   type2 s2
//   ...
//
// Sortie :
//   une ligne par entree
//   puis quatre bilans : POINTS, SEGMENTS, TRIANGLES, REFUSES
//
// La regle des restants :
//   POINTS          : s points, 0 restant
//   LINES           : s / 2 segments, s % 2 restants
//   LINE_STRIP      : s - 1 segments si s >= 2, sinon 0 et s restants
//   TRIANGLES       : s / 3 triangles, s % 3 restants
//   TRIANGLE_STRIP  : s - 2 triangles si s >= 3, sinon 0 et s restants
//   TRIANGLE_FAN    : idem TRIANGLE_STRIP
//   tout autre type : REFUSE (comparaison exacte, QUADS compris)

#include <cstdio>
#include <cstring>

// Compte ce que forme une liste de sommets selon son type.
// Retourne le nombre d'unites, ou -1 si le type est refuse.
// Renseigne restants (sommets qui ne forment rien) et unite (POINTS,
// SEGMENTS ou TRIANGLES) par reference.
static int compte(const char *type, int s, int &restants, const char *&unite)
{
    restants = 0;
    unite = "";

    if (strcmp(type, "POINTS") == 0)
    {
        unite = "POINTS";
        return s;
    }
    if (strcmp(type, "LINES") == 0)
    {
        unite = "SEGMENTS";
        restants = s % 2;
        return s / 2;
    }
    if (strcmp(type, "LINE_STRIP") == 0)
    {
        unite = "SEGMENTS";
        if (s >= 2)
        {
            return s - 1;
        }
        restants = s;
        return 0;
    }
    if (strcmp(type, "TRIANGLES") == 0)
    {
        unite = "TRIANGLES";
        restants = s % 3;
        return s / 3;
    }
    if (strcmp(type, "TRIANGLE_STRIP") == 0 || strcmp(type, "TRIANGLE_FAN") == 0)
    {
        unite = "TRIANGLES";
        if (s >= 3)
        {
            return s - 2;
        }
        restants = s;
        return 0;
    }
    return -1;  // type inconnu, refuse
}

int main()
{
    int N = 0;
    if (scanf("%d", &N) != 1)
    {
        return 0;
    }

    long totalPoints = 0, totalSegments = 0, totalTriangles = 0, totalRefuses = 0;

    for (int i = 0; i < N; ++i)
    {
        char type[64];
        int s;
        if (scanf("%63s %d", type, &s) != 2)
        {
            return 0;
        }

        int restants;
        const char *unite;
        int n = compte(type, s, restants, unite);

        if (n < 0)
        {
            printf("%s %d REFUSE\n", type, s);
            totalRefuses++;
            continue;
        }

        printf("%s %d %d %s %d\n", type, s, n, unite, restants);

        if (strcmp(unite, "POINTS") == 0)
        {
            totalPoints += n;
        }
        else if (strcmp(unite, "SEGMENTS") == 0)
        {
            totalSegments += n;
        }
        else if (strcmp(unite, "TRIANGLES") == 0)
        {
            totalTriangles += n;
        }
    }

    printf("POINTS %ld\n", totalPoints);
    printf("SEGMENTS %ld\n", totalSegments);
    printf("TRIANGLES %ld\n", totalTriangles);
    printf("REFUSES %ld\n", totalRefuses);

    return 0;
}
