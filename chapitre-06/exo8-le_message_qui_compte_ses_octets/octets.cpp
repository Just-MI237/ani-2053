// Datation de la mesure
//
// Commande : git log -1 --format="%h %ad" --date=short
// Sortie brute : 039421b1 2026-10-15
// Version Jenga : 2.9.0.

#include "NKNetwork/NKNetwork.h"
#include "NKWindow/NKMain.h"
#include "NKTime/NkChrono.h"

#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>

using namespace nkentseu;
using namespace nkentseu::net;

static const bool VIVANT = true;
static const uint8 NUMERO = 42;
static const float X = 123.456f;
static const float Y = -67.891f;
static const float ANGLE = 47.3f;
static const int32 SANTE = 73;

static void ecrireComplet(uint8* buffer, uint32& taille)
{
    NkBitWriter writer(buffer, 64);
    writer.WriteU8(VIVANT ? 1 : 0);
    writer.WriteU8(NUMERO);
    writer.WriteF32(X);
    writer.WriteF32(Y);
    writer.WriteF32(ANGLE);
    writer.WriteI32(SANTE);
    std::printf("Types complets : %u octets, %u bits\n",
                writer.BytesWritten(), writer.BitsWritten());
    taille = writer.BytesWritten();
}

static void ecrireQuantifie(uint8* buffer, uint32& taille)
{
    NkBitWriter writer(buffer, 64);
    writer.WriteBool(VIVANT);
    writer.WriteU8(NUMERO);
    writer.WriteF32Q(X, -500.f, 500.f, 0.01f);
    writer.WriteF32Q(Y, -500.f, 500.f, 0.01f);
    writer.WriteF32Q(ANGLE, 0.f, 360.f, 0.5f);
    writer.WriteInt(SANTE, 0, 100);
    std::printf("Quantifie : %u octets, %u bits\n",
                writer.BytesWritten(), writer.BitsWritten());
    taille = writer.BytesWritten();
}

static void lireComplet(const uint8* buffer, uint32 taille)
{
    NkBitReader reader(buffer, taille);
    uint8 vivant = reader.ReadU8();
    uint8 numero = reader.ReadU8();
    float x = reader.ReadF32();
    float y = reader.ReadF32();
    float angle = reader.ReadF32();
    int32 sante = reader.ReadI32();
    std::printf("Relu complet : vivant %u numero %u x %.4f y %.4f angle %.4f sante %d\n",
                vivant, numero, x, y, angle, sante);
}

static void lireQuantifie(const uint8* buffer, uint32 taille)
{
    NkBitReader reader(buffer, taille);
    bool vivant = reader.ReadBool();
    uint8 numero = reader.ReadU8();
    float x = reader.ReadF32Q(-500.f, 500.f, 0.01f);
    float y = reader.ReadF32Q(-500.f, 500.f, 0.01f);
    float angle = reader.ReadF32Q(0.f, 360.f, 0.5f);
    int32 sante = reader.ReadInt(0, 100);
    std::printf("Relu quantifie : vivant %d numero %u x %.4f y %.4f angle %.4f sante %d\n",
                vivant ? 1 : 0, numero, x, y, angle, sante);
    std::printf("Ecarts : x %.4f y %.4f angle %.4f sante %.4f\n",
                std::fabs(x - X), std::fabs(y - Y), std::fabs(angle - ANGLE),
                std::fabs((double)sante - (double)SANTE));
}

int nkmain(const NkEntryState &state)
{
    NkNetResult initResult = NkSocket::PlatformInit();
    if (initResult != NkNetResult::NK_NET_OK)
    {
        std::printf("PlatformInit echoue : code %d\n", (int)initResult);
        return 1;
    }

    std::string mode;
    if (state.args.Size() >= 2)
    {
        mode = state.args[1].CStr();
    }

    if (mode == "serveur")
    {
        uint8 bufferComplet[64];
        uint8 bufferQuantifie[64];
        uint32 tailleComplet = 0;
        uint32 tailleQuantifie = 0;

        ecrireComplet(bufferComplet, tailleComplet);
        ecrireQuantifie(bufferQuantifie, tailleQuantifie);

        NkConnectionManager manager;
        NkNetResult r = manager.StartServer(7777, 8);
        if (r != NkNetResult::NK_NET_OK)
        {
            std::printf("StartServer echoue\n");
            return 1;
        }
        std::printf("Mode serveur, en attente du client\n");

        bool envoye = false;
        for (int t = 0; t < 200 && !envoye; ++t)
        {
            NkChrono::SleepMilliseconds(50);
            NkPeerId cible = NkPeerId::Invalid();
            NkVector<NkReceiveMsg> messages;
            manager.DrainAll(messages);
            for (nk_size i = 0; i < messages.Size(); ++i)
            {
                cible = messages[i].from;
            }
            if (cible.IsValid())
            {
                manager.SendTo(cible, bufferComplet, tailleComplet);
                manager.SendTo(cible, bufferQuantifie, tailleQuantifie);
                std::printf("Envoye complet %u octets, quantifie %u octets a value %llu\n",
                            tailleComplet, tailleQuantifie, (unsigned long long)cible.value);
                envoye = true;
            }
        }

        NkChrono::SleepMilliseconds(2000);
        manager.Shutdown();
    }
    else if (mode == "client")
    {
        NkConnectionManager manager;
        NkNetResult r = manager.Connect(NkAddress("127.0.0.1", 7777));
        if (r != NkNetResult::NK_NET_OK)
        {
            std::printf("Connect echoue\n");
            return 1;
        }
        std::printf("Mode client, connecte\n");

        NkChrono::SleepMilliseconds(1000);

        NkPeerId serveur = NkPeerId::Server();
        uint8 ping[1] = {0};
        manager.SendTo(serveur, ping, 1);

        int recus = 0;
        for (int t = 0; t < 40 && recus < 2; ++t)
        {
            NkChrono::SleepMilliseconds(50);
            NkVector<NkReceiveMsg> messages;
            manager.DrainAll(messages);
            for (nk_size i = 0; i < messages.Size(); ++i)
            {
                const NkReceiveMsg& m = messages[i];
                if (recus == 0)
                {
                    lireComplet(m.data, m.size);
                    recus++;
                }
                else
                {
                    lireQuantifie(m.data, m.size);
                    recus++;
                }
            }
        }

        NkChrono::SleepMilliseconds(500);
        manager.Shutdown();
    }
    else
    {
        std::printf("usage : TestOctets serveur|client\n");
    }

    NkSocket::PlatformShutdown();
    return 0;
}
