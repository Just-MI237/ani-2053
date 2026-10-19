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
#include <map>

using namespace nkentseu;
using namespace nkentseu::net;

static std::map<uint64, NkTimestampMs> derniersMessages;

static void afficherTable(const char* titre)
{
    std::printf("[%s] table joueurs (%d) :\n", titre, (int)derniersMessages.size());
    for (const auto& paire : derniersMessages)
    {
        std::printf("  joueur %llu, dernier message a %llu ms\n",
                    (unsigned long long)paire.first,
                    (unsigned long long)paire.second);
    }
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
        NkConnectionManager manager;

        manager.onPeerConnected = [](NkPeerId peer) {
            derniersMessages[peer.value] = NkNetNowMs();
            std::printf("[CONNECT] joueur %llu ajoute a la table\n",
                        (unsigned long long)peer.value);
            afficherTable("apres ajout");
            std::fflush(stdout);
        };

        manager.onPeerDisconnected = [](NkPeerId peer, const char* raison) {
            NkTimestampMs maintenant = NkNetNowMs();
            NkTimestampMs dernier = 0;
            auto it = derniersMessages.find(peer.value);
            if (it != derniersMessages.end())
            {
                dernier = it->second;
                derniersMessages.erase(it);
            }
            NkTimestampMs ecart = maintenant - dernier;
            std::printf("[DISCONNECT] joueur %llu retire, raison = %s, ecart = %llu ms\n",
                        (unsigned long long)peer.value,
                        raison ? raison : "(aucune)",
                        (unsigned long long)ecart);
            afficherTable("apres retrait");
            std::fflush(stdout);
        };

        NkNetResult r = manager.StartServer(7777, 8);
        if (r != NkNetResult::NK_NET_OK)
        {
            std::printf("StartServer echoue : code %d\n", (int)r);
            return 1;
        }
        std::printf("Mode serveur, table des joueurs active\n");
        std::fflush(stdout);

        for (int t = 0; t < 8000; ++t)
        {
            NkVector<NkReceiveMsg> messages;
            manager.DrainAll(messages);
            NkTimestampMs maintenant = NkNetNowMs();
            for (nk_size i = 0; i < messages.Size(); ++i)
            {
                const NkReceiveMsg& m = messages[i];
                derniersMessages[m.from.value] = maintenant;
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
        std::printf("Mode client, envoi toutes les 100 ms\n");

        NkChrono::SleepMilliseconds(3000);

        for (int k = 0; k < 10; ++k)
        {
            uint8 octet = (uint8)(k & 0xFF);
            manager.SendTo(NkPeerId::Server(), &octet, 1,
                           NkNetChannel::NK_NET_CHANNEL_UNRELIABLE);
            NkChrono::SleepMilliseconds(100);
        }

        std::printf("Fermeture propre par Shutdown\n");
        manager.Shutdown();
    }
    else
    {
        std::printf("usage : TestFantome serveur|client\n");
    }

    NkSocket::PlatformShutdown();
    return 0;
}
