Exercice 13 - La deuxieme machine

J'ai clone le depot dans un environnement propre, sous /tmp/test_env,
et j'ai note tout ce qui a manque ou failli manquer. Ce journal est la
vraie documentation d'installation du projet.

Datation de la mesure

Commande :

```
git log -1 --format="%h %ad" --date=short
```

Sortie brute (du depot original) :

```
9c3fad3 2026-09-13
```

Mais dans le clone frais, le dernier commit est bd9800a, plus recent.
Ce point est important, on y reviendra.

Etape 1 - Cloner le depot

Premiere tentative :

```
git clone --depth 1 https://github.com/Rihen-Universe/Nkentseu.git Nkentseu
```

Erreur :

```
error: RPC failed; curl 56 GnuTLS recv error (-24): Decryption has failed.
error: 5305 bytes of body are still expected
fetch-pack: unexpected disconnect while reading sideband packet
fatal: early EOF
fatal: fetch-pack: invalid index-pack output
```

Le clone a echoue avec une erreur de decryption GnuTLS. C'est un
probleme connu de git sur WSL avec HTTP/2 et certaines versions de
GnuTLS. La solution n'est pas dans le depot, elle est dans la
configuration de git.

Etape 2 - Corriger git

Commande :

```
git config --global http.version HTTP/1.1
git config --global http.postBuffer 524288000
git config --global http.lowSpeedLimit 0
git config --global http.lowSpeedTime 999999
```

Ces quatre lignes forcent git a utiliser HTTP/1.1 et augmentent les
delais avant echec. C'est une configuration de la machine, pas du
depot. Un autre utilisateur qui n'a pas ces lignes ne pourra pas
cloner le depot si sa configuration est la meme que la mienne de
depart.

Apres cette correction, le clone reussit.

Etape 3 - Charger le workspace

Commande :

```
jenga info
```

Erreur :

```
Error loading workspace: External file not found: Externals/Libs/NKGlad/NKGlad.jenga
Failed to load workspace.
```

Le depot utilise des sous-modules git, places dans Externals/Libs.
Un clone normal ne les telecharge pas. Sans eux, le workspace ne
charge meme pas.

Etape 4 - Initialiser les sous-modules

Commande :

```
git submodule update --init --recursive --depth 1
```

Sortie brute (extrait) :

```
Submodule 'Externals/Libs/NKAssimp' registered for path
Submodule 'Externals/Libs/NKGlad' registered for path
Submodule 'Externals/Libs/NKGLSlang' registered for path
Submodule 'Externals/Libs/Vulkan-Headers-1.4.350' registered for path
Cloning into '/tmp/test_env/Nkentseu/Externals/Libs/NKGlad'...
Submodule path 'Externals/Libs/NKGlad': checked out 'a550abba...'
```

Sept sous-modules ont ete telecharges : ImGui, NKAssimp, NKGLSlang,
NKGlad, NKSPIRVCross, NKShaderc, Vulkan-Headers-1.4.350.

Etape 5 - Recharger le workspace

Commande :

```
jenga info
```

Sortie brute :

```
Jenga Workspace: Nkentseu
Location: /tmp/test_env/Nkentseu
Configurations: Debug, Release
Target OSes: Windows, Linux, macOS, Android, iOS, Web, HarmonyOS
Start project: Sandbox
```

Cette fois le workspace charge. Les sous-modules etaient la condition
manquante.

Etape 6 - Construire

Commande :

```
jenga build --project NKPlatform --config Debug
```

Resultat :

```
Projects Built:  1/1
Status:         SUCCESS
```

Puis NKCanvas :

```
Projects Built:  17/17
Status:         SUCCESS
```

Puis NkRef :

```
Projects Built:  18/18
Status:         SUCCESS
```

Tout compile dans le clone frais. Aucune correction n'a ete
necessaire sur le code source.

Ce que j'ai remarque sur la version du depot

Dans mon depot local, j'avais du corriger trois fichiers du moteur
(NkOpenGLRenderer2D.cpp, NkOpenGLContext.cpp, NkOpenGLComputeContext.cpp)
pour ajouter une definition vide de APIENTRY. Sans cela, NKCanvas ne
compilait pas, et NkRef non plus.

Dans le clone frais, ces trois corrections n'ont pas ete necessaires.
L'explication : mon depot local etait au commit 9c3fad3 du 13/09/2026.
Le clone frais est a un commit plus recent. Entre les deux, le bug
APIENTRY a ete corrige en amont par l'equipe du moteur.

C'est une lecon a retenir : avant de patcher un bug localement,
verifier si une version plus recente du depot ne le corrige pas deja.

Ce que ce journal retient

1. Configurer git pour HTTP/1.1 avant de cloner sur WSL
2. Ne pas oublier git submodule update --init --recursive
3. Verifier la version du depot avant de patcher un bug
4. Un clone frais fonctionne si ces trois points sont respectes

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
Version de git : 2.34.1.
Version de Python : 3.10.12.
