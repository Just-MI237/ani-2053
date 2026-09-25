Exercice 11 - Casser expres

J'ai introduit une faute de syntaxe dans un fichier de NKMath,
reconstruit, note le temps d'arret, les projets construits, et ce
que le message d'erreur dit sur l'ordre de construction. Puis j'ai
remis le fichier en etat.

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

La faute introduite

Fichier : Kernel/Foundation/NKMath/src/NKMath/NkFunctions.cpp
Ligne 62, dans la fonction NkFloor.

Avant (etat normal) :

```
                float32 NkFloor(float32 x) noexcept {
                        return static_cast<float32>(floor(x));
                }
```

Apres (point-virgule supprime a la fin de la ligne 62) :

```
                float32 NkFloor(float32 x) noexcept {
                        return static_cast<float32>(floor(x))
                }
```

La faute est simple : un point-virgule manquant a la fin d'une
instruction return.

La construction

Commande :

```
rm -rf Build/
time jenga build --project NKMath --config Debug
```

Sortie de time :

```
real    0m13.749s
user    0m24.644s
sys     0m5.438s
```

Build Order affiche par Jenga :

```
Build Order (5 projects):
  1. NKPlatform [STATIC_LIB]
  2. NKCore [STATIC_LIB] (depends: NKPlatform)
  3. NKMemory [STATIC_LIB] (depends: NKCore, NKPlatform)
  4. NKContainers [STATIC_LIB] (depends: NKCore, NKMemory, NKPlatform)
  5. NKMath [STATIC_LIB] (depends: NKContainers, NKCore, NKMemory, NKPlatform)
```

Message d'erreur complet :

```
/home/justmaxit/Projets/Nkentseu/Kernel/Foundation/NKMath/src/NKMath/NkFunctions.cpp:62:41:
error: expected ';' after return statement
                        return static_cast<float32>(floor(x))
                                                             ^
                                                             ;
1 error generated.
```

Bilan affiche par Jenga :

```
Projects Built:  4/5
Failed:         1
Status:         FAILURE
```

Ce que cela dit sur l'ordre de construction

Jenga a construit les quatre premiers projets avant d'arriver a
NKMath. NKPlatform, NKCore, NKMemory et NKContainers ont ete
compiles et lies sans probleme. L'erreur n'apparait qu'au moment
de compiler NKMath lui-meme.

Cela confirme deux choses. D'abord, l'ordre de construction suit
bien le graphe de dependances : NKMath est le dernier parce qu'il
depend de tout le reste. Ensuite, Jenga s'arrete au premier echec.
Il ne continue pas a construire les projets suivants. C'est pour
cela que le bilan indique 4/5 et non 5/5.

Le temps d'arret est de 13.749 secondes. C'est presque le temps
d'une construction complete (environ 17 secondes mesurees a
l'exercice 7). La difference vient du fait que la compilation de
NKMath s'interrompt des le debut, sans compiler tous ses fichiers.

La reparation

J'ai restaure le fichier avec une copie de sauvegarde faite avant
la modification.

Commande :

```
cp /tmp/NkFunctions.cpp.backup Kernel/Foundation/NKMath/src/NKMath/NkFunctions.cpp
jenga build --project NKMath --config Debug
```

Resultat :

```
BUILD COMPLETED
Projects Built:  5/5
Time:           6.84s
Status:         SUCCESS
```

Le projet recompile entierement. La faute est corrigee.

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
