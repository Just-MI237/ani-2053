Exercice 10 - Le graphe d'un module

J'ai pris NKCanvas. J'ai ouvert son .jenga et j'ai releve ses
dependances directes, puis celles de ses dependances, en m'arretant
a deux niveaux.

Datation de la mesure

Commande :

```
git log -1 --format="%h %ad" --date=short
```

Sortie brute :

```
9c3fad3 2026-09-13
```

Version de Jenga : 2.8.0.

Dependances directes de NKCanvas

Commande :

```
sed -n "61,64p" Kernel/Runtime/NKCanvas/NKCanvas.jenga
```

Sortie brute :

```
    _canvasDeps = ["NKWindow", "NKFont", "NKImage", "NKGui", "NKEvent", "NKGlad",
                   "NKStream", "NKTime", "NKThreading",
                   "NKFileSystem", "NKLogger", "NKMath", "NKContainers", "NKMemory",
```

NKCanvas depend directement de 16 projets :
- NKWindow
- NKFont
- NKImage
- NKGui
- NKEvent
- NKGlad
- NKStream
- NKTime
- NKThreading
- NKFileSystem
- NKLogger
- NKMath
- NKContainers
- NKMemory
- NKCore
- NKPlatform

Dependances de ces dependances (niveau 2)

Pour chaque projet du niveau 1, voici ses propres dependances, telles
qu'elles apparaissent dans le Build Order de Jenga.

NKWindow depend de : NKContainers, NKCore, NKEvent, NKFileSystem,
  NKLogger, NKMath, NKMemory, NKPlatform, NKThreading, NKTime
NKFont depend de : NKContainers, NKCore, NKLogger, NKMath, NKMemory,
  NKPlatform, NKThreading
NKImage depend de : NKContainers, NKCore, NKFileSystem, NKLogger,
  NKMath, NKMemory, NKPlatform, NKStream, NKThreading
NKGui depend de : NKContainers, NKCore, NKEvent, NKFileSystem, NKFont,
  NKImage, NKLogger, NKMath, NKMemory, NKPlatform, NKThreading
NKEvent depend de : NKContainers, NKCore, NKLogger, NKMath, NKMemory,
  NKPlatform, NKThreading, NKTime
NKGlad ne depend de rien.
NKStream depend de : NKContainers, NKCore, NKFileSystem, NKLogger,
  NKMemory, NKPlatform, NKThreading
NKTime depend de : NKContainers, NKCore, NKLogger, NKMemory, NKPlatform,
  NKThreading
NKThreading depend de : NKContainers, NKCore, NKMemory, NKPlatform
NKFileSystem depend de : NKContainers, NKCore, NKLogger, NKMemory,
  NKPlatform, NKThreading
NKLogger depend de : NKContainers, NKCore, NKMemory, NKPlatform,
  NKThreading
NKMath depend de : NKContainers, NKCore, NKMemory, NKPlatform
NKContainers depend de : NKCore, NKMemory, NKPlatform
NKMemory depend de : NKCore, NKPlatform
NKCore depend de : NKPlatform
NKPlatform ne depend de rien.

Graphe

Du plus bas (construit en premier) au plus haut (NKCanvas) :

Niveau 0 : NKPlatform, NKGlad (ne dependent de rien)
Niveau 1 : NKCore (depend de NKPlatform)
Niveau 2 : NKMemory (depend de NKCore, NKPlatform)
Niveau 3 : NKContainers (depend de NKCore, NKMemory, NKPlatform)
Niveau 4 : NKMath, NKThreading (dependent de NKContainers, NKCore,
  NKMemory, NKPlatform)
Niveau 5 : NKLogger (depend de NKContainers, NKCore, NKMemory,
  NKPlatform, NKThreading)
Niveau 6 : NKFont, NKTime, NKFileSystem (dependent de NKContainers,
  NKCore, NKLogger, NKMemory, NKPlatform, NKThreading)
Niveau 7 : NKEvent (depend de NKContainers, NKCore, NKLogger, NKMath,
  NKMemory, NKPlatform, NKThreading, NKTime)
Niveau 8 : NKStream (depend de NKContainers, NKCore, NKFileSystem,
  NKLogger, NKMemory, NKPlatform, NKThreading)
Niveau 9 : NKWindow (depend de NKContainers, NKCore, NKEvent,
  NKFileSystem, NKLogger, NKMath, NKMemory, NKPlatform, NKThreading,
  NKTime)
Niveau 10 : NKImage (depend de NKContainers, NKCore, NKFileSystem,
  NKLogger, NKMath, NKMemory, NKPlatform, NKStream, NKThreading)
Niveau 11 : NKGui (depend de NKContainers, NKCore, NKEvent, NKFileSystem,
  NKFont, NKImage, NKLogger, NKMath, NKMemory, NKPlatform, NKThreading)
Niveau 12 : NKCanvas (depend de tous les 16 projets ci-dessus)

Combien de projets avant NKCanvas

Commande :

```
jenga build --project NKCanvas --config Debug
```

Extrait du Build Order :

```
Build Order (17 projects):
  1. NKPlatform [STATIC_LIB] → 
  2. NKGlad [STATIC_LIB] → 
  3. NKCore [STATIC_LIB] (depends: NKPlatform) → 
  4. NKMemory [STATIC_LIB] (depends: NKCore, NKPlatform) → 
  5. NKContainers [STATIC_LIB] (depends: NKCore, NKMemory, NKPlatform) → 
  6. NKMath [STATIC_LIB] (depends: NKContainers, NKCore, NKMemory, NKPlatform) → 
  7. NKThreading [STATIC_LIB] (depends: NKContainers, NKCore, NKMemory, NKPlatform) → 
  8. NKLogger [STATIC_LIB] (depends: NKContainers, NKCore, NKMemory, NKPlatform, NKThreading) → 
  9. NKFont [STATIC_LIB] (depends: NKContainers, NKCore, NKLogger, NKMath, NKMemory, NKPlatform, NKThreading) → 
  10. NKTime [STATIC_LIB] (depends: NKContainers, NKCore, NKLogger, NKMemory, NKPlatform, NKThreading) → 
  11. NKFileSystem [STATIC_LIB] (depends: NKContainers, NKCore, NKLogger, NKMemory, NKPlatform, NKThreading) → 
  12. NKEvent [STATIC_LIB] (depends: NKContainers, NKCore, NKLogger, NKMath, NKMemory, NKPlatform, NKThreading, NKTime) → 
  13. NKStream [STATIC_LIB] (depends: NKContainers, NKCore, NKFileSystem, NKLogger, NKMemory, NKPlatform, NKThreading) → 
  14. NKWindow [STATIC_LIB] (depends: NKContainers, NKCore, NKEvent, NKFileSystem, NKLogger, NKMath, NKMemory, NKPlatform, NKThreading, NKTime) → 
  15. NKImage [STATIC_LIB] (depends: NKContainers, NKCore, NKFileSystem, NKLogger, NKMath, NKMemory, NKPlatform, NKStream, NKThreading) → 
  16. NKGui [STATIC_LIB] (depends: NKContainers, NKCore, NKEvent, NKFileSystem, NKFont, NKImage, NKLogger, NKMath, NKMemory, NKPlatform, NKThreading) → 
  17. NKCanvas [STATIC_LIB] (depends: NKContainers, NKCore, NKEvent, NKFileSystem, NKFont, NKGlad, NKGui, NKImage, NKLogger, NKMath, NKMemory, NKPlatform, NKStream, NKThreading, NKTime, NKWindow)

```

Le Build Order compte 17 projets. NKCanvas est le 17e. Il faut donc
construire 16 projets avant lui.

Conclusion

NKCanvas est le projet le plus haut de son graphe. Il depend
directement de 16 projets, qui eux-memes dependent de 15 autres
projets, dont certains sont communs. Deux projets ne dependent de
rien : NKPlatform et NKGlad. Ce sont les deux socles.

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
