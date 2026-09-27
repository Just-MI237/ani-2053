Exercice 8 - Le presse-papiers, dans les deux sens

J'ai ecrit un programme qui lit le presse-papiers texte, le met en
majuscules, le remet. Puis qui lit une image, inverse ses couleurs et
la remet. Tout passe par NkWindow, comme demande.

Le programme est depose a cote sous le nom c3-exo8_main.cpp. Il fait
96 lignes.

Datation de la mesure

Commande :

    git log -1 --format="%h %ad" --date=short

Sortie brute :

    9c3fad3 2026-09-13

Version de Jenga : 2.8.0.

Ce que j'ai mis dans le presse-papiers avant de lancer

Dans Notepad sous Windows, j'ai tape le texte suivant et je l'ai copie
avec Ctrl+C :

    test externe

J'ai aussi copie une image dans le presse-papiers Windows (une capture
d'ecran avec Windows+Maj+S, puis Ctrl+C). Je voulais verifier si le
programme voyait cette image externe.

Resultat du test

Sortie brute du programme :

    === Phase texte ===
    Presse-papiers texte au demarrage : "" (taille 0)
    Apres SetClipboardText(bonjour le monde) : "bonjour le monde"
    Apres mise en majuscules : "BONJOUR LE MONDE"
    === Phase image ===
    Image disponible au demarrage : 0
    SetClipboardImage 4x4 RGBA=(200,100,50,255) : 1
    GetClipboardImage : 1, dims=4x4, octets=64, bpp=32
    Premier pixel lu : RGBA=(200,100,50,255)
    SetClipboardImage inverse : 1
    Lecture finale : 1, dims=4x4
    Premier pixel final : RGBA=(55,155,205,255)

Phase texte

Au demarrage, le programme lit le presse-papiers texte et trouve une
chaine vide, de taille zero. Le texte `test externe` que j'avais copie
sous Windows n'est pas vu. Le presse-papiers de NkWindow ne recoit pas
ce que Windows met dans le sien.

Ensuite, le programme ecrit son propre texte, `bonjour le monde`, et le
relit correctement. Puis il met les lettres en majuscules et relit
`BONJOUR LE MONDE`. Le cycle ecriture-lecture-reecriture-lecture
fonctionne a l'interieur du programme.

Phase image

Au demarrage, le programme demande s'il y a une image disponible et
recoit 0. J'avais pourtant copie une image dans le presse-papiers
Windows juste avant. Le programme ne la voit pas, comme il ne voyait
pas le texte externe.

Le programme cree ensuite une image 4x4 en RGBA8. Chaque pixel est
RGBA=(200,100,50,255). Il la depose dans le presse-papiers, la relit,
et retrouve exactement ce qu'il a mis : dims=4x4, octets=64 (4 pixels
de large fois 4 de haut fois 4 octets par pixel), bpp=32 (4 canaux de
8 bits). Le premier pixel lu est (200,100,50,255), identique a celui
ecrit.

Puis il inverse les trois composantes RGB sans toucher l'alpha. Le
premier pixel devient (55,155,205,255). Verification : 255 - 200 = 55,
255 - 100 = 155, 255 - 50 = 205. L'alpha reste a 255.

Ce que j'ai trouve dans le presse-papiers apres

Dans le sens texte

Avant le programme : rien (chaine vide de taille zero).
Apres SetClipboardText : la chaine exacte qui a ete ecrite.
Apres mise en majuscules : la chaine exacte en majuscules.

Dans le sens image

Avant le programme : aucune image.
Apres SetClipboardImage : une image 4x4 RGBA8, 64 octets, 32 bits par
pixel.
Apres inversion : la meme image, avec les composantes de couleur
inversees et l'alpha conserve.

Ce que cela dit de la plateforme

Sous WSLg/X11, le presse-papiers de NkWindow n'est pas le presse-papiers
du systeme. C'est un tampon interne a l'application. Les commentaires
de NkWindow.h le disent deja a la ligne 154 :

    Presse-papiers texte (UTF-8). Win32 = vrai presse-papiers OS ; autres
    plateformes = fallback interne a l'application (copier/coller intra-app).

Et a la ligne 230 pour l'image :

    Win32 = vrai presse-papiers OS (CF_DIBV5/CF_DIB, alpha preserve
    quand la source le fournit) ; autres plateformes = fallback interne
    a l'application (copier/coller intra-app) en attendant les
    implementations OS (X11 CLIPBOARD image/png, NSPasteboard,
    wl_data_device — notees dans la ROADMAP).

Le programme fait tout ce que l'enonce demande. Il lit, transforme,
reecrit. Dans les deux sens. Mais il ne parle qu'a lui-meme, pas au
reste du systeme.

Sur Windows, le meme programme verrait le texte externe et pourrait
deposer une image vraiment disponible pour les autres applications. Le
code ne change pas. Seule l'implementation de la plateforme change.

Tableau recapitulatif

| Etape | Valeur avant | Valeur apres |
|-------|--------------|--------------|
| Texte au demarrage | chaine vide | chaine vide |
| Texte ecrit puis relu | bonjour le monde | bonjour le monde |
| Texte en majuscules | bonjour le monde | BONJOUR LE MONDE |
| Image au demarrage | aucune | aucune |
| Image 4x4 ecrite puis relue | absente | 4x4, 64 octets, 32 bpp |
| Premier pixel avant inversion | (200,100,50,255) | (200,100,50,255) |
| Premier pixel apres inversion | (200,100,50,255) | (55,155,205,255) |

L'alpha est passe de 255 a 255, inchange. Les trois autres canaux sont
inverses.

Ce que l'exercice m'apprend

Le presse-papiers fait partie de ces fonctions dont l'API est la meme
partout, mais dont l'implementation varie radicalement. Un programme
qui copie du texte dans NkWindow::SetClipboardText et le relit dans
NkWindow::GetClipboardText fonctionne sur toutes les plateformes. Mais
ce que voit l'utilisateur n'est pas le meme : sur Windows, il collera
dans un autre logiciel ; sur Linux, il ne pourra coller que dans ce
programme.

C'est un cas ou la portabilite est dans l'interface, pas dans le
comportement.

Mesure faite le 27/09/2026.
Version de Jenga : 2.8.0.
