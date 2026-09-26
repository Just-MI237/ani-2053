Exercice 7 - Le temps que ca prend

J'ai chronometre deux constructions du projet NKMath, l'une apres
nettoyage du dossier Build, l'autre juste apres sans rien modifier.
NKMath est un projet qui se termine par BUILD COMPLETED, contrairement
a NkRef qui s'arrete sur un bug de NKCanvas.

Datation de la mesure

Commande :

```
git log -1 --format="%h %ad" --date=short
```

Sortie brute :

```
9c3fad3 2026-09-13
```

Mesure a froid

Commande :

```
rm -rf Build/
time jenga build --project NKMath --config Debug
```

Sortie brute complete :

```

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

Build Order (5 projects):
  1. NKPlatform [STATIC_LIB] → 
  2. NKCore [STATIC_LIB] (depends: NKPlatform) → 
  3. NKMemory [STATIC_LIB] (depends: NKCore, NKPlatform) → 
  4. NKContainers [STATIC_LIB] (depends: NKCore, NKMemory, NKPlatform) → 
  5. NKMath [STATIC_LIB] (depends: NKContainers, NKCore, NKMemory, NKPlatform)


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
│  ✓ Build Successful                                                             Time: 0.46s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKCore                                                           Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 6 source file(s)
✓   [1/6] Compiled: NkLimits.cpp
✓   [2/6] Compiled: NkBits.cpp
✓   [3/6] Compiled: NkAssert.cpp
✓   [4/6] Compiled: NkTraits.cpp
✓   [5/6] Compiled: NkPlatform.cpp
✓   [6/6] Compiled: NkSnprintf.cpp
ℹ Linking...
✓ Built: Build/Lib/Debug-Linux/NKCore.a

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.42s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKMemory                                                         Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 14 source file(s)
✓   [1/14] Compiled: NkGlobalOperators.cpp
✓   [2/14] Compiled: NkFunction.cpp
✓   [3/14] Compiled: NkContainerAllocator.cpp
✓   [4/14] Compiled: NkHash.cpp
✓   [5/14] Compiled: NkGc.cpp
✓   [6/14] Compiled: NkFunctionSIMD.cpp
✓   [7/14] Compiled: NkAllocator.cpp
✓   [8/14] Compiled: NkProfiler.cpp
✓   [9/14] Compiled: NkUtils.cpp
✓   [10/14] Compiled: NkMemory.cpp
✓   [11/14] Compiled: NkTracker.cpp
✓   [12/14] Compiled: NkMultiLevelAllocator.cpp
✓   [13/14] Compiled: NkPoolAllocator.cpp
✓   [14/14] Compiled: NkTag.cpp
ℹ Linking...
✓ Built: Build/Lib/Debug-Linux/NKMemory.a

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.99s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKContainers                                                     Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 43 source file(s)
✓   [1/43] Compiled: NkBTree.cpp
✓   [2/43] Compiled: NkBinaryTree.cpp
✓   [3/43] Compiled: NkHashMap.cpp
✓   [4/43] Compiled: NkMap.cpp
✓   [5/43] Compiled: NkSet.cpp
✓   [6/43] Compiled: NkPriorityQueue.cpp
✓   [7/43] Compiled: NkTrie.cpp
✓   [8/43] Compiled: NkFunction.cpp
✓   [9/43] Compiled: NkArray.cpp
✓   [10/43] Compiled: NkPool.cpp
✓   [11/43] Compiled: NkUnorderedMap.cpp
✓   [12/43] Compiled: NkBind.cpp
✓   [13/43] Compiled: NkRingBuffer.cpp
✓   [14/43] Compiled: NkUnorderedSet.cpp
✓   [15/43] Compiled: NkTuple.cpp
✓   [16/43] Compiled: NkPair.cpp
✓   [17/43] Compiled: NkFunctional.cpp
✓   [18/43] Compiled: NkInitializerList.cpp
✓   [19/43] Compiled: NkIterator.cpp
✓   [20/43] Compiled: NkContainers.cpp
✓   [21/43] Compiled: NkDeque.cpp
✓   [22/43] Compiled: NkQuadTree.cpp
✓   [23/43] Compiled: NkVector.cpp
✓   [24/43] Compiled: NkGraph.cpp
✓   [25/43] Compiled: NkList.cpp
✓   [26/43] Compiled: NkASCII.cpp
✓   [27/43] Compiled: NkDoubleList.cpp
✓   [28/43] Compiled: NkUTF16.cpp
✓   [29/43] Compiled: NkEncoding.cpp
✓   [30/43] Compiled: NkUTF32.cpp
✓   [31/43] Compiled: NkUTF8.cpp
✓   [32/43] Compiled: NkBase64.cpp
✓   [33/43] Compiled: NkBasicString.cpp
✓   [34/43] Compiled: NkFormat.cpp
✓   [35/43] Compiled: NkOptional.cpp
✓   [36/43] Compiled: NkResult.cpp
✓   [37/43] Compiled: NkStringView.cpp
✓   [38/43] Compiled: NkString.cpp
✓   [39/43] Compiled: NkStringBuilder.cpp
✓   [40/43] Compiled: NkStringHash.cpp
✓   [41/43] Compiled: NkVariant.cpp
✓   [42/43] Compiled: NkStringUtils.cpp
✓   [43/43] Compiled: NkSpan.cpp
ℹ Linking...
✓ Built: Build/Lib/Debug-Linux/NKContainers.a

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 3.49s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKMath                                                           Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 12 source file(s)
✓   [1/12] Compiled: NkAngle.cpp
✓   [2/12] Compiled: NkFunctions.cpp
✓   [3/12] Compiled: NkEulerAngle.cpp
✓   [4/12] Compiled: NkQuat.cpp
✓   [5/12] Compiled: NkRandom.cpp
✓   [6/12] Compiled: NkMat.cpp
✓   [7/12] Compiled: NkSIMD.cpp
✓   [8/12] Compiled: NkRange.cpp
✓   [9/12] Compiled: NkColor.cpp
✓   [10/12] Compiled: NkRectangle.cpp
✓   [11/12] Compiled: NkVec.cpp
✓   [12/12] Compiled: NkSegment.cpp
ℹ Linking...
✓ Built: Build/Lib/Debug-Linux/NKMath.a

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 3.74s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED                                 
════════════════════════════════════════════════════════════════════════════════
Projects Built:  5/5
Time:           9.10s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════

```

Sortie de time :

```
real    0m17.006s
user    0m31.015s
sys     0m8.177s
```

Mesure a chaud

Commande :

```
time jenga build --project NKMath --config Debug
```

Sortie brute complete :

```

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

Build Order (5 projects):
  1. NKPlatform [STATIC_LIB] → 
  2. NKCore [STATIC_LIB] (depends: NKPlatform) → 
  3. NKMemory [STATIC_LIB] (depends: NKCore, NKPlatform) → 
  4. NKContainers [STATIC_LIB] (depends: NKCore, NKMemory, NKPlatform) → 
  5. NKMath [STATIC_LIB] (depends: NKContainers, NKCore, NKMemory, NKPlatform)


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKPlatform                                                       Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 7 source file(s)
✓ All files up to date

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.01s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKCore                                                           Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 6 source file(s)
✓ All files up to date

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.02s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKMemory                                                         Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 14 source file(s)
✓ All files up to date

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.03s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKContainers                                                     Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 43 source file(s)
✓ All files up to date

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.14s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKMath                                                           Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 12 source file(s)
✓ All files up to date

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.07s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED                                 
════════════════════════════════════════════════════════════════════════════════
Projects Built:  5/5
Time:           0.27s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════

```

Sortie de time :

```
real    0m6.241s
user    0m1.730s
sys     0m0.531s
```

Les deux nombres

- A froid : 17.006s
- A chaud : 6.241s

Explication de l'ecart

A froid, Jenga compile les 5 projets dans l'ordre : NKPlatform,
NKCore, NKMemory, NKContainers, NKMath. Chaque fichier source est
compile, chaque bibliotheque est liee. C'est ca qui prend 17 secondes.

A chaud, Jenga parcourt le workspace, lit tous les fichiers .jenga,
reconstruit le graphe de dependances, et compare les dates des
sources a celles des objets deja produits. Pour les 5 projets, il
affiche All files up to date. Il ne recompile rien, il ne relie
rien. Les 6 secondes restantes correspondent a ce travail de
verification, plus le demarrage de Jenga lui-meme.

Ce que cela dit sur le confort de travail

Un depot de plusieurs centaines de projets comme Nkentseu serait
inutilisable si chaque compilation recompilait tout. La comparaison
des dates permet de ne recompiler que ce qui a change depuis la
derniere construction. C'est ce qui rend le travail quotidien
possible : apres avoir modifie un seul fichier, la reconstruction
ne prend que quelques secondes.


Comment les sorties ont ete capturees

Pour separer la sortie de Jenga de celle de time, j'ai utilise une
redirection vers un fichier :

    time jenga build --project NKMath --config Debug > /tmp/exo7_froid.txt 2>&1
    time jenga build --project NKMath --config Debug > /tmp/exo7_chaud.txt 2>&1

La commande time ecrit son resultat (real, user, sys) sur la sortie
d'erreur du shell, pas dans le fichier. J'ai donc note ces trois lignes
a part, et j'ai colle le contenu des deux fichiers pour les sorties
brutes de Jenga.
Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
