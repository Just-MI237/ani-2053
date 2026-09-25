Exercice 3 - Le module seul

J'ai lance la commande :

    jenga build --project NKMath --config Debug

Sortie brute complete, depuis la banniere Jenga jusqu'au bilan final :

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
✓   [2/7] Compiled: NkCompilerDetect.cpp
✓   [3/7] Compiled: NkCGXDetect.cpp
✓   [4/7] Compiled: NkEndianness.cpp
✓   [5/7] Compiled: NkEnv.cpp
✓   [6/7] Compiled: NkPlatformConfig.cpp
✓   [7/7] Compiled: NkCPUFeatures.cpp
ℹ Linking...
✓ Built: Build/Lib/Debug-Linux/NKPlatform.a

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.26s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKCore                                                           Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 6 source file(s)
✓   [1/6] Compiled: NkLimits.cpp
✓   [2/6] Compiled: NkBits.cpp
✓   [3/6] Compiled: NkAssert.cpp
✓   [4/6] Compiled: NkTraits.cpp
✓   [5/6] Compiled: NkSnprintf.cpp
✓   [6/6] Compiled: NkPlatform.cpp
ℹ Linking...
✓ Built: Build/Lib/Debug-Linux/NKCore.a

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.22s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKMemory                                                         Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 14 source file(s)
✓   [1/14] Compiled: NkGlobalOperators.cpp
✓   [2/14] Compiled: NkFunction.cpp
✓   [3/14] Compiled: NkContainerAllocator.cpp
✓   [4/14] Compiled: NkGc.cpp
✓   [5/14] Compiled: NkFunctionSIMD.cpp
✓   [6/14] Compiled: NkAllocator.cpp
✓   [7/14] Compiled: NkHash.cpp
✓   [8/14] Compiled: NkProfiler.cpp
✓   [9/14] Compiled: NkMemory.cpp
✓   [10/14] Compiled: NkUtils.cpp
✓   [11/14] Compiled: NkMultiLevelAllocator.cpp
✓   [12/14] Compiled: NkTracker.cpp
✓   [13/14] Compiled: NkPoolAllocator.cpp
✓   [14/14] Compiled: NkTag.cpp
ℹ Linking...
✓ Built: Build/Lib/Debug-Linux/NKMemory.a

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.57s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKContainers                                                     Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 43 source file(s)
✓   [1/43] Compiled: NkMap.cpp
✓   [2/43] Compiled: NkSet.cpp
✓   [3/43] Compiled: NkBTree.cpp
✓   [4/43] Compiled: NkTrie.cpp
✓   [5/43] Compiled: NkHashMap.cpp
✓   [6/43] Compiled: NkBinaryTree.cpp
✓   [7/43] Compiled: NkPriorityQueue.cpp
✓   [8/43] Compiled: NkArray.cpp
✓   [9/43] Compiled: NkUnorderedSet.cpp
✓   [10/43] Compiled: NkUnorderedMap.cpp
✓   [11/43] Compiled: NkFunction.cpp
✓   [12/43] Compiled: NkPool.cpp
✓   [13/43] Compiled: NkBind.cpp
✓   [14/43] Compiled: NkRingBuffer.cpp
✓   [15/43] Compiled: NkTuple.cpp
✓   [16/43] Compiled: NkPair.cpp
✓   [17/43] Compiled: NkInitializerList.cpp
✓   [18/43] Compiled: NkFunctional.cpp
✓   [19/43] Compiled: NkIterator.cpp
✓   [20/43] Compiled: NkContainers.cpp
✓   [21/43] Compiled: NkDeque.cpp
✓   [22/43] Compiled: NkVector.cpp
✓   [23/43] Compiled: NkASCII.cpp
✓   [24/43] Compiled: NkGraph.cpp
✓   [25/43] Compiled: NkList.cpp
✓   [26/43] Compiled: NkDoubleList.cpp
✓   [27/43] Compiled: NkQuadTree.cpp
✓   [28/43] Compiled: NkBase64.cpp
✓   [29/43] Compiled: NkEncoding.cpp
✓   [30/43] Compiled: NkUTF32.cpp
✓   [31/43] Compiled: NkUTF8.cpp
✓   [32/43] Compiled: NkUTF16.cpp
✓   [33/43] Compiled: NkBasicString.cpp
✓   [34/43] Compiled: NkFormat.cpp
✓   [35/43] Compiled: NkOptional.cpp
✓   [36/43] Compiled: NkResult.cpp
✓   [37/43] Compiled: NkStringView.cpp
✓   [38/43] Compiled: NkString.cpp
✓   [39/43] Compiled: NkStringBuilder.cpp
✓   [40/43] Compiled: NkVariant.cpp
✓   [41/43] Compiled: NkSpan.cpp
✓   [42/43] Compiled: NkStringHash.cpp
✓   [43/43] Compiled: NkStringUtils.cpp
ℹ Linking...
✓ Built: Build/Lib/Debug-Linux/NKContainers.a

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 2.13s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKMath                                                           Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 12 source file(s)
✓   [1/12] Compiled: NkFunctions.cpp
✓   [2/12] Compiled: NkAngle.cpp
✓   [3/12] Compiled: NkEulerAngle.cpp
✓   [4/12] Compiled: NkMat.cpp
✓   [5/12] Compiled: NkQuat.cpp
✓   [6/12] Compiled: NkRandom.cpp
✓   [7/12] Compiled: NkSIMD.cpp
✓   [8/12] Compiled: NkColor.cpp
✓   [9/12] Compiled: NkRange.cpp
✓   [10/12] Compiled: NkRectangle.cpp
✓   [11/12] Compiled: NkVec.cpp
✓   [12/12] Compiled: NkSegment.cpp
ℹ Linking...
✓ Built: Build/Lib/Debug-Linux/NKMath.a

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 2.17s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED                                 
════════════════════════════════════════════════════════════════════════════════
Projects Built:  5/5
Time:           5.35s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════

```

Arbre de construction

L'enonce demande que ce qui vient en premier soit en bas et que NKMath
soit en haut. C'est le sens des fondations : ce qui ne depend de rien
est le socle, tout le reste repose dessus.

- NKMath (depend de NKContainers, NKCore, NKMemory et NKPlatform)
  - NKContainers (depend de NKCore, NKMemory et NKPlatform)
    - NKMemory (depend de NKCore et NKPlatform)
      - NKCore (depend de NKPlatform)
        - NKPlatform (ne depend de rien, construit en premier)

Fleches de dependance

Pour trois modules, voici ce dont chacun depend directement :

NKMath
  --> NKContainers
  --> NKCore
  --> NKMemory
  --> NKPlatform

NKMemory
  --> NKCore
  --> NKPlatform

NKCore
  --> NKPlatform

NKPlatform
  (ne depend de rien)

Ce que ces fleches montrent

La hierarchie des dependances ne suit pas la hierarchie des dossiers.
NKMath depend directement de quatre modules differents, pas seulement
de celui qui est juste au-dessus de lui dans l'arborescence. C'est
cette hierarchie-la, celle des dependances, qui decide de l'ordre de
construction. Jenga la calcule a partir des fichiers .jenga.

Datation de la mesure

Commande :

```
git log -1 --format="%h %ad" --date=short
```

Sortie brute :

```
9c3fad3 2026-09-13
```

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
