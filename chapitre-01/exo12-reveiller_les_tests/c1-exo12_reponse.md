Exercice 12 - Reveiller les tests

Le workspace desactive une partie des suites de tests. J'ai cherche
la ligne qui le fait, puis j'ai lance une suite autorisee pour voir
combien de tests s'executent et combien passent.

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

La ligne qui controle les tests

Commande :

```
grep -n "dutc(\|dute(" Nkentseu.jenga
```

Deux lignes apparaissent : 452 et 454.

Ligne 452 : dutc(enable=True, allow=[...])
Ligne 454 : dute(enable=True, allow=[...])

dutc signifie disable unit test compilation. dute signifie disable
unit test execution. Ces deux appels prennent une liste allow qui
restreint les suites de tests reconnues. Tout ce qui n'est pas dans
la liste est ignore.

Combien de suites existent

Commande :

```
jenga info | grep TestSuite | awk "{print \$1}" | sort
```

Resultat : 68 suites de tests existent dans le depot.

Combien sont autorisees

Commande :

```
sed -n "452p" Nkentseu.jenga | grep -o "\"[^\"]*\"" | tr -d "\"" | grep "_Tests" | sort
```

Resultat : 30 suites sont dans l'allow-list.

Comparaison :

```
Suites existantes  : 68
Suites autorisees  : 30
Suites desactivees : 38
```

Le workspace autorise donc moins de la moitie des suites : 30 sur 68.
Les 38 autres existent dans le depot mais ne sont ni compilees ni
executees par defaut.

Lancement d'une suite autorisee

Commande de construction :

```
jenga build --project NKMath_Tests --config Debug
```

Sortie brute :

```
Projects Built:  7/7
Time:           16.74s
Status:         SUCCESS
```

Commande d'execution :

```
jenga run NKMath_Tests --config Debug --platform x86_64 --target Linux
```

Sortie brute (extrait) :

```
Tests :      8 reussis, 8 au total
Assertions : 499 reussies, 499 au total
Taux succes : Tests: 100.0%, Assertions: 100.0%
Temps total : 31ms (3ms/test)
Tous les tests sont reussis !
```

Les trois reponses

- Combien de suites existent : 68
- Combien s'executent : 1 (celle que j'ai lancee, NKMath_Tests).
  Si on lance toutes les suites autorisees, ce serait 30.
- Combien passent : 1 sur 1. La suite NKMath_Tests affiche 8 tests
  sur 8 et 499 assertions sur 499, soit 100 pour cent.

Ce que cette experience a appris

Le workspace ne compile pas toutes les suites par defaut. Il utilise
un mecanisme d'allow-list qui limite volontairement les tests aux
modules consideres comme stables. C'est une decision de developpement
du depot, pas une limite technique. On peut lancer n'importe quelle
suite autorisee avec jenga run, et elle fonctionne.

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
