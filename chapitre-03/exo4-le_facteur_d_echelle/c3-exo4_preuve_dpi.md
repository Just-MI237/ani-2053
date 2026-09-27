Preuve brute - proposition DPI XRandR / Xlib

Mesure faite le 27/09/2026.
Version de Jenga : 2.8.0.
Commit Nkentseu : 9c3fad33.

Cas 1 - Xvfb :98 configure a 192 dpi

xdpyinfo sur Xvfb :98 :

```
  dimensions:    1920x1080 pixels (254x143 millimeters)
  resolution:    192x192 dots per inch
```

xrandr sur Xvfb :98 :

```
Screen 0: minimum 1 x 1, current 1920 x 1080, maximum 1920 x 1080
screen connected 1920x1080+0+0 0mm x 0mm
   1920x1080      0.00*
```

TestDpi sur Xvfb :98, avec le patch :

```
[DBG-dpi] oi mm=0x0, screen=0, xlib=254x143
dpiScale=2.000000
```

XRandR renvoie 0mm. Xlib renvoie 254x143. Le patch tombe sur Xlib et donne dpiScale = 2.0.

Cas 2 - WSLg (:0), qui declare 96 dpi

xdpyinfo sur WSLg :

```
  dimensions:    1920x1080 pixels (508x285 millimeters)
  resolution:    96x96 dots per inch
```

TestDpi sur WSLg, avec le patch :

```
[DBG-dpi] oi mm=0x0, screen=0, xlib=508x285
dpiScale=1.000000
```

XRandR renvoie 0mm. Xlib renvoie 508x285. Le patch tombe sur Xlib, mais 508mm correspond a 96 dpi pour 1920 pixels. Le resultat reste coherent avec ce que WSLg declare.

Conclusion

Le correctif rend service quand XRandR se tait et que le serveur X est configure pour autre chose que 96 dpi. Sur WSLg, il ne change rien parce que WSLg declare lui-meme 96 dpi. Ce n'est pas un bug de WSLg, c'est sa configuration.
