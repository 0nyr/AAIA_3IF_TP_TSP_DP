# AAIA: Traveling Salesman Problem (TSP) resolution with Dynamic Programming (DP)

> This project is configured for Linux, NixOS. Get all the necessary dependencies using the [Nix package manager](https://nixos.org/).

## Introduction

Le problème du TSP (Traveling Salesman Problem) est un problème d’optimisation classique où un voyageur doit parcourir un ensemble de villes exactement une fois, puis revenir à son point de départ. L’objectif est de déterminer l’ordre de visite qui minimise la distance totale parcourue. Malgré son énoncé simple, le TSP est réputé difficile (NP-difficile) et fait l’objet de nombreuses recherches en algorithmique et en heuristiques.

Parmi les approches existantes, la programmation dynamique se distingue comme une méthode exacte capable de résoudre des instances de taille modérée. Son principe consiste à décomposer le problème en sous-problèmes plus petits et à utiliser les solutions de ces sous-problèmes pour construire progressivement la solution globale. Grâce à une mémorisation systématique des résultats intermédiaires, la programmation dynamique réduit les calculs redondants et offre une solution optimale, au prix toutefois d’une complexité fortement croissante, notamment en coût mémoire, avec l'augmentation de la taille du problème.

### Commandes

#### Make compilation

`make dirs`: **Before doing anything**, create output directories for Make rules.

`make build`: Build by compiling project.

`make run`: Run compiled executable.

