# Projet de Challenge de Programmation - Pilote de Formule 1

![Notre voiture sur une course](https://github.com/Cleanz7/Picole-nationale/blob/main/raccourcis_boosts.png)

## Enseignants Encadrants
- Mr Ziad-Forrest
- Mr Fourney

## Table des Matières
1. [Introduction](#introduction)
2. [Outils](#outils)
3. [Objectifs](#objectifs)
4. [Conception](#conception)
    - 4.1. [Idée Générale](#idée-générale)
5. [Particularités de Notre Pilote](#particularités-de-notre-pilote)
6. [Difficultés Rencontrées](#difficultés-rencontrées)
    - 6.1. [Les Difficultés](#les-difficultés)
    - 6.2. [Solutions Apportées](#solutions-apportées)
7. [Résultats et Optimisation](#résultats-et-optimisation)
8. [Pistes d'Améliorations](#pistes-daméliorations)
9. [Conclusion](#conclusion)

## Introduction

Le challenge proposé consistait à programmer un pilote de formule 1 évoluant sur un circuit virtuel, en communiquant avec un gestionnaire de course qui fournissait les informations sur la course et les autres pilotes. Ce rapport présente notre approche algorithmique pour résoudre ce problème ainsi que les particularités de notre pilote, les difficultés rencontrées et les solutions apportées.

## Outils

Nous avons utilisé le langage C compilable sous Linux Ubuntu 20.04 LTS, Gitlab, et VisualCode Live Share pour la gestion de version et la collaboration.

## Objectifs

L'objectif principal était de programmer un pilote de formule 1 atteignant l'arrivée le plus rapidement possible tout en respectant les règles de la course. Pour atteindre cet objectif, nous avons établi différentes tâches comme l'analyse des règles de la course, la conception d'un algorithme de mouvement, son implémentation, des tests sur différents circuits, et l'optimisation du code.

## Conception

### Idée Générale

Nous avons utilisé l'algorithme A* pour la planification de la trajectoire, prenant en compte les obstacles et la consommation de carburant. Initialement, l'algorithme était limité, mais nous l'avons étendu pour prendre en compte plusieurs paramètres comme la gestion du sable et des collisions.

## Particularités de Notre Pilote

Notre pilote se distingue par l'utilisation de l'algorithme A* pour la planification de la trajectoire, la gestion des collisions, et la planification de la consommation de carburant.

## Difficultés Rencontrées

### Les Difficultés

Nous avons rencontré des défis tels que l'équilibre entre vitesse et consommation de carburant, l'optimisation du code pour répondre aux contraintes de temps et la détection des collisions.

### Solutions Apportées

Pour résoudre ces difficultés, nous avons ajusté notre fonction d'évaluation heuristique, optimisé notre code en utilisant des structures de données plus efficaces, et renforcé notre détection de collision.

## Résultats et Optimisation

Les résultats ont montré que notre pilote atteignait l'arrivée en un temps raisonnable, mais nous avons encore optimisé le code pour améliorer ses performances.

## Pistes d'Améliorations

Nous envisageons d'améliorer notre algorithme principal en explorant des alternatives comme l'algorithme JPS ou en régressant vers Dijkstra pour des trajectoires plus optimales. De plus, des améliorations pour gérer les virages serrés pourraient être apportées.

## Conclusion

Ce projet nous a permis d'appliquer nos connaissances en algorithmique et programmation pour résoudre un problème complexe de pilotage de formule 1. Nous avons réussi à développer un pilote efficace malgré les défis rencontrés.

## Auteurs 
- Calliste Ravix
- Clément Jantet
