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
#include <string>

using namespace nkentseu;
using namespace nkentseu::net;

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
        NkConnectionManager manager;
        NkNetResult r = manager.StartServer(7777, 8);
        if (r != NkNetResult::NK_NET_OK)
        {
            std::printf("StartServer echoue : code %d\n", (int)r);
            return 1;
        }
        std::printf("Mode serveur, renvoie chaque message a son expediteur\n");

        for (int t = 0; t < 400; ++t)
        {
            NkVector<NkReceiveMsg> messages;
            manager.DrainAll(messages);
            for (nk_size i = 0; i < messages.Size(); ++i)
            {
                const NkReceiveMsg& m = messages[i];
                manager.SendTo(m.from, m.data, m.size);
            }
            NkChrono::SleepMilliseconds(10);
        }
        manager.Shutdown();
    }
    else if (mode == "client")
    {
        NkConnectionManager manager;
        NkNetResult r = manager.Connect(NkAddress("127.0.0.1", 7777));
        if (r != NkNetResult::NK_NET_OK)
        {
            std::printf("Connect echoue : code %d\n", (int)r);
            return 1;
        }
        std::printf("Mode client, cent allers-retours\n");

        NkChrono::SleepMilliseconds(2000);

        FILE* fichier = std::fopen("mesures-rtt.txt", "w");
        if (!fichier)
        {
            std::printf("Impossible d'ouvrir mesures-rtt.txt\n");
            return 1;
        }

        double somme = 0.0;
        double mini = 1e9;
        double maxi = 0.0;

        for (int k = 0; k < 100; ++k)
        {
            uint8 numero = (uint8)(k & 0xFF);
            NkChrono chrono;
            manager.SendTo(NkPeerId::Server(), &numero, 1);

            bool recu = false;
            for (int t = 0; t < 200 && !recu; ++t)
            {
                NkVector<NkReceiveMsg> messages;
                manager.DrainAll(messages);
                for (nk_size i = 0; i < messages.Size(); ++i)
                {
                    if (messages[i].size == 1 && messages[i].data[0] == numero)
                    {
                        recu = true;
                    }
                }
                if (!recu)
                {
                    NkChrono::SleepMilliseconds(1);
                }
            }

            double ms = chrono.Elapsed().milliseconds;
            std::fprintf(fichier, "%.3f\n", ms);
            somme += ms;
            if (ms < mini) mini = ms;
            if (ms > maxi) maxi = ms;
        }

        std::fclose(fichier);

        double moyenne = somme / 100.0;
        double srtt = 100.0;
        FILE* f2 = std::fopen("mesures-rtt.txt", "r");
        char ligne[64];
        while (f2 && std::fgets(ligne, sizeof(ligne), f2))
        {
            double r = std::atof(ligne);
            srtt = (1.0 - 0.125) * srtt + 0.125 * r;
        }
        if (f2) std::fclose(f2);

        NkChrono::SleepMilliseconds(500);
        NkConnectionStats stats;
        float rttMoteur = 0.f;
        if (manager.GetConnectionStats(NkPeerId::Server(), stats))
        {
            rttMoteur = stats.rttMs;
        }

        std::printf("Moyenne : %.3f\n", moyenne);
        std::printf("Minimum : %.3f\n", mini);
        std::printf("Maximum : %.3f\n", maxi);
        std::printf("Lissee : %.3f\n", srtt);
        std::printf("Moteur : %.3f\n", (double)rttMoteur);
        std::printf("Ecart : %.3f\n", moyenne - (double)rttMoteur);

        manager.Shutdown();
    }
    else
    {
        std::printf("usage : TestRtt serveur|client\n");
    }

    NkSocket::PlatformShutdown();
    return 0;
}
