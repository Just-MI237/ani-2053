Bonjour,

J'ai suivi la piste de l'exercice 4. J'ai trouve la fonction Xlib qui donne
la taille physique que le moteur n'interroge pas.

Le probleme

Sous un serveur X ou XRandR ne renvoie pas la taille physique de l'ecran,
le moteur garde 96 dpi et un facteur d'echelle de 1.0. C'est le cas de
Xvfb :98 que j'ai lance avec -dpi 192 : xrandr dit 0mm x 0mm, mais
xdpyinfo dit 254x143 mm et 192 dpi. Le moteur ne voit que la premiere
reponse.

La cause

Dans NkXLibWindow.cpp, la fonction XLibFillDisplayInfoFromCrtc interroge
XRandR via oi->mm_width et oi->mm_height. Quand ces valeurs valent 0,
elle garde 96 dpi. La fonction de secours XLibFallbackDisplayInfo, elle,
interroge deja Xlib via DisplayWidthMM et DisplayHeightMM. La fonction
principale ne le fait pas.

Le correctif

J'ai ajoute un parametre int screen a XLibFillDisplayInfoFromCrtc, passe
DefaultScreen(display) depuis l'appelant, et ajoute une chute sur
DisplayWidthMM et DisplayHeightMM quand XRandR renvoie 0. La fonction
principale fait maintenant ce que la fonction de secours faisait deja.

Les mesures avant et apres, sur Xvfb :98 a 192 dpi

xdpyinfo :

    dimensions:    1920x1080 pixels (254x143 millimeters)
    resolution:    192x192 dots per inch

xrandr :

    screen connected 1920x1080+0+0 0mm x 0mm

Avant le patch, dpiScale valait 1.000000.
Apres le patch, dpiScale vaut 2.000000.

Sur WSLg (:0), qui declare 96 dpi :

    dimensions:    1920x1080 pixels (508x285 millimeters)
    resolution:    96x96 dots per inch

Le patch tombe aussi sur Xlib (508x285), mais la valeur est coherente
avec 96 dpi, donc dpiScale reste 1.000000. Ce n'est pas un bug de WSLg,
c'est ce qu'il declare. Le correctif rend service quand XRandR se tait et
que le serveur X est configure pour autre chose que 96 dpi.

Le diff

Le diff est joint. Il modifie un seul fichier :
Kernel/Runtime/NKWindow/src/NKWindow/Platform/XLib/NkXLibWindow.cpp

Si vous voulez, je peux preparer une pull request sur le depot Nkentseu.

Bonne journee,
Yann Vivien


---- debut du diff ----

diff --git a/Kernel/Runtime/NKWindow/src/NKWindow/Platform/XLib/NkXLibWindow.cpp b/Kernel/Runtime/NKWindow/src/NKWindow/Platform/XLib/NkXLibWindow.cpp
index d5d6a061..fe4ee709 100644
--- a/Kernel/Runtime/NKWindow/src/NKWindow/Platform/XLib/NkXLibWindow.cpp
+++ b/Kernel/Runtime/NKWindow/src/NKWindow/Platform/XLib/NkXLibWindow.cpp
@@ -555,7 +555,7 @@ namespace nkentseu {
 	// Remplit un NkDisplayInfo depuis un CRTC XRandR actif. Calcule le DPI a
 	// partir de la taille physique (mm) de l'output relie, et le refresh depuis
 	// le mode courant. Retourne false si le CRTC est inactif (pas de mode).
-	static bool XLibFillDisplayInfoFromCrtc(Display *display, XRRScreenResources *res, RRCrtc crtc,
+	static bool XLibFillDisplayInfoFromCrtc(Display *display, XRRScreenResources *res, int screen, RRCrtc crtc,
 											RROutput primaryOutput, uint32 index, NkDisplayInfo &info) {
 		XRRCrtcInfo *ci = XRRGetCrtcInfo(display, res, crtc);
 		if (!ci)
@@ -604,10 +604,20 @@ namespace nkentseu {
 
 			// DPI calcule depuis la taille physique (mm). dpi = px * 25.4 / mm.
 			float32 dpiX = 96.f, dpiY = 96.f;
-			if (oi->mm_width > 0)
-				dpiX = (float32)info.width * 25.4f / (float32)oi->mm_width;
-			if (oi->mm_height > 0)
-				dpiY = (float32)info.height * 25.4f / (float32)oi->mm_height;
+			uint32 mmW = (uint32)oi->mm_width;
+                        uint32 mmH = (uint32)oi->mm_height;
+                        if (mmW == 0) {
+                                int fbW = DisplayWidthMM(display, screen);
+                                if (fbW > 0) mmW = (uint32)fbW;
+                        }
+                        if (mmH == 0) {
+                                int fbH = DisplayHeightMM(display, screen);
+                                if (fbH > 0) mmH = (uint32)fbH;
+                        }
+                        if (mmW > 0)
+                                dpiX = (float32)info.width * 25.4f / (float32)mmW;
+                        if (mmH > 0)
+                                dpiY = (float32)info.height * 25.4f / (float32)mmH;
 			info.dpiX = dpiX;
 			info.dpiY = dpiY;
 			info.dpiScale = dpiX / 96.f;
@@ -680,7 +690,7 @@ namespace nkentseu {
 		uint32 idx = 0;
 		for (int i = 0; i < res->ncrtc; ++i) {
 			NkDisplayInfo info;
-			if (XLibFillDisplayInfoFromCrtc(display, res, res->crtcs[i], primary, idx, info)) {
+			if (XLibFillDisplayInfoFromCrtc(display, res, DefaultScreen(display), res->crtcs[i], primary, idx, info)) {
 				out.PushBack(info);
 				++idx;
 			}

---- fin du diff ----
