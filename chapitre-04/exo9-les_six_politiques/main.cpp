// Exercice 9 du chapitre 4 - Les six politiques.
//
// Ce programme calcule, pour chacune des six politiques de
// redimensionnement, le viewport et la taille du monde visible.
//
// Entree :
//   RW RH AW AH W H
//
// Sortie :
//   six lignes NOM vx vy vw vh mw mh
//   puis BANDES n et DEFORMATION OUI ou NON

#include <cstdio>

// Arrondi au plus proche, une moitie monte.
static long long arrondi(long long a, long long b)
{
    return (2 * a + b) / (2 * b);
}

int main()
{
    long long RW = 0, RH = 0, AW = 0, AH = 0, W = 0, H = 0;
    if (scanf("%lld %lld %lld %lld %lld %lld",
              &RW, &RH, &AW, &AH, &W, &H) != 6)
    {
        return 0;
    }

    bool haveRef = (RW > 0 && RH > 0);

    long long vx[6], vy[6], vw[6], vh[6], mw[6], mh[6];

    vx[0] = 0;
    vy[0] = 0;
    vw[0] = W;
    vh[0] = H;
    mw[0] = W;
    mh[0] = H;

    if (haveRef)
    {
        vx[1] = 0;
        vy[1] = 0;
        vw[1] = W;
        vh[1] = H;
        mw[1] = RW;
        mh[1] = RH;
    }
    else
    {
        vx[1] = 0;
        vy[1] = 0;
        vw[1] = W;
        vh[1] = H;
        mw[1] = W;
        mh[1] = H;
    }

    if (haveRef)
    {
        if (W * RH <= H * RW)
        {
            vw[2] = W;
            vh[2] = arrondi(RH * W, RW);
        }
        else
        {
            vh[2] = H;
            vw[2] = arrondi(RW * H, RH);
        }
        vx[2] = (W - vw[2]) / 2;
        vy[2] = (H - vh[2]) / 2;
        mw[2] = RW;
        mh[2] = RH;
    }
    else
    {
        vx[2] = 0;
        vy[2] = 0;
        vw[2] = W;
        vh[2] = H;
        mw[2] = W;
        mh[2] = H;
    }

    if (haveRef && W >= RW && H >= RH)
    {
        long long k1 = W / RW;
        long long k2 = H / RH;
        long long k = (k1 < k2) ? k1 : k2;
        vw[3] = RW * k;
        vh[3] = RH * k;
        vx[3] = (W - vw[3]) / 2;
        vy[3] = (H - vh[3]) / 2;
        mw[3] = RW;
        mh[3] = RH;
    }
    else if (haveRef)
    {
        vx[3] = vx[2];
        vy[3] = vy[2];
        vw[3] = vw[2];
        vh[3] = vh[2];
        mw[3] = mw[2];
        mh[3] = mh[2];
    }
    else
    {
        vx[3] = 0;
        vy[3] = 0;
        vw[3] = W;
        vh[3] = H;
        mw[3] = W;
        mh[3] = H;
    }

    if (haveRef)
    {
        vx[4] = 0;
        vy[4] = 0;
        vw[4] = W;
        vh[4] = H;
        if (W * RH > H * RW)
        {
            mw[4] = RW;
            mh[4] = arrondi(RW * H, W);
        }
        else
        {
            mw[4] = arrondi(RH * W, H);
            mh[4] = RH;
        }
    }
    else
    {
        vx[4] = 0;
        vy[4] = 0;
        vw[4] = W;
        vh[4] = H;
        mw[4] = W;
        mh[4] = H;
    }

    vx[5] = 0;
    vy[5] = 0;
    vw[5] = AW;
    vh[5] = AH;
    mw[5] = AW;
    mh[5] = AH;

    const char *noms[6] = {
        "FOLLOW_WINDOW", "STRETCH", "FIT_LETTERBOX",
        "INTEGER_SCALE", "FIT_CROP", "MANUAL"
    };

    for (int i = 0; i < 6; ++i)
    {
        printf("%s %lld %lld %lld %lld %lld %lld\n",
               noms[i], vx[i], vy[i], vw[i], vh[i], mw[i], mh[i]);
    }

    long long bandes = 0;
    for (int i = 0; i < 6; ++i)
    {
        if (vw[i] < W || vh[i] < H)
        {
            bandes++;
        }
    }

    bool deform = haveRef && (W * RH != H * RW);

    printf("BANDES %lld\n", bandes);
    printf("DEFORMATION %s\n", deform ? "OUI" : "NON");

    return 0;
}
