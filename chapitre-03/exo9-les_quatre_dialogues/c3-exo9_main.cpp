#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"

#include <thread>
#include <chrono>

using namespace nkentseu;

int nkmain(const NkEntryState &state) {
    NkWindowConfig cfg;
    cfg.title = "Quatre dialogues";
    cfg.width = 640;
    cfg.height = 480;
    NkWindow window(cfg);
    if (!window.IsOpen()) {
        return -1;
    }

    logger.Info("=== Dialogue 1 : OpenFileDialog ===");
    NkDialogResult r1 = NkDialogs::OpenFileDialog("*.txt;*.md", "Choisir un fichier");
    logger.Infof("confirmed=%d, path=\"%s\"", (int)r1.confirmed, r1.path.CStr());

    logger.Info("=== Dialogue 2 : SaveFileDialog ===");
    NkDialogResult r2 = NkDialogs::SaveFileDialog("txt", "Enregistrer sous");
    logger.Infof("confirmed=%d, path=\"%s\"", (int)r2.confirmed, r2.path.CStr());

    logger.Info("=== Dialogue 3 : OpenFolderDialog ===");
    NkDialogResult r3 = NkDialogs::OpenFolderDialog("Choisir un dossier");
    logger.Infof("confirmed=%d, path=\"%s\"", (int)r3.confirmed, r3.path.CStr());

    logger.Info("=== Dialogue 4 : OpenMessageBox ===");
    NkDialogs::OpenMessageBox("Ceci est un message d'information.", "Information", 0);
    logger.Info("OpenMessageBox retourne, programme toujours vivant");

    logger.Info("=== Fin des quatre dialogues ===");

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
