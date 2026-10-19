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

    NkConnectionManager manager;

    manager.onPeerConnected = [](NkPeerId peer) {
        std::printf("Pair connecte : value = %llu\n", (unsigned long long)peer.value);
    };

    manager.onPeerDisconnected = [](NkPeerId peer, const char* reason) {
        std::printf("Pair deconnecte : value = %llu, raison = %s\n",
                    (unsigned long long)peer.value, reason ? reason : "(aucune)");
    };

    std::string mode;
    if (state.args.Size() >= 2)
    {
        mode = state.args[1].CStr();
    }

    if (mode == "serveur")
    {
        std::printf("Mode serveur, port 7777\n");
        NkNetResult r = manager.StartServer(7777, 8);
        if (r != NkNetResult::NK_NET_OK)
        {
            std::printf("StartServer echoue : code %d\n", (int)r);
            return 1;
        }

        for (int t = 0; t < 100; ++t)
        {
            NkVector<NkReceiveMsg> messages;
            manager.DrainAll(messages);
            for (nk_size i = 0; i < messages.Size(); ++i)
            {
                const NkReceiveMsg& m = messages[i];
                std::printf("Message recu de value = %llu : %.*s\n",
                            (unsigned long long)m.from.value,
                            (int)m.size, (const char*)m.data);
            }
            NkChrono::SleepMilliseconds(100);
        }
    }
    else if (mode == "client")
    {
        std::printf("Mode client, vers 127.0.0.1:7777\n");
        NkNetResult r = manager.Connect(NkAddress("127.0.0.1", 7777));
        if (r != NkNetResult::NK_NET_OK)
        {
            std::printf("Connect echoue : code %d\n", (int)r);
            return 1;
        }

        NkChrono::SleepMilliseconds(2000);

        NkPeerId serveur = NkPeerId::Server();
        const char* texte = "bonjour du client";
        NkNetResult s = manager.SendTo(serveur,
                                       (const uint8*)texte,
                                       (uint32)std::strlen(texte),
                                       NkNetChannel::NK_NET_CHANNEL_RELIABLE_ORDERED);
        std::printf("SendTo vers value = %llu, resultat = %d\n",
                    (unsigned long long)serveur.value, (int)s);

        NkChrono::SleepMilliseconds(3000);
    }
    else
    {
        std::printf("usage : TestParle serveur|client\n");
        manager.Shutdown();
        NkSocket::PlatformShutdown();
        return 1;
    }

    manager.Shutdown();
    NkSocket::PlatformShutdown();
    return 0;
}
