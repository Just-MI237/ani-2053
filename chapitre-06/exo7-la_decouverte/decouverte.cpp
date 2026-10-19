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
        NkDiscovery::ServerInfo info;
        info.addr = NkAddress::Any(7777);
        std::strncpy(info.name, "Serveur Test", sizeof(info.name) - 1);
        info.playerCount = 0;
        info.maxPlayers = 16;

        std::printf("Mode serveur, diffusion toutes les 2 secondes\n");
        std::printf("Nom : %s, maxPlayers : %u\n", info.name, info.maxPlayers);

        for (int i = 0; i < 30; ++i)
        {
            NkNetResult r = NkDiscovery::Broadcast(info);
            std::printf("Broadcast %d, resultat = %d\n", i + 1, (int)r);
            NkChrono::SleepMilliseconds(2000);
        }
    }
    else if (mode == "client")
    {
        std::printf("Mode client, ecoute 6 secondes\n");
        NkVector<NkDiscovery::ServerInfo> trouves;
        NkNetResult r = NkDiscovery::Listen(6000, trouves);
        std::printf("Listen resultat = %d, %d serveur(s) trouve(s)\n",
                    (int)r, (int)trouves.Size());
        for (nk_size i = 0; i < trouves.Size(); ++i)
        {
            const NkDiscovery::ServerInfo& s = trouves[i];
            std::printf("Serveur %d : adresse = %s, nom = %s, maxPlayers = %u\n",
                        (int)(i + 1), s.addr.ToString().CStr(), s.name, s.maxPlayers);
        }
    }
    else
    {
        std::printf("usage : TestDecouverte serveur|client\n");
        NkSocket::PlatformShutdown();
        return 1;
    }

    NkSocket::PlatformShutdown();
    return 0;
}
