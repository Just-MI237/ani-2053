Exercice 9 - Les quatre dialogues

J'ai ecrit un programme qui appelle les quatre dialogues natifs de
NkDialogs, l'un apres l'autre, et journalise ce que chacun retourne.
Les quatre dialogues sont OpenFileDialog, SaveFileDialog,
OpenFolderDialog et OpenMessageBox.

Le programme est depose a cote sous le nom c3-exo9_main.cpp. Il fait
47 lignes.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Les quatre dialogues appeles

Le fichier NkDialogs.h declare cinq fonctions statiques. Les quatre
premieres sont celles de l'enonce. La cinquieme, ColorPicker, n'est pas
utilisee ici.

    static NkDialogResult OpenFileDialog(const NkString &filter = "*.*", const NkString &title = "Open File");
    static NkDialogResult SaveFileDialog(const NkString &defaultExt = "", const NkString &title = "Save File", const NkString &initialDir = "");
    static NkDialogResult OpenFolderDialog(const NkString &title = "Selectionner un dossier");
    static void OpenMessageBox(const NkString &message, const NkString &title = "Message", int type = 0);

Trois retournent une structure NkDialogResult, qui contient trois
champs : confirmed (bool), path (NkString), color (uint32).
OpenMessageBox retourne void.

Etat de Zenity sur la machine

Le backend Linux de NkDialogs passe par Zenity. Le fichier NkDialogs.cpp
l'annonce a la ligne 6 de son en-tete :

    Implementations pour Windows, Linux (Zenity), macOS (osascript), et stubs.

Zenity est installe a /usr/bin/zenity. Mais il refuse de demarrer :

    zenity: error while loading shared libraries: libwebkit2gtk-4.0.so.37: cannot open shared object file: No such file or directory

Une bibliotheque lui manque. J'ai essaye de l'installer. Le miroir
Ubuntu sert un paquet dont le hash ne correspond pas, donc apt refuse
le telechargement. L'installation n'a pas abouti.

Zenity est donc present, mais il ne peut pas s'executer.

Resultat du test

Sortie brute du programme :

    === Dialogue 1 : OpenFileDialog ===
    confirmed=0, path=""
    === Dialogue 2 : SaveFileDialog ===
    confirmed=0, path=""
    === Dialogue 3 : OpenFolderDialog ===
    confirmed=0, path=""
    === Dialogue 4 : OpenMessageBox ===
    OpenMessageBox retourne, programme toujours vivant
    === Fin des quatre dialogues ===

Aucun des quatre dialogues n'a fait planter le programme. Chacun est
retourne normalement.

Pourquoi

Les trois dialogues fichier utilisent la fonction ExecCommand de
NkDialogs.cpp, qui lance Zenity via popen et lit sa sortie standard :

    static NkString ExecCommand(const char *cmd) {
        NkString result;
        FILE *pipe = popen(cmd, "r");
        if (!pipe)
            return result;
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        pclose(pipe);
        if (!result.Empty() && result.Back() == '\n')
            result.PopBack();
        return result;
    }

Comme Zenity ne s'execute pas, il n'ecrit rien sur sa sortie standard.
ExecCommand retourne une chaine vide. Puis les trois fonctions
appliquent la meme ligne :

    res.confirmed = !path.Empty();

Une chaine vide donne confirmed = false. Aucune n'essaie d'utiliser
path quand il est vide. Aucune ne dereference un pointeur invalide.

OpenMessageBox utilise system au lieu de popen :

    system(cmd.CStr());

Le shell retourne 127 (command not found), mais OpenMessageBox est
declaree void et n'utilise pas cette valeur. Elle rend la main tout de
suite. Le programme continue.

Ce que cela montre sur l'annulation

L'enonce demande de verifier qu'aucun dialogue ne fait planter le
programme quand l'utilisateur ferme la boite sans rien choisir.

Ici, la boite ne s'ouvre meme pas. Le resultat est le meme que si
l'utilisateur avait ferme sans rien choisir : la chaine path reste
vide, confirmed reste a false. Le programme continue sans erreur.

C'est le bon comportement. Une fonction qui retourne un NkDialogResult
doit toujours retourner une valeur coherente. confirmed=false est la
valeur par defaut quand rien n'a ete confirme. Le code appelant n'a
pas besoin de savoir pourquoi la confirmation n'a pas eu lieu : annule,
ou erreur, ou plateforme non supportee. La distinction n'existe pas
dans l'interface.

Tableau recapitulatif

| Dialogue | Type de retour | Valeur obtenue | Crash |
|----------|----------------|----------------|-------|
| OpenFileDialog | NkDialogResult | confirmed=0, path="" | non |
| SaveFileDialog | NkDialogResult | confirmed=0, path="" | non |
| OpenFolderDialog | NkDialogResult | confirmed=0, path="" | non |
| OpenMessageBox | void | aucun retour | non |

Les quatre dialogues gerent l'absence de confirmation de la meme
maniere : ils ne font rien de special, ils retournent l'etat par
defaut. La valeur par defaut de NkDialogResult (ligne 26 de
NkDialogs.h) est confirmed=false. C'est suffisant pour que l'appelant
sache que rien n'a ete choisi.

Ce que l'exercice m'apprend

Une interface qui retourne une structure de resultat est plus facile a
utiliser qu'une interface qui retourne void. Les trois dialogues
fichier ont un moyen simple de signaler l'echec : confirmed=false. Le
quatrieme, OpenMessageBox, n'a aucun moyen de signaler quoi que ce
soit. C'est logique pour une boite d'information, mais c'est un choix
qui coute : l'appelant ne sait pas si le message a ete vu ou si la
boite ne s'est pas ouverte.

Sur Windows, les quatre dialogues s'ouvrent vraiment. Le code ne change
pas. Ce qui change, c'est que la boite apparaisse ou pas, et que
l'annulation soit detectee par le retour de l'API systeme au lieu d'un
pipe vide.

Mesure faite le 27/09/2026.
Version de Jenga : 2.8.0.
