#include <iostream>
#include <string>
#include <vector>
#include <cctype>

struct Fichier
{
    std::string nom;
    long long taille;
    std::vector<unsigned char> octets;
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

static std::string extensionMinuscule(const std::string& nom)
{
    std::size_t pos = nom.find_last_of('.');
    if (pos == std::string::npos)
    {
        return "";
    }
    std::string ext = nom.substr(pos + 1);
    for (char& c : ext)
    {
        c = (char)std::tolower((unsigned char)c);
    }
    return ext;
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

static std::string reconnaitreFormat(const Fichier& f)
{
    const std::vector<unsigned char>& b = f.octets;

    if (f.taille < 4)
    {
        return "";
    }

    if (f.taille >= 8 && commencePar(b, 0, {0x89, 0x50, 0x4E, 0x47}))
    {
        return "PNG";
    }
    if (commencePar(b, 0, {0xFF, 0xD8, 0xFF}))
    {
        return "JPEG";
    }
    if (commencePar(b, 0, {0x42, 0x4D}))
    {
        return "BMP";
    }
    if (commencePar(b, 0, {0x71, 0x6F, 0x69, 0x66}))
    {
        return "QOI";
    }
    if (commencePar(b, 0, {0x47, 0x49, 0x46, 0x38}))
    {
        return "GIF";
    }
    if (b.size() >= 4 && b[0] == 0x00 && b[1] == 0x00 &&
        (b[2] == 0x01 || b[2] == 0x02) && b[3] == 0x00)
    {
        return "ICO";
    }
    if (f.taille >= 10 && commencePar(b, 0, {0x23, 0x3F}))
    {
        return "HDR";
    }
    if (commencePar(b, 0, {0x76, 0x2F, 0x31, 0x01}))
    {
        return "EXR";
    }
    if (b.size() >= 2 && b[0] == 0x50 && b[1] >= 0x31 && b[1] <= 0x36)
    {
        if (b[1] == 0x31 || b[1] == 0x34) return "PBM";
        if (b[1] == 0x32 || b[1] == 0x35) return "PGM";
        if (b[1] == 0x33 || b[1] == 0x36) return "PPM";
    }
    if (f.taille >= 18 && b.size() >= 3)
    {
        unsigned char c2 = b[2];
        if (c2 == 0x00 || c2 == 0x01 || c2 == 0x02 || c2 == 0x03 ||
            c2 == 0x09 || c2 == 0x0A || c2 == 0x0B)
        {
            return "TGA";
        }
    }

    std::size_t i = 0;
    if (commencePar(b, 0, {0xEF, 0xBB, 0xBF}))
    {
        i = 3;
    }
    while (i < b.size() && (b[i] == 0x20 || b[i] == 0x09 ||
                            b[i] == 0x0A || b[i] == 0x0D))
    {
        ++i;
    }
    if (commencePar(b, i, {0x3C, 0x3F, 0x78, 0x6D, 0x6C}) ||
        commencePar(b, i, {0x3C, 0x73, 0x76, 0x67}))
    {
        return "SVG";
    }

    return "";
}

static bool extensionJuste(const std::string& format, const std::string& ext)
{
    if (format == "PNG") return ext == "png";
    if (format == "JPEG") return ext == "jpg" || ext == "jpeg";
    if (format == "BMP") return ext == "bmp";
    if (format == "QOI") return ext == "qoi";
    if (format == "GIF") return ext == "gif";
    if (format == "ICO") return ext == "ico" || ext == "cur";
    if (format == "HDR") return ext == "hdr";
    if (format == "EXR") return ext == "exr";
    if (format == "PBM") return ext == "pbm";
    if (format == "PGM") return ext == "pgm";
    if (format == "PPM") return ext == "ppm";
    if (format == "TGA") return ext == "tga";
    if (format == "SVG") return ext == "svg";
    return false;
}

int main()
{
    int n = 0;
    if (!(std::cin >> n))
    {
        return 0;
    }

    std::vector<Fichier> fichiers;
    fichiers.resize(n);

    for (int i = 0; i < n; ++i)
    {
        std::string hex;
        std::cin >> fichiers[i].nom >> fichiers[i].taille >> hex;
        fichiers[i].octets = decoderHex(hex);
    }

    int lus = 0;
    int mensonges = 0;
    int refuses = 0;

    for (const Fichier& f : fichiers)
    {
        std::string format = reconnaitreFormat(f);
        if (format.empty())
        {
            std::cout << f.nom << " REFUSE\n";
            refuses++;
            continue;
        }
        std::string ext = extensionMinuscule(f.nom);
        if (extensionJuste(format, ext))
        {
            std::cout << f.nom << " " << format << " OK\n";
            lus++;
        }
        else
        {
            std::cout << f.nom << " " << format << " MENT\n";
            lus++;
            mensonges++;
        }
    }

    std::cout << "LUS " << lus << "\n";
    std::cout << "MENSONGES " << mensonges << "\n";
    std::cout << "REFUSES " << refuses << "\n";

    return 0;
}
