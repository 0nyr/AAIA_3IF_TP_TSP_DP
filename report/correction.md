---
header-includes:
    - \usepackage{amssymb}
    - \usepackage[utf8]{inputenc}
mainfont: Tinos
monofont: 'JetBrainsMono Nerd Font'
---

# AAIA: Traveling Salesman Problem (TSP) resolution with Dynamic Programming (DP)

Florian Rascoussier, Aurélien Delage, Christine Solnon

Mars 2025

>[!WARNING]
This file is old, see the latest version on my Obsidian.


## 0. Introduction

>Voir le sujet: **sujet-TSP-DP-2022.pdf**

Ce TP est l'occasion de se plonger dans un problème combinatoire classique et de découvrir du même coup une technique de programmation très utile.

### Le Problème du Voyageur de Commerce (TSP)

Le problème du TSP (Traveling Salesman Problem) est un problème d’optimisation classique où un voyageur doit parcourir un ensemble de villes exactement une fois, puis revenir à son point de départ. L’objectif est de déterminer l’ordre de visite qui minimise la distance totale parcourue. Malgré son énoncé simple, le TSP est réputé difficile (NP-difficile) et fait l’objet de nombreuses recherches en algorithmique et en heuristiques.

### La Programmation Dynamique (DP)

La programmation dynamique (PD) est une méthode exacte couramment utilisée pour résoudre des instances de taille modérée de problèmes combinatoires, comme le TSP. Son principe général consiste à décomposer le problème initial en de multiples sous-problèmes plus simples, puis à mémoriser (ou « mémoïser ») leurs solutions pour éviter les recalculs inutiles. Grâce à cette approche, on parvient à réduire considérablement la redondance des calculs et à obtenir une solution optimale. Toutefois, la complexité en temps et en mémoire tend à croître rapidement à mesure que la taille du problème augmente.

Pour illustrer cette méthode, vous pouvez visionner une courte présentation en anglais : [How Dynamic Programming Broke Software Engineers](https://youtu.be/KLW9gDP6nN8?si=jO6ZUUZC1MaB35W4).

En pratique, la programmation dynamique se déroule en trois étapes principales :

1. **Résolution itérative ou récursive** : le problème est abordé étape par étape ou de manière récursive.  
2. **Sauvegarde des résultats intermédiaires** : on enregistre ces résultats partiels pour éviter de les recalculer.  
3. **Réutilisation** : on s’appuie sur ces résultats pour construire la solution finale, sans relancer les mêmes calculs.

On distingue généralement deux approches de la programmation dynamique :

1. **Top-Down (récursion et mémoïsation)** : on part d’un appel récursif du problème principal, et dès qu’un sous-problème se répète, on en réutilise le résultat précédemment mémorisé.  
2. **Bottom-Up (tabulation)** : on calcule systématiquement tous les sous-problèmes, en commençant par les plus simples pour remonter progressivement vers la solution finale. Durant ce processus, on remplit une « table » où chaque entrée correspond à un sous-problème intermédiaire, facilitant ainsi le passage à des niveaux de complexité supérieurs.


## 1. Questions

*Les assertions suivantes sont-elles vraies ou fausses ? (voir les diapos 112 et 113 du cours)*

### Question 1.1

*Un problème appartient à la classe $NP$ s’il n’est pas possible de le résoudre en temps polynomial.*

**Réponse : Faux**:
- **Définition de NP.** Un problème appartient à NP si on peut vérifier *en temps polynomial* la validité d’une solution candidate (ou, de manière équivalente, si une machine de Turing *non déterministe* peut le résoudre en temps polynomial).  
- **Confusion courante.** Dire qu’« il n’existe pas d’algorithme polynomial » pour le problème ne correspond pas à la définition de NP, mais plutôt à l’un des scénarios de la question ouverte « P vs NP ».  
- **P vs NP.** Nous ne savons pas encore si un *algorithme polynomial* (en machine déterministe) existe pour résoudre **tous** les problèmes de NP. Affirmer qu’aucune solution polynomialement efficace n’existe reviendrait à prouver que $P \neq NP$, ce qui est un problème non résolu en informatique théorique.

### Question 1.2
*Un problème appartient à la classe NP s’il existe un algorithme permettant de le résoudre en temps polynomial sur une machine de Turing non déterministe.*  

**Réponse : Vrai.**  
- **Machine de Turing non déterministe.** C’est la caractérisation la plus couramment utilisée pour définir NP : un problème est dans NP s’il peut être résolu en temps polynomial par un dispositif de calcul non déterministe.  
- **Vérification polynomiale.** L’équivalent déterministe, plus intuitif, est la capacité de *vérifier* rapidement une solution proposée.

### Question 1.3
*Les problèmes NP-complets sont les problèmes les plus difficiles de la classe NP.*

**Réponse : Vrai (avec nuances).** 
- **NP-complétude.** Un problème est dit NP-complet s’il est à la fois dans NP et NP-difficile, c’est-à-dire qu’il « domine » (via des réductions polynomiales) tous les autres problèmes de NP.  
- **“Les plus difficiles.”** Cette expression signifie que si un problème NP-complet admettait un algorithme déterministe en temps polynomial, alors **tous** les problèmes de NP pourraient également être résolus en temps polynomial, ce qui impliquerait que $P = NP$.  
- **Statut ouvert.** Personne n’a encore prouvé ou réfuté l’existence d’un tel algorithme polynomial. Les problèmes NP-complets sont « considérés » comme les plus durs de NP tant que l’on n’a pas établi la preuve que $P \neq NP$.

### Question 1.4
*Tout problème NP-complet appartient à la classe NP.*  

**Réponse : Vrai.**  
- **Par Définition.** Pour être NP-complet, il faut d’abord être dans NP. Ensuite, on montre que ce problème est au moins aussi difficile que n’importe quel autre problème de NP (NP-hard).  

### Question 1.5
*Tout problème NP-difficile appartient à la classe NP.*

**Réponse : Faux.**  
- **NP-difficile.** Dire qu’un problème est NP-difficile signifie qu’il est *au moins* aussi dur que les problèmes de NP. Il peut donc se trouver en dehors de NP (cas de certains problèmes indécidables, par exemple).  
- **Comparaison.** Les problèmes NP-complets sont à la fois NP-difficiles et *dans* NP. Mais un problème NP-difficile n’est pas nécessairement dans NP (il peut être « encore plus compliqué » ou non décidable en temps fini).

---

En résumé, il est essentiel de ne pas confondre « être dans NP » avec « ne pas pouvoir se résoudre en temps polynomial ». Tant que la question « P = NP ? » n’est pas tranchée, nous ne pouvons pas affirmer de manière définitive qu’il n’existe pas de solution déterministe en temps polynomial pour les problèmes NP-complets.

En pratique, si l’on sait qu’un problème est NP-difficile comme c'est le cas pour le TSP, on considère qu’il est peu envisageable de le résoudre efficacement avec un simple algorithme déterministe. Dès lors que la taille du problème devient importante, on recourt généralement à des techniques avancées (exactes, approchées ou hybrides) ou à des heuristiques afin d’obtenir une solution de bonne qualité dans un temps et avec une mémoire raisonnables.

## 2. Algorithme de Held et Karp

L’algorithme de Held & Karp est une méthode classique et exacte pour résoudre le problème du voyageur de commerce (TSP) à l’aide de la programmation dynamique. Son idée de base consiste à stocker, pour chaque sous-ensemble de villes et pour chaque ville de ce sous-ensemble, le coût minimal d’un chemin reliant ces villes et se terminant dans la ville en question. Cet algorithme procède donc en deux grandes étapes :

1. **Énumération des sous-ensembles** : on considère tous les sous-ensembles possibles de villes, puis on construit la solution optimale pour chacun de ces sous-ensembles en s’appuyant sur les solutions des sous-problèmes plus petits (sous-ensembles de taille inférieure).  
2. **Combinaison des solutions partielles** : à partir de ces résultats mémorisés, on détermine progressivement la meilleure façon de boucler le circuit, de sorte à visiter l’ensemble des villes une seule fois avant de revenir au point de départ.

Grâce à la mémorisation systématique des solutions intermédiaires, on évite de recalculer plusieurs fois les mêmes quantités. Held & Karp parviennent ainsi à trouver une solution optimale, mais leur approche présente une complexité en temps de l’ordre de $O(n^2 \cdot 2^n)$, classique des algos de DP. Elle reste donc applicable en pratique surtout pour des instances de taille modérée, avant que l’**explosion combinatoire** ne la rende trop coûteuse en temps et en mémoire.

Notez que traditionnellement, l'algorithme de Held & Karp pour le TSP est présenté sous une forme **Bottom-Up**, car on va remplir de manière itérative une table pour tous les sous-ensembles de villes de taille croissante. Autrement dit, on part des sous-problèmes les plus petits (sous-ensembles réduits) et on “remonte” progressivement vers la solution globale.

### Question 2.1

*Combien y-a-t-il de sous-problèmes différents possibles dans le cas d'un graphe comportant n sommets ?*

Une chose à bien voire en regardant la définition d'un sous problème mémorisé $D$ est qu'on ne mémorise pas une liste (ensemble ordonée) de sommets précédement visités, mais bien un ensemble (non-ordonné), correspondant au meilleur sous problème rencontré à cette étape. Le fait de perdre l'ordre des chemins permet en pratique de mémoriser seulement ce qui nous intéresse pour calculer les sous-problèmes suivants, c'est-à-dire la valeur du meilleur sous-chemin actuel. A cause de cela, et pour se permettre d'écrire les ensembles non ordonnés d'une facon similaire, on écrira toujours un ensemble non ordonné $E$ sous **forme canonique**, c'est à dire en écrivant ses éléments dans l'ordre lexicographique. Exemple, si $E$ contients les sommets $1, 3, 2, 8, 5$, on écrira $E = \{1, 2, 3, 5, 8\}$.

Rappel: Combinaison - Le nombre de façons de choisir un échantillon de k éléments à partir d'un ensemble de n objets distincts où l'ordre n'a pas d'importance et où les remplacements ne sont pas autorisés.

Pour bien comprendre combien de sous-problèmes on a, rien de tel qu'un petit exemple pour se donner l'idée de la preuve: Pour un graphe complet avec 3 sommets:

1. Premier niveau: 3 choix possibles pour i, $E = \emptyset$ donc 1 seul choix pour E.
2. 2nd niveau: 2 choix possibles pour i. A ce niveau, 1 noeud a déjà été visité, c'est-à-dire que $\#E = 1$, donc $\binom{3}{1} = 3$ valeures possibles pour E: $E \in \{\{0\}, \{1\}, \{2\} \} \:|\: E \in \{ \{0, 1\},  \{0, 2\},  \{1, 2\}\}$
3. 3eme niveau: 1 choix obligatoire pour i, $\#E = 2, \#E = \binom{3}{2} = 3$

On a donc: $\sum_{\forall Niv}(\#i_{Niv}*\#E_{Niv}) = (3*1) + (2*3) + (1*3) = 12$ sous-problèmes différents. Auquel on peut ajouter le sous-problème *origine* $D(0, \{\})$ et le sous-problème *destination* $D(0, \{1, 2, 3\})$. Ces deux sous-problèmes sont toujours présents et donc sous-entendu (on ne les compte pas).

#### Preuve par Aurélien Delage:

##### Analyse du nombre de sous-problèmes dans un algorithme récursif avec mémoïsation

On rappelle que le nombre de **sous-problèmes distincts** correspond au nombre de **nœuds dans l’arbre des appels récursifs** du calcul du problème global, **à condition d’identifier les appels récursifs identiques**, c’est-à-dire d’appliquer de la **mémoïsation**.

Pour un problème de la forme $D(n, S)$, l’ensemble des sous-problèmes correspond à ceux de la forme $D(i, S \setminus \{i\})$, qui eux-mêmes se décomposent récursivement. Sans mémoïsation, on obtient $n!$ appels récursifs (i.e., toutes les permutations possibles).

---

##### Comptage avec mémoïsation

À chaque niveau $k$ de la récursion :
- $k$ sommets ont été choisis pour descendre dans l’arbre,
- Il y a donc $\binom{n}{k}$ ensembles possibles pour le second argument $S$,
- Le premier argument de $D$ peut alors prendre $(n - k)$ valeurs possibles.

En résumé, le nombre total de sous-problèmes distincts est :

$$
\sum_{k=0}^{n} \binom{n}{k} \cdot (n - k) = \mathcal{O}(n \cdot 2^n)
$$

Ce $\mathcal{O}$ n’est pas une approximation grossière. On a en effet :

$$
\sum_{k=0}^{n} \binom{n}{k} \cdot (n - k)
= \sum_{k=0}^{n} \binom{n}{k} \cdot k
= n \cdot 2^{n - 1}
$$

##### Complexité temporelle totale

La complexité de $\mathcal{O}(n^2 \cdot 2^n)$ mentionnée dans l’image vient de :

- $\mathcal{O}(n \cdot 2^n)$ sous-problèmes,
- Chaque sous-problème nécessite un **opérateur de Bellman**, qui agrège les solutions des sous-sous-problèmes en une solution du sous-problème courant,
- Cet opérateur coûte en moyenne $\mathcal{O}(n)$.

Donc la complexité totale est :

$$
\mathcal{O}(n \cdot 2^n) \cdot \mathcal{O}(n) = \mathcal{O}(n^2 \cdot 2^n)
$$

Cette analyse est typique des algorithmes de **programmation dynamique** sur des ensembles, comme le **TSP avec mémoïsation**, où les sous-problèmes sont indexés par un **état partiel** (ex. un sommet actuel et un ensemble de sommets visités).



### Question 2.2
*Quel est le sous-problème donnant la longueur du plus court circuit hamiltonien partant de 0 et revenant
sur 0 ?*

C'est le sous-problème: $D(0, S \ {0})$. Ce sous-problème part de $i=0$ jusqu'à $0$ (par définition). C'est un circuit hamiltonien car son ensemble de sommet visités $E = S \setminus \{0\}$ contient tous les sommets (sauf $i=0$ par définition).

## 3. Implémentation naïve.

On considère `src/TSPnaif.c`.

### Questions 5, 6, 7
```
 ❮onyr ★ kenzae❯ ❮AAIA_3IF_TP_TSP_DP❯❯ ./bin/TSPnaifO3 
Number of vertices: 8
Length of the smallest hamiltonian circuit = 2633; CPU time = 0.000s
    - Number of calls to computeD = 13700
 ❮onyr ★ kenzae❯ ❮AAIA_3IF_TP_TSP_DP❯❯ ./bin/TSPnaifO3 
Number of vertices: 10
Length of the smallest hamiltonian circuit = 2735; CPU time = 0.010s
    - Number of calls to computeD = 986410
 ❮onyr ★ kenzae❯ ❮AAIA_3IF_TP_TSP_DP❯❯ ./bin/TSPnaifO3 
Number of vertices: 12
Length of the smallest hamiltonian circuit = 2801; CPU time = 1.371s
    - Number of calls to computeD = 108505112
```

### Question 8:

*Combien y-a-t-il d’appels récursifs par rapport au nombre de sous-problèmes différents ?*

>[!WARNING]
Dans le code de Christine, le set $s$ correspond à l'inverse de celui défini dans le sujet: il correspond aux noeuds qu'il est encore possible de visiter, et non pas à ceux déjà visités.

```python
>>> def calc(x): return (x)*(2**(x-1))
... 
>>> calc(3)
12
>>> calc(8)
1024
```

```
 ❮onyr ★ kenzae❯ ❮AAIA_3IF_TP_TSP_DP❯❯ ./bin/TSPnaifO3 
Number of vertices: 3
Length of the smallest hamiltonian circuit = 1904; CPU time = 0.000s
    - Number of calls to computeD = 5
 ❮onyr ★ kenzae❯ ❮AAIA_3IF_TP_TSP_DP❯❯ ./bin/TSPnaifO3 
Number of vertices: 8
Length of the smallest hamiltonian circuit = 2633; CPU time = 0.001s
    - Number of calls to computeD = 13700
```

Il n'y a pas le même nombre d'appel récursifs que de sous-problèmes différents. Cela vient du fait de l'implémentation qui diffère du pseudocode. En pratique le nombre d'appels croit plus vite que le nombre de sous-problème alors ce ce dernier est déjà exponentiel à cause de l'ordre naïf des appels récursifs.


