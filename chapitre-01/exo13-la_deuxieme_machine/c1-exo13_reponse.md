Exercice 13 - La deuxieme machine

J'ai construit le depot dans un conteneur Docker Ubuntu 22.04 tout
neuf. Ce n'est pas une autre machine physique, mais c'est un
environnement propre au sens strict : image vierge, aucun outil
preinstalle, rien de ma machine WSL. J'ai note tout ce qui a manque
au fur et a mesure. Ce journal est la vraie documentation
d'installation du projet.

Datation de la mesure

Commande :

```
git log -1 --format="%h %ad" --date=short
```

Sortie brute :

```
9c3fad3 2026-09-13
```

Version de Jenga dans le conteneur : 2.8.4. Version de Jenga sur ma
machine WSL : 2.8.0. C'est une difference importante, on y reviendra.

Etape 1 - Lancer un conteneur Ubuntu 22.04 vierge

Commande :

```
docker run -it --rm ubuntu:22.04 bash
```

L'image fait 29 Mo. Elle contient le minimum : bash, apt, et c'est
tout. Aucun outil de developpement n'est preinstalle.

Etape 2 - Verifier ce qui est deja la

Commande :

```
which git python3 pip3 clang cmake make g++
```

Sortie brute :

```
git absent
python3 absent
pip3 absent
clang absent
cmake absent
make absent
g++ absent
```

Rien. C'est le point de depart.

Etape 3 - Installer git

Commande :

```
apt update
apt install -y git
```

Resultat : git version 2.34.1.

Etape 4 - Premier clone du depot

Commande :

```
git clone --depth 1 https://github.com/Rihen-Universe/Nkentseu.git Nkentseu
```

Erreur :

```
error: RPC failed; curl 56 GnuTLS recv error (-24): Decryption has failed.
error: 5524 bytes of body are still expected
fetch-pack: unexpected disconnect while reading sideband packet
fatal: early EOF
fatal: fetch-pack: invalid index-pack output
```

Le meme bug qu'on avait deja vu sur WSL. C'est donc un probleme de
git 2.34.1 et de sa bibliotheque GnuTLS face a GitHub, pas un
probleme de WSL. Le meme bug apparait sur une Ubuntu 22.04 normale.

Etape 5 - Tenter la correction HTTP/1.1

Commande :

```
git config --global http.version HTTP/1.1
git config --global http.postBuffer 524288000
git config --global http.lowSpeedLimit 0
git config --global http.lowSpeedTime 999999
```

Resultat : nouveau clone, meme erreur GnuTLS.

Etape 6 - Mettre a jour les paquets

Commande :

```
apt upgrade -y
```

Resultat : git reste a 2.34.1. Nouveau clone, meme erreur.

Etape 7 - Contourner le controle SSL

Commande :

```
GIT_SSL_NO_VERIFY=1 git clone --depth 1 https://github.com/Rihen-Universe/Nkentseu.git Nkentseu
```

Resultat :

```
Cloning into Nkentseu...
Updating files: 100% (8710/8710), done.
```

Le clone reussit, mais en desactivant la verification du certificat
HTTPS. C'est un contournement, pas une vraie solution. Pour un vrai
environnement de production, il faudrait installer une version plus
recente de git, ou passer par SSH. Pour cet exercice, c'est
suffisant pour debloquer la suite.

Etape 8 - Installer les outils de compilation

Commande :

```
apt install -y python3 python3-pip clang cmake make build-essential
```

Resultat :

```
Python 3.10.12
pip 22.0.2
Ubuntu clang version 14.0.0-1ubuntu1.1
```

A noter : clang 14, plus ancien que celui de ma machine WSL. C'est
la version par defaut d'Ubuntu 22.04, elle suffit pour compiler.

Etape 9 - Installer Jenga

Commande :

```
git clone --depth 1 https://github.com/Rihen-Universe/Jenga.git Jenga
cd Jenga
pip3 install -e . --no-build-isolation
```

Resultat :

```
Successfully installed UNKNOWN-0.0.0
```

Le paquet s'installe, mais sous le nom UNKNOWN. Le fichier
pyproject.toml du depot Jenga n'a pas de nom propre. Consequence :
la commande jenga n'est pas creee.

Etape 10 - Creer la commande jenga

Commande :

```
cat > /usr/local/bin/jenga << FIN
#!/bin/bash
exec python3 -m Jenga \"\$@\"
FIN
chmod +x /usr/local/bin/jenga
```

Verification :

```
which jenga
/usr/local/bin/jenga
jenga --version
Multi-platform C/C++ Build System v2.8.4
```

La version de Jenga est 2.8.4 dans le conteneur, alors qu'elle est
2.8.0 sur ma machine WSL. Le clone frais a recupere la derniere
version disponible sur GitHub.

Etape 11 - Initialiser les sous-modules

Commande :

```
cd Nkentseu
git submodule update --init --recursive --depth 1
```

Sortie brute (extrait) :

```
Submodule path Externals/Libs/ImGui: checked out 9e274c69...
Submodule path Externals/Libs/NKAssimp: checked out df4e0ea9...
Submodule path Externals/Libs/NKGlad: checked out a550abba...
Submodule path Externals/Libs/NKSPIRVCross: checked out a26e468d...
Submodule path Externals/Libs/NKShaderc: checked out 2eded6c2...
Submodule path Externals/Libs/Vulkan-Headers-1.4.350: checked out 9a52351a...
```

Sept sous-modules recuperes. Sans eux, le workspace ne charge meme
pas.

Etape 12 - Premier build, NKPlatform

Commande :

```
jenga build --project NKPlatform --config Debug
```

Resultat :

```
Projects Built:  1/1
Time:           0.32s
Status:         SUCCESS
```

Etape 13 - Deuxieme build, NKMath

Resultat :

```
Projects Built:  5/5
Time:           4.89s
Status:         SUCCESS
```

Etape 14 - Troisieme build, NkRef, premier echec

Commande :

```
jenga build --project NkRef --config Debug
```

Erreur :

```
Compilation Error: glx.c
glad/glx.h:38:10: fatal error: X11/X.h file not found
```

NKGlad a besoin des headers X11 pour compiler sur Linux. C'est une
dependance systeme qui n'est pas presente dans une image Ubuntu nue.

Etape 15 - Installer les headers X11

Commande :

```
apt install -y libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

Etape 16 - Relancer NkRef, deuxieme echec

Erreur :

```
Compilation Error: NkContext.cpp
NkContext.cpp:18:10: fatal error: GL/glx.h file not found
```

NKWindow a besoin des headers OpenGL pour compiler. Encore une
dependance systeme absente.

Etape 17 - Installer les headers OpenGL

Commande :

```
apt install -y libgl1-mesa-dev mesa-common-dev libglx-dev libglu1-mesa-dev
```

Etape 18 - Relancer NkRef, succes

Resultat :

```
Projects Built:  18/18
Time:           16.85s
Status:         SUCCESS
```

Le build complet passe dans le conteneur.

Journal des dependances systeme

Pour construire Nkentseu sur une Ubuntu 22.04 nue, il faut :

1. git
2. python3, python3-pip
3. clang, cmake, make, build-essential
4. Jenga (pip install -e . depuis un clone du depot Jenga)
5. libx11-dev, libxrandr-dev, libxinerama-dev, libxcursor-dev, libxi-dev
6. libgl1-mesa-dev, mesa-common-dev, libglx-dev, libglu1-mesa-dev

Sans ces deux dernieres lignes, deux modules ne compilent pas :
NKGlad (X11) et NKWindow (OpenGL).

Ce que ce journal retient

1. Sur Ubuntu 22.04 de base, git 2.34.1 ne peut pas cloner GitHub
   a cause d'un bug GnuTLS. Il faut GIT_SSL_NO_VERIFY=1, ou
   installer un git plus recent, ou passer par SSH.
2. Le paquet Jenga s'installe sous le nom UNKNOWN et ne cree pas
   la commande jenga. Il faut la creer manuellement.
3. Le depot Nkentseu utilise des sous-modules git. Sans eux, le
   workspace ne charge pas.
4. Deux modules exigent des headers systeme specifiques : NKGlad
   (X11) et NKWindow (OpenGL). Sur une machine de developpement
   classique, ces headers sont presents. Sur une image nue, non.
5. La version de Jenga dans le conteneur est 2.8.4, plus recente
   que celle de ma machine WSL (2.8.0). Ce n'est pas un probleme
   pour la construction, mais il faut le noter.

Ce que je n'ai pas fait

- Je n'ai pas teste sur une autre machine physique, uniquement
  dans un conteneur. C'est un environnement propre, mais le
  noyau Linux reste celui de ma machine hote.
- Je n'ai pas installe une version recente de git, donc le bug
  GnuTLS reste present dans le journal. C'est un point a
  documenter plutot qu'a cacher.

Mesure faite le 25/09/2026.
Version de Jenga dans le conteneur : 2.8.4.
Version de Jenga sur ma machine WSL : 2.8.0.
Version de git dans le conteneur : 2.34.1.
