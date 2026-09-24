Exercice 2 - Mesurer avant de croire

J'ai compté moi-même les fichiers source et les lignes du dépôt Nkentseu. Voici
les commandes que j'ai lancées et leurs résultats.

Commande pour compter les fichiers .cpp et .h (sans Externals ni Build) :
find . -type f \( -name "*.cpp" -o -name "*.h" \) -not -path "./Build/*" -not -path "./Externals/*" | wc -l
Résultat : 2755

Commande pour compter les lignes (sans Externals ni Build) :
find . -type f \( -name "*.cpp" -o -name "*.h" \) -not -path "./Build/*" -not -path "./Externals/*" -exec cat {} + | wc -l
Résultat : 1324635

Commande pour compter les fichiers .jenga :
find . -type f -name "*.jenga" | wc -l
Résultat : 219

Comparaison avec le chapitre :

Le chapitre annonce 2641 fichiers source et 1 193 385 lignes. Mes chiffres sont
plus élevés : 2755 fichiers et 1 324 635 lignes.

Pourquoi cet écart :

Le chapitre a été écrit il y a quelques semaines. Depuis, le dépôt a continué
d'évoluer, des fichiers ont été ajoutés et du code a été écrit. C'est un projet
vivant, donc les chiffres changent entre deux mesures.

J'ai exclu le dossier Externals de mes comptages parce qu'il contient des
sous-modules externes (ImGui, NKAssimp, NKGlad, etc.) qui ne font pas partie
du code propre de Nkentseu. Sans cette exclusion, je trouve 4428 fichiers, ce
qui est encore plus loin du chiffre du chapitre.

J'ai bien compté les en-têtes (.h) et les fichiers source (.cpp), comme le
chapitre. Je n'ai pas compté le dossier Build, qui contient des copies
générées des en-têtes et des fichiers intermédiaires.

Pour les fichiers .jenga, j'en trouve 219 contre 221 dans le livre. Le léger
écart vient probablement de la même raison : le dépôt a évolué.

Mesure faite le 24/09/2026.
Version de Jenga : 2.8.0.
Dernier commit du dépôt : 9c3fad3, daté du 2026-09-13.
