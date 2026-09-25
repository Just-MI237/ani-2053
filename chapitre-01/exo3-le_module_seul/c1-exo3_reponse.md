Exercice 3 - Le module seul

J'ai lance la commande :

    jenga build --project NKMath --config Debug

Sortie brute de l'outil, collee telle quelle :

    Build Order (5 projects):
      1. NKPlatform [STATIC_LIB] →
      2. NKCore [STATIC_LIB] (depends: NKPlatform) →
      3. NKMemory [STATIC_LIB] (depends: NKCore, NKPlatform) →
      4. NKContainers [STATIC_LIB] (depends: NKCore, NKMemory, NKPlatform) →
      5. NKMath [STATIC_LIB] (depends: NKContainers, NKCore, NKMemory, NKPlatform)

Bilan complet affiche a la fin du build :

    ════════════════════════════════════════════════════════════════
                                    BUILD COMPLETED
    ════════════════════════════════════════════════════════════════
    Projects Built:  5/5
    Time:           5.10s
    Status:         ✓ SUCCESS
    ════════════════════════════════════════════════════════════════

Voici l'arbre de construction, lu du sommet (construit en dernier) vers
le socle (construit en premier) :

- NKMath (depend de NKContainers, NKCore, NKMemory, NKPlatform)
  - NKContainers (depend de NKCore, NKMemory, NKPlatform)
    - NKMemory (depend de NKCore, NKPlatform)
      - NKCore (depend de NKPlatform)
        - NKPlatform (ne depend de rien)

Ce que cet arbre montre :

NKPlatform est tout en bas, c'est le socle. Il ne depend de rien et
c'est le premier construit. Tout le reste repose dessus.

NKMath est tout en haut. Il depend directement de quatre modules
differents, qui reposent tous sur NKPlatform. C'est pour cette raison
que la figure n'est pas une simple colonne : plusieurs branches
partent de NKMath et convergent toutes vers NKPlatform.

Si on retire NKPlatform du socle, tout s'effondre. Si on retire
NKMemory, seuls NKContainers et NKMath tombent, mais NKCore et
NKPlatform tiennent encore.

C'est Jenga qui calcule cet ordre a partir des dependances declarees
dans les fichiers .jenga, ce n'est pas quelqu'un qui l'a ecrit a la
main.

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
Dernier commit du depot Nkentseu : 9c3fad3, date du 2026-09-13.
