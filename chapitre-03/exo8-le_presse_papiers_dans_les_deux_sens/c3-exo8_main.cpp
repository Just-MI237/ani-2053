#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKEvent/NkEventSystem.h"
#include "NKLogger/NkLog.h"

#include <thread>
#include <chrono>

using namespace nkentseu;

int nkmain(const NkEntryState &state) {
    NkWindowConfig cfg;
    cfg.title = "Presse-papiers";
    cfg.width = 640;
    cfg.height = 480;
    NkWindow window(cfg);
    if (!window.IsOpen()) {
        return -1;
    }

    logger.Info("=== Phase texte ===");

    NkString initial = window.GetClipboardText();
    logger.Infof("Presse-papiers texte au demarrage : \"%s\" (taille %d)",
        initial.CStr(), (int)initial.Size());

    window.SetClipboardText(NkString("bonjour le monde"));
    NkString afterSet = window.GetClipboardText();
    logger.Infof("Apres SetClipboardText(bonjour le monde) : \"%s\"",
        afterSet.CStr());

    NkString upper = afterSet;
    for (usize i = 0; i < upper.Size(); ++i) {
        char c = upper[i];
        if (c >= 'a' && c <= 'z') {
            upper[i] = (char)(c - 32);
        }
    }
    window.SetClipboardText(upper);
    NkString finalText = window.GetClipboardText();
    logger.Infof("Apres mise en majuscules : \"%s\"", finalText.CStr());

    logger.Info("=== Phase image ===");

    bool hasBefore = window.HasClipboardImage();
    logger.Infof("Image disponible au demarrage : %d", (int)hasBefore);

    NkClipboardImage img;
    img.width = 4;
    img.height = 4;
    img.pixels.Resize(4 * 4 * 4);
    for (uint32 i = 0; i < 4 * 4; ++i) {
        img.pixels[i * 4 + 0] = 200;
        img.pixels[i * 4 + 1] = 100;
        img.pixels[i * 4 + 2] = 50;
        img.pixels[i * 4 + 3] = 255;
    }
    bool setOk = window.SetClipboardImage(img);
    logger.Infof("SetClipboardImage 4x4 RGBA=(200,100,50,255) : %d", (int)setOk);

    NkClipboardImage readBack;
    bool getOk = window.GetClipboardImage(readBack);
    logger.Infof("GetClipboardImage : %d, dims=%ux%u, octets=%d, bpp=32",
        (int)getOk, readBack.width, readBack.height, (int)readBack.pixels.Size());

    if (getOk && readBack.IsValid()) {
        logger.Infof("Premier pixel lu : RGBA=(%d,%d,%d,%d)",
            readBack.pixels[0], readBack.pixels[1], readBack.pixels[2], readBack.pixels[3]);

        for (uint32 i = 0; i < readBack.width * readBack.height; ++i) {
            readBack.pixels[i * 4 + 0] = (uint8)(255 - readBack.pixels[i * 4 + 0]);
            readBack.pixels[i * 4 + 1] = (uint8)(255 - readBack.pixels[i * 4 + 1]);
            readBack.pixels[i * 4 + 2] = (uint8)(255 - readBack.pixels[i * 4 + 2]);
        }
        bool setInvOk = window.SetClipboardImage(readBack);
        logger.Infof("SetClipboardImage inverse : %d", (int)setInvOk);

        NkClipboardImage finalImg;
        bool finalOk = window.GetClipboardImage(finalImg);
        logger.Infof("Lecture finale : %d, dims=%ux%u", (int)finalOk, finalImg.width, finalImg.height);
        if (finalOk && finalImg.IsValid()) {
            logger.Infof("Premier pixel final : RGBA=(%d,%d,%d,%d)",
                finalImg.pixels[0], finalImg.pixels[1], finalImg.pixels[2], finalImg.pixels[3]);
        }
    }

    auto start = std::chrono::steady_clock::now();
    while (window.IsOpen()) {
        NkEvents().PollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (elapsed >= 10) break;
    }
    return 0;
}
