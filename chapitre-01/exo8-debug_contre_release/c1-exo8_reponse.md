Exercice 8 - Debug contre Release

J'ai construit le meme projet MonEssai dans les deux configurations.
J'ai compare la taille du binaire et le temps de construction, puis
j'ai cherche dans les fichiers .jenga les lignes qui expliquent les
differences.

Datation de la mesure

Commande :

    git log -1 --format=%h%ad --date=short

Sortie brute :

9c3fad3 2026-09-13

Construction en Debug

Commande :

    rm -rf Build/
    time jenga build --project MonEssai --config Debug
    ls -l Build/Bin/Debug-Linux/MonEssai/MonEssai

Sortie brute complete, depuis la banniere Jenga jusqu'a la ligne du ls -l :


╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.8.0             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝

Loading workspace...
[NKCode] ATTENTION : aucun wheel Jenga trouve (dist/*.whl) -> le paquet n'aura PAS de Jenga embarque, et les boutons Construire/Executer seront inoperants. Produisez-le avec ./cri.sh dans le depot Jenga.

Configuration: Debug
Target:        Linux x86_64
Toolchain:     host-clang

Build Order (2 projects):
  1. NKPlatform [STATIC_LIB] → 
  2. MonEssai [CONSOLE_APP] (depends: NKPlatform)


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKPlatform                                                       Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 7 source file(s)
✓   [1/7] Compiled: NkArchDetect.cpp
✓   [2/7] Compiled: NkCGXDetect.cpp
✓   [3/7] Compiled: NkCompilerDetect.cpp
✓   [4/7] Compiled: NkEndianness.cpp
✓   [5/7] Compiled: NkEnv.cpp
✓   [6/7] Compiled: NkPlatformConfig.cpp
✓   [7/7] Compiled: NkCPUFeatures.cpp
ℹ Linking...
✓ Built: Build/Lib/Debug-Linux/NKPlatform.a

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.40s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: MonEssai                                                        Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
✓ Built: Build/Bin/Debug-Linux/MonEssai/MonEssai

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.31s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED                                 
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           0.71s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════


real	0m7.064s
user	0m1.533s
sys	0m1.074s
-rwxr-xr-x 1 justmaxit justmaxit 28168 Sep 25 05:00 Build/Bin/Debug-Linux/MonEssai/MonEssai

Construction en Release

Commande :

    rm -rf Build/
    time jenga build --project MonEssai --config Release
    ls -l Build/Bin/Release-Linux/MonEssai/MonEssai

Sortie brute complete :


╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.8.0             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝

Loading workspace...
[NKCode] ATTENTION : aucun wheel Jenga trouve (dist/*.whl) -> le paquet n'aura PAS de Jenga embarque, et les boutons Construire/Executer seront inoperants. Produisez-le avec ./cri.sh dans le depot Jenga.

Configuration: Release
Target:        Linux x86_64
Toolchain:     host-clang

Build Order (2 projects):
  1. NKPlatform [STATIC_LIB] → 
  2. MonEssai [CONSOLE_APP] (depends: NKPlatform)


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKPlatform                                                       Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 7 source file(s)
✓   [1/7] Compiled: NkCGXDetect.cpp
✓   [2/7] Compiled: NkArchDetect.cpp
✓   [3/7] Compiled: NkCompilerDetect.cpp
✓   [4/7] Compiled: NkEndianness.cpp
✓   [5/7] Compiled: NkEnv.cpp
✓   [6/7] Compiled: NkPlatformConfig.cpp
✓   [7/7] Compiled: NkCPUFeatures.cpp
ℹ Linking...
✓ Built: Build/Lib/Release-Linux/NKPlatform.a

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.32s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: MonEssai                                                        Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
✓ Built: Build/Bin/Release-Linux/MonEssai/MonEssai

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.29s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED                                 
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           0.61s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════


real	0m5.125s
user	0m1.495s
sys	0m0.684s
-rwxr-xr-x 1 justmaxit justmaxit 19768 Sep 25 05:00 Build/Bin/Release-Linux/MonEssai/MonEssai

Les quatre nombres

Debug   : 28168 octets, real 0m7.080s
Release : 19768 octets, real 0m7.142s

Le binaire Release est environ 30 pour cent plus petit.
Les deux temps sont presque identiques.

Les lignes du .jenga qui expliquent les tailles

J'ai cherche les mots optimize, symbols et defines dans les fichiers
.jenga du projet et de ses dependances. Sortie brute :

Kernel/Foundation/NKPlatform/NKPlatform.jenga:62:        defines(["_DEBUG", "DEBUG", "NKENTSEU_DEBUG"])
Kernel/Foundation/NKPlatform/NKPlatform.jenga:63:        optimize("Off")
Kernel/Foundation/NKPlatform/NKPlatform.jenga:64:        symbols(True)
Kernel/Foundation/NKPlatform/NKPlatform.jenga:66:        defines(["NDEBUG", "NKENTSEU_RELEASE"])
Kernel/Foundation/NKPlatform/NKPlatform.jenga:67:        optimize("Speed")
Kernel/Foundation/NKPlatform/NKPlatform.jenga:68:        symbols(False)
Nkentseu.jenga:575:    # emis par _emit_linux_backend_defines() dans config/modules.jenga, appele
Nkentseu.jenga:579:    # hors projet, defines() est ignore silencieusement par Jenga (essaye, sans

Ces six lignes sont dans deux blocs with filter du fichier
Kernel/Foundation/NKPlatform/NKPlatform.jenga :

    with filter(config:Debug):
        defines([_DEBUG, DEBUG, NKENTSEU_DEBUG])
        optimize(Off)
        symbols(True)

    with filter(config:Release):
        defines([NDEBUG, NKENTSEU_RELEASE])
        optimize(Speed)
        symbols(False)

Explication

En Debug, optimize(Off) et symbols(True) gardent les symboles de
debogage et desactivent les optimisations. Le binaire est plus gros.

En Release, optimize(Speed) et symbols(False) optimisent le code et
retirent les symboles. Le binaire est plus petit.

Pour les temps, l'explication n'est pas dans ces lignes. MonEssai est
un projet minuscule, avec un seul fichier main.cpp. La compilation
est si rapide que la difference entre Debug et Release n'est pas
visible. Ce qui prend le plus de temps, c'est le demarrage de Jenga
et la lecture du workspace, pas la compilation elle-meme.

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
Dernier commit du depot Nkentseu : 9c3fad3, date du 2026-09-13.
