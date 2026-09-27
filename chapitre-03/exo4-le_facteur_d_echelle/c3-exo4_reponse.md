Exercice 4 - Le facteur d'echelle

J'ai ecrit un programme qui affiche trois valeurs : la taille rendue par
la fenetre, celle rendue par la cible de rendu (le contexte graphique),
et le facteur d'echelle. J'ai lance le programme et note les resultats.

Le programme est depose a cote sous le nom c3-exo4_main.cpp. Il fait
46 lignes.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Le programme

Le programme cree une fenetre de 1280 x 720, puis un contexte graphique
OpenGL. Il lit ensuite trois valeurs :

- window.GetSize() pour la taille de la fenetre
- ctx->GetInfo().windowWidth et windowHeight pour la taille de la cible
  de rendu
- window.GetDpiScale() pour le facteur d'echelle

Les trois valeurs observees

Commande :

    timeout 5 jenga run TestEchelle --config Debug --platform x86_64 --target Linux

Sortie brute (extrait du journal) :

    Taille fenetre     : 1280 x 720
    Taille cible rendu : 1280 x 720
    Facteur d'echelle  : 1.0000

Tableau recapitulatif :

| Valeur | Mesure |
|--------|--------|
| Taille fenetre | 1280 x 720 |
| Taille cible rendu | 1280 x 720 |
| Facteur d'echelle | 1.0000 |

L'ecran donne 1. L'enonce demande de trouver un ecran qui donne autre
chose, ou de changer le reglage d'echelle du systeme. J'ai essaye les
deux. Aucun des deux n'a fonctionne.

Comment le facteur est calcule

Dans le backend Linux (XLib), la fonction GetDpiScale fait ceci
(NkXLibWindow.cpp, ligne 743) :

    float NkWindow::GetDpiScale() const {
        NkDisplayInfo cur = GetCurrentMonitor();
        float scale = cur.dpiScale;
        return (scale > 0.f) ? scale : 1.0f;
    }

Et le dpiScale du moniteur est calcule dans EnumerateMonitors
(NkXLibWindow.cpp, ligne 607) :

    float32 dpiX = 96.f, dpiY = 96.f;
    if (oi->mm_width > 0)
        dpiX = (float32)info.width * 25.4f / (float32)oi->mm_width;
    if (oi->mm_height > 0)
        dpiY = (float32)info.height * 25.4f / (float32)oi->mm_height;
    info.dpiX = dpiX;
    info.dpiY = dpiY;
    info.dpiScale = dpiX / 96.f;

Le calcul est : dpiX = largeur en pixels x 25.4 / largeur en
millimetres. Si le serveur X ne rapporte pas la taille physique en
millimetres, le code saute la branche et garde dpiX = 96, ce qui donne
dpiScale = 1.0.

Ce que WSLg rapporte

Commande :

    xrandr

Sortie brute (extrait) :

    XWAYLAND0 connected 1920x1080+0+0 (normal left inverted right x axis y axis) 0mm x 0mm

La taille physique est 0mm x 0mm. Le code ne peut donc pas calculer le
DPI a partir de cette valeur. Il garde 96 DPI et le facteur vaut 1.0.

Ce que j'ai essaye

Premier essai : forcer la taille physique avec xrandr.

    xrandr --output XWAYLAND0 --fbmm 254x143

Resultat : la commande ne produit aucun effet. La taille physique reste
a 0mm x 0mm apres la commande.

Deuxieme essai : lancer un serveur X virtuel avec un DPI de 192.

    sudo apt install -y xvfb
    Xvfb :98 -screen 0 1920x1080x24 -dpi 192 -listen tcp -nolisten unix

Verification du DPI rapporte par le serveur :

    DISPLAY=localhost:98 xdpyinfo

Sortie brute (extrait) :

    dimensions:    1920x1080 pixels (254x143 millimeters)
    resolution:    192x192 dots per inch

Le serveur X virtuel rapporte bien 192 DPI et une taille physique de
254x143 mm. Mais quand on interroge XRandR sur le meme serveur :

    DISPLAY=localhost:98 xrandr

Sortie brute :

    screen connected 1920x1080+0+0 0mm x 0mm

XRandR rapporte toujours 0mm x 0mm. Le code de Nkentseu utilise XRandR
pour lire la taille physique, pas xdpyinfo. Donc il ne voit pas le DPI
de 192 et reste a 1.0.

Le programme lance sur ce serveur virtuel :

    DISPLAY=localhost:98 timeout 5 Build/Bin/Debug-Linux/TestEchelle/TestEchelle

Sortie brute :

    Taille fenetre     : 1280 x 720
    Taille cible rendu : 1280 x 720
    Facteur d'echelle  : 1.0000

Le resultat est identique. Le facteur reste a 1.0.

Ce que ces essais montrent

Le facteur d'echelle depend d'une seule source : la taille physique de
l'ecran en millimetres, telle que rapportee par XRandR. Dans WSLg et
dans Xvfb, cette taille est rapportee comme 0mm x 0mm. Le code de
Nkentseu ne peut donc pas calculer un facteur different de 1.0.

Ce n'est pas un bug du programme. C'est une limite de l'environnement
d'execution. Le programme fait ce que le chapitre demande : il interroge
GetDpiScale et affiche la valeur. Mais cette valeur vaut 1.0 parce que
les serveurs X disponibles sous WSL ne rapportent pas la taille physique.

Ce qu'il faudrait pour voir autre chose

Trois pistes possibles, non testees ici :

1. Un ecran physique connecte a une machine Linux native. La, XRandR
   rapporterait une taille en millimetres differente de 0.
2. Un serveur X avec un pilote qui rapporte la taille physique, par
   exemple avec Xorg et une configuration de moniteur explicite.
3. Un environnement de bureau comme GNOME ou KDE, ou le reglage
   d'echelle est applique a plusieurs niveaux et ou le facteur peut
   etre change dans les preferences systeme.

Ces trois pistes demandent une machine ou un environnement que je n'ai
pas sous WSL.

Ce que j'ai retenu

Le facteur d'echelle n'est pas un reglage du programme. C'est une valeur
lue depuis le systeme, calculee a partir de la taille physique de
l'ecran. Si le systeme ne rapporte pas cette taille, le facteur reste a
sa valeur par defaut. C'est exactement ce qui se passe sous WSLg et
sous Xvfb.

Mesure faite le 27/09/2026.
Version de Jenga : 2.8.0.
