Exercice 6 - Les deux erreurs de dependance

J'ai teste quatre etats du projet MonEssai. Dans chaque etat, main.cpp
appelle nkentseu::platform::GetPlatformConfig(), une fonction definie
dans NkPlatformConfig.cpp. Sans cette fonction, le lien echoue.

Datation de la mesure

Commande :

```
git log -1 --format="%h %ad" --date=short
```

Sortie brute :

```
9c3fad3 2026-09-13
```

Version de Jenga : 2.8.0. C'est important pour la conclusion, parce
que le comportement de dependson a change selon les versions.

Cas 0 - dependson et links presents

Commande :

```
rm -rf Build/
jenga build --project MonEssai --config Debug
```

Sortie brute :

```
Build Order (2 projects):
  1. NKPlatform [STATIC_LIB]
  2. MonEssai [CONSOLE_APP] (depends: NKPlatform)
Projects Built:  2/2
Status:         SUCCESS
```

Cas 1 - links seul, sans dependson

Commande :

```
rm -rf Build/
jenga build --project MonEssai --config Debug
```

Sortie brute :

```
Build Order (1 projects):
  1. MonEssai [CONSOLE_APP]
/usr/bin/ld: cannot find -lNKPlatform: No such file or directory
clang: error: linker command failed with exit code 1
Status:         FAILURE
```

Cas 2 - dependson seul, sans links

Commande :

```
rm -rf Build/
jenga build --project MonEssai --config Debug
```

Sortie brute :

```
Build Order (2 projects):
  1. NKPlatform [STATIC_LIB]
  2. MonEssai [CONSOLE_APP] (depends: NKPlatform)
Projects Built:  2/2
Status:         SUCCESS
```

Cas 3 - ni dependson ni links

Commande :

```
rm -rf Build/
jenga build --project MonEssai --config Debug
```

Sortie brute :

```
Build Order (1 projects):
  1. MonEssai [CONSOLE_APP]
/usr/bin/ld: MonEssai/src_main.o: in function main:
main.cpp:(.text+0x10): undefined reference to
  nkentseu::platform::GetPlatformConfig()
clang: error: linker command failed with exit code 1
Status:         FAILURE
```

Table de diagnostic

- dependson + links    -> build reussi
- links seul           -> cannot find -lNKPlatform
- dependson seul       -> build reussi (Jenga 2.8.0)
- ni l'un ni l'autre   -> undefined reference a GetPlatformConfig

Ce que les deux messages disent

cannot find -lNKPlatform veut dire que la bibliotheque n'a pas ete
construite avant. dependson manque. La contrainte d'ordre n'existe
pas, donc Jenga n'a pas construit NKPlatform avant MonEssai. Au
moment de lier, le fichier .a n'existe pas.

undefined reference a GetPlatformConfig veut dire que la bibliotheque
n'est pas dans la ligne de lien. Ni dependson ni links ne sont
declares, donc Jenga ne sait pas qu'il doit chercher le symbole dans
NKPlatform. Le fichier .a a ete construit par chance (un build
precedent), mais il n'est pas utilise.

Et le cas 2 ?

Avec dependson seul, le build reussit. La bibliotheque est construite
avant, et Jenga ajoute aussi le drapeau de lien vers le projet du
workspace. C'est une decision de conception, et elle n'est pas la
meme dans toutes les versions de Jenga. Dans la version 2.8.0
utilisee ici, dependson implique le lien. C'est pour ca que le cas 2
reussit. Dans une version plus ancienne, ce cas aurait echoue avec
undefined reference.

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
