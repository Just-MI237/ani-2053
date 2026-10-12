// Datation de la mesure
//
// Commande : git log -1 --format="%h %ad" --date=short
// Sortie brute : 9c3fad33 2026-09-13
// Version Jenga : 2.8.0.
//
#include <iostream>
#include <string>
#include <vector>

struct Piste
{
    std::string type;
    std::string code;
};

struct Fichier
{
    std::string nom;
    std::vector<unsigned char> octets;
    std::vector<Piste> pistes;
    std::string conteneur;
};

static std::vector<unsigned char> decoderHex(const std::string& s)
{
    std::vector<unsigned char> out;
    if (s == "-")
    {
        return out;
    }
    for (std::size_t i = 0; i + 1 < s.size(); i += 2)
    {
        auto val = [](char c) -> int {
            if (c >= '0' && c <= '9') return c - '0';
            if (c >= 'a' && c <= 'f') return c - 'a' + 10;
            if (c >= 'A' && c <= 'F') return c - 'A' + 10;
            return 0;
        };
        int hi = val(s[i]);
        int lo = val(s[i + 1]);
        out.push_back((unsigned char)((hi << 4) | lo));
    }
    return out;
}

static bool commencePar(const std::vector<unsigned char>& b, std::size_t pos,
                        const std::vector<unsigned char>& motif)
{
    if (b.size() < pos + motif.size())
    {
        return false;
    }
    for (std::size_t i = 0; i < motif.size(); ++i)
    {
        if (b[pos + i] != motif[i])
        {
            return false;
        }
    }
    return true;
}

static std::string reconnaitreConteneur(const std::vector<unsigned char>& b)
{
    std::size_t n = b.size();

    if (n >= 12 && commencePar(b, 4, {0x66, 0x74, 0x79, 0x70}))
    {
        return "MP4";
    }
    if (n >= 4 && commencePar(b, 0, {0x1A, 0x45, 0xDF, 0xA3}))
    {
        return "WEBM";
    }
    if (n >= 12 && commencePar(b, 0, {0x52, 0x49, 0x46, 0x46}) &&
        commencePar(b, 8, {0x57, 0x41, 0x56, 0x45}))
    {
        return "WAV";
    }
    if (n >= 4 && commencePar(b, 0, {0x4F, 0x67, 0x67, 0x53}))
    {
        return "OGG";
    }
    if (n >= 4 && commencePar(b, 0, {0x66, 0x4C, 0x61, 0x43}))
    {
        return "FLAC";
    }
    if (n >= 3 && commencePar(b, 0, {0x49, 0x44, 0x33}))
    {
        return "MP3";
    }
    if (n >= 2 && b[0] == 0xFF && b[1] >= 0xE0)
    {
        return "MP3";
    }
    return "INCONNU";
}

static std::string nommerCodec(const std::string& code)
{
    if (code == "mp4a") return "aac";
    if (code == "Opus" || code == "opus") return "opus";
    if (code == "avc1" || code == "avc3") return "h264";
    if (code == "hvc1" || code == "hev1") return "h265";
    if (code == "vp08") return "vp8";
    if (code == "vp09") return "vp9";
    if (code == "mp4v") return "mpeg4";
    if (code == ".mp3") return "mp3";
    if (code == "twos" || code == "sowt" || code == "lpcm") return "pcm";
    return code;
}

static bool codecLisible(const std::string& code)
{
    return code == "mjpa" || code == "jpeg" || code == "MJPG" ||
           code == "avc1" || code == "avc3" || code == "hvc1" ||
           code == "hev1" || code == "av01";
}

int main()
{
    int n = 0;
    std::cin >> n;

    std::vector<Fichier> fichiers;
    fichiers.resize(n);

    for (int i = 0; i < n; ++i)
    {
        std::string hex;
        int p = 0;
        std::cin >> fichiers[i].nom >> hex >> p;
        fichiers[i].octets = decoderHex(hex);
        fichiers[i].conteneur = reconnaitreConteneur(fichiers[i].octets);
        fichiers[i].pistes.resize(p);
        for (int j = 0; j < p; ++j)
        {
            std::cin >> fichiers[i].pistes[j].type >> fichiers[i].pistes[j].code;
        }
    }

    int nombreMp4 = 0;
    int lisibles = 0;
    int inconnus = 0;

    for (const Fichier& f : fichiers)
    {
        std::cout << f.nom << " " << f.conteneur << "\n";

        if (f.conteneur == "INCONNU")
        {
            inconnus++;
            continue;
        }

        if (f.conteneur != "MP4")
        {
            continue;
        }

        nombreMp4++;

        for (std::size_t j = 0; j < f.pistes.size(); ++j)
        {
            std::string typeAffiche = "AUDIO";
            if (f.pistes[j].type == "vide")
            {
                typeAffiche = "VIDEO";
            }
            std::cout << f.nom << " PISTE " << (j + 1) << " "
                      << typeAffiche << " " << nommerCodec(f.pistes[j].code) << "\n";
        }

        std::string verdict = "SANS_IMAGE";
        for (const Piste& p : f.pistes)
        {
            if (p.type == "vide")
            {
                verdict = codecLisible(p.code) ? "LISIBLE" : "ECHEC";
                break;
            }
        }

        std::cout << f.nom << " LECTEUR " << verdict << "\n";

        if (verdict == "LISIBLE")
        {
            lisibles++;
        }
    }

    std::cout << "MP4 " << nombreMp4 << "\n";
    std::cout << "LISIBLES " << lisibles << "\n";
    std::cout << "INCONNUS " << inconnus << "\n";

    return 0;
}
