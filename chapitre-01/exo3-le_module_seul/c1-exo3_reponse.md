Exercice 3 - Le module seul

J'ai lancé la commande :

jenga build --project NKMath --config Debug

Voici l'ordre de construction que Jenga a affiché avant de compiler :

Build Order (5 projects):
  1. NKPlatform
  2. NKCore (depends: NKPlatform)
  3. NKMemory (depends: NKCore, NKPlatform)
  4. NKContainers (depends: NKCore, NKMemory, NKPlatform)
  5. NKMath (depends: NKContainers, NKCore, NKMemory, NKPlatform)

La compilation a réussi : 5 projets sur 5, en 5,66 secondes.

Voici les dépendances de NKMath sous forme hiérarchique, du plus bas au plus haut :

- NKPlatform (ne dépend de rien)
  - NKCore (dépend de NKPlatform)
    - NKMemory (dépend de NKCore et NKPlatform)
      - NKContainers (dépend de NKCore, NKMemory et NKPlatform)
        - NKMath (dépend de NKContainers, NKCore, NKMemory et NKPlatform)

Une autre façon de voir la même chose, en partant de NKMath et en descendant :

- NKMath dépend de :
  - NKContainers
  - NKCore
  - NKMemory
  - NKPlatform

Ce que ça montre :

NKMath ne dépend pas d'une seule chaîne. Il dépend directement de quatre
modules différents. C'est pour cette raison que la figure n'est pas une
simple colonne : plusieurs branches partent de NKMath et convergent toutes
vers NKPlatform, qui est le socle commun.

NKPlatform est construit en premier parce qu'il ne dépend de rien. NKMath
est construit en dernier parce qu'il dépend de tout le reste. C'est Jenga
qui calcule cet ordre à partir des dependson déclarés dans les fichiers
.jenga, ce n'est pas quelqu'un qui l'a écrit à la main.

Mesure faite le 24/09/2026.
Version de Jenga : 2.8.0.
Dernier commit du dépôt Nkentseu : 9c3fad3, daté du 2026-09-13.
