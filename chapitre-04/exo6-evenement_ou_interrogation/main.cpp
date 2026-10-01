// Exercice 6 du chapitre 4 - Evenement ou interrogation.
//
// Ce programme rejoue une sequence de touches de deux facons : par
// evenements (chaque appui compte) et par interrogation (l'etat est
// lu une fois par image). Il compare les deux.
//
// Entree :
//   v N
//   k1 e1_1 e1_2 ... e1_k1
//   k2 ...
//   ...
//
// Sortie :
//   une ligne par image : i xe xi
//   puis SAUTS EVENEMENTS n, SAUTS INTERROGATION n, MANQUES n
//
// La regle :
//   les deux carres partent de 0
//   pour chaque image, dans l'ordre :
//     lire les evenements un a un, mettre a jour l'etat des touches
//     par evenements : chaque +SPACE compte un saut
//     par evenements : chaque +RIGHT ajoute v, chaque +LEFT retire v
//     par interrogation, une fois les evenements lus :
//       si SPACE enfoncee, compter un saut
//       si RIGHT enfoncee, ajouter v
//       si LEFT enfoncee, retirer v
//     MANQUES compte les +SPACE d'une image ou SPACE n'est plus
//     enfoncee a la fin de l'image

#include <cstdio>
#include <cstring>

int main()
{
    long long v = 0;
    int N = 0;
    if (scanf("%lld %d", &v, &N) != 2)
    {
        return 0;
    }

    long long xe = 0;
    long long xi = 0;
    bool espaceEnfoncee = false;
    bool gaucheEnfoncee = false;
    bool droiteEnfoncee = false;

    long long sautsEv = 0;
    long long sautsInt = 0;
    long long manques = 0;

    for (int i = 1; i <= N; ++i)
    {
        int k = 0;
        if (scanf("%d", &k) != 1)
        {
            return 0;
        }

        long long plusEspaceCetteImage = 0;

        for (int j = 0; j < k; ++j)
        {
            char ev[64];
            if (scanf("%63s", ev) != 1)
            {
                return 0;
            }

            bool enfoncee = (ev[0] == '+');
            const char *nom = ev + 1;

            if (strcmp(nom, "SPACE") == 0)
            {
                espaceEnfoncee = enfoncee;
                if (enfoncee)
                {
                    sautsEv++;
                    plusEspaceCetteImage++;
                }
            }
            else if (strcmp(nom, "RIGHT") == 0)
            {
                droiteEnfoncee = enfoncee;
                if (enfoncee)
                {
                    xe += v;
                }
            }
            else if (strcmp(nom, "LEFT") == 0)
            {
                gaucheEnfoncee = enfoncee;
                if (enfoncee)
                {
                    xe -= v;
                }
            }
        }

        if (espaceEnfoncee)
        {
            sautsInt++;
        }
        if (droiteEnfoncee)
        {
            xi += v;
        }
        if (gaucheEnfoncee)
        {
            xi -= v;
        }

        if (!espaceEnfoncee && plusEspaceCetteImage > 0)
        {
            manques += plusEspaceCetteImage;
        }

        printf("%d %lld %lld\n", i, xe, xi);
    }

    printf("SAUTS EVENEMENTS %lld\n", sautsEv);
    printf("SAUTS INTERROGATION %lld\n", sautsInt);
    printf("MANQUES %lld\n", manques);

    return 0;
}
