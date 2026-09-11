# Simulateur de Processeur 16 bits en C

## Présentation

Projet académique réalisé dans le cadre du cours d'**Architecture des ordinateurs**.

L'objectif du projet est de concevoir et implémenter en **langage C** un simulateur de processeur **16 bits** permettant de reproduire le fonctionnement interne d'un CPU et d'observer l'exécution des instructions cycle par cycle.

Le simulateur reproduit notamment :

* Un processeur 16 bits
* Une architecture **Von Neumann**
* Une architecture **pipeline à 4 étages**
* Un banc de **4 registres de travail**
* Une mémoire RAM de **32 cases**
* Une **Unité Arithmétique et Logique (UAL)**
* Un **Compteur Ordinal (PC)**
* Des registres inter-étages pour le pipeline
* Un jeu d'instructions réduit
* La gestion des **aléas de structure**
* La gestion des **aléas de données**
* Le mécanisme de **forwarding**
* La simulation du cycle d'instruction en temps réel

---

## Objectifs du projet

Ce projet vise à mettre en pratique les principes fondamentaux de l'architecture des ordinateurs et à comprendre l'interaction entre le matériel et le logiciel.

Les principaux objectifs sont :

* **Concevoir** une architecture simplifiée de processeur 16 bits.
* **Modéliser** les principaux composants internes d'un CPU en langage C.
* **Implémenter** le cycle d'exécution d'une instruction.
* **Structurer** l'exécution autour d'un pipeline à 4 étages.
* **Simuler** l'évolution du processeur à chaque cycle d'horloge.
* **Détecter** et **résoudre** les conflits entre instructions.
* **Valider** le fonctionnement du processeur à l'aide d'un programme de test.

---

## Architecture

Le processeur repose sur une architecture **Von Neumann pipelinée**. Les instructions et les données sont stockées dans une mémoire commune, ce qui permet de simplifier la structure générale du système.

### Composants principaux

| Composant | Description                                          |
| --------- | ---------------------------------------------------- |
| CPU       | Processeur simulé sur 16 bits                        |
| Registres | R0, R1, R2 et R3                                     |
| PC        | Pointeur vers la prochaine instruction               |
| RAM       | Mémoire de 32 cases                                  |
| UAL       | Réalisation des opérations arithmétiques et logiques |
| regD      | Registre inter-étage Decode                          |
| regE      | Registre inter-étage Execute                         |
| regM      | Registre inter-étage Memory                          |
| Pipeline  | Exécution simultanée de plusieurs instructions       |

Le banc de registres contient quatre registres de travail de 16 bits : **R0, R1, R2 et R3**. Le PC conserve l'adresse de la prochaine instruction à exécuter.

---

## Pipeline

L'exécution des instructions est divisée en quatre étapes :

```text
┌─────────┐
│  FETCH  │
└────┬────┘
     ↓
┌─────────┐
│ DECODE  │
└────┬────┘
     ↓
┌─────────┐
│ EXECUTE │
└────┬────┘
     ↓
┌──────────────┐
│ MEMORY / WB  │
└──────────────┘
```

### 1. FETCH

**Récupérer** l'instruction présente à l'adresse indiquée par le PC.

Le PC est ensuite incrémenté afin de pointer vers l'instruction suivante.

### 2. DECODE

**Décoder** l'instruction 16 bits afin d'identifier :

* l'Opcode ;
* les registres sources ;
* le registre destination ;
* les valeurs immédiates.

Cette phase participe également à la détection et à la résolution des **Data Hazards**.

### 3. EXECUTE

**Exécuter** l'opération demandée par l'instruction.

L'UAL réalise notamment :

* `ADD`
* `SUB`

La phase Execute prend également en charge certaines opérations de transfert et les instructions de saut `JMP`.

### 4. MEMORY / WRITE-BACK

**Accéder** à la mémoire lorsque cela est nécessaire et **écrire** les résultats dans leur destination finale.

Cette phase prend notamment en charge :

* `LOAD`
* `STORE`
* l'écriture des résultats dans les registres.

---

## Jeu d'instructions

Le simulateur implémente un ensemble réduit d'instructions :

| Instruction | Fonction                             |
| ----------- | ------------------------------------ |
| `ADD`       | Additionner deux registres           |
| `SUB`       | Soustraire deux registres            |
| `MOV`       | Charger une valeur dans un registre  |
| `LOAD`      | Charger une donnée depuis la mémoire |
| `STORE`     | Écrire une donnée en mémoire         |
| `JMP`       | Modifier le flux d'exécution         |
| `NOP`       | Ne réaliser aucune opération         |
| `HALT`      | Arrêter le processeur                |

Ce jeu d'instructions constitue l'ISA simplifiée utilisée par le simulateur.

---

## Gestion des aléas

L'un des principaux objectifs techniques du projet est la gestion des **Hazards** liés à l'exécution pipelinée.

### Structural Hazards

Un conflit structurel apparaît lorsque plusieurs étapes du pipeline nécessitent simultanément une ressource matérielle qui ne peut être utilisée qu'une seule fois.

Dans notre architecture, le principal conflit concerne l'accès à la mémoire : le `FETCH` et l'étage `MEMORY` peuvent nécessiter simultanément l'accès à la RAM.

La solution implémentée consiste à **insérer une bulle dans le pipeline** et à geler temporairement le `FETCH`.

### Data Hazards

Un Data Hazard apparaît lorsqu'une instruction dépend du résultat d'une instruction précédente qui n'a pas encore terminé son écriture.

Exemple :

```text
MOV R1, 5
ADD R3, R1, R2
```

L'instruction `ADD` dépend directement de la valeur produite par `MOV`.

Pour éviter l'utilisation d'une valeur obsolète, le simulateur implémente un mécanisme de **Forwarding** permettant de récupérer directement le résultat depuis l'étage Execute précédent.

---

## Fonctionnement du simulateur

Le processeur est simulé **cycle par cycle**.

La boucle principale appelle les différentes étapes du pipeline dans l'ordre inverse :

```text
memoryWriteBack()
        ↓
execute()
        ↓
decode()
        ↓
fetch()
```

Cet ordre permet de stabiliser les données des différents étages avant leur transmission au cycle suivant.

Le simulateur permet ainsi d'observer simultanément plusieurs instructions présentes dans différents étages du pipeline.

---

## Programme de test

Un programme de test a été utilisé pour valider le fonctionnement de l'architecture :

```text
MOV R1, 5
MOV R2, 3
ADD R3, R1, R2
STORE R3, [10]
HALT
```

### Déroulement

1. **MOV R1, 5**
   Charger `5` dans `R1`.

2. **MOV R2, 3**
   Charger `3` dans `R2`.

3. **ADD R3, R1, R2**
   Additionner `R1` et `R2`.

4. **STORE R3, [10]**
   Stocker le résultat à l'adresse mémoire `10`.

5. **HALT**
   Arrêter proprement le processeur.

---

## Résultats

À la fin de l'exécution, le simulateur permet de vérifier l'état du processeur.

Résultat attendu :

```text
R1 = 5
R2 = 3
R3 = 8

RAM[10] = 8
```

Ces valeurs permettent de vérifier que les opérations arithmétiques, le transfert de données et l'écriture en mémoire ont été correctement exécutés.

---

## Technologies utilisées

* **Langage :** C
* **Architecture :** Von Neumann
* **Architecture d'exécution :** Pipeline 4 étages
* **Mémoire :** RAM simulée de 32 cases
* **Registres :** 4 × 16 bits
* **UAL :** opérations arithmétiques
* **Gestion mémoire :** `LOAD` / `STORE`
* **Gestion des aléas :** Structural Hazards / Data Hazards
* **Optimisation du pipeline :** Forwarding

---

## Compétences développées

### Programmation en C

* Manipulation de structures de données
* Gestion de la mémoire
* Manipulation de données binaires
* Décalages et masques binaires
* Organisation d'un programme modulaire
* Simulation d'un système informatique

### Architecture des ordinateurs

* Compréhension du fonctionnement d'un CPU
* Cycle Fetch / Decode / Execute / Memory
* Fonctionnement d'une UAL
* Gestion des registres
* Fonctionnement d'une mémoire RAM
* Architecture Von Neumann
* Architecture pipeline
* Gestion des dépendances entre instructions
* Forwarding et insertion de bulles

### Systèmes bas niveau

* Représentation des instructions machine
* Manipulation de mots de 16 bits
* Encodage et décodage d'instructions
* Simulation du comportement matériel
* Synchronisation par cycles d'horloge

---

## Organisation possible du projet

```text
.
├── src/
│   ├── main.c
│   ├── cpu.c
│   ├── cpu.h
│   ├── memory.c
│   ├── memory.h
│   ├── registers.c
│   ├── registers.h
│   ├── pipeline.c
│   ├── pipeline.h
│   └── alu.c
│
├── tests/
│   └── test_program.txt
│
├── docs/
│   └── architecture.md
│
├── Makefile
└── README.md
```

> L'organisation ci-dessus constitue une proposition de structuration du dépôt. Elle peut être adaptée à l'organisation réelle du code source.

---

## Compilation

Si le projet utilise GCC et un Makefile :

```bash
make
```

Puis lancer le simulateur :

```bash
./cpu_simulator
```

Compilation manuelle possible :

```bash
gcc -Wall -Wextra -std=c11 src/*.c -o cpu_simulator
```

---

## Exemple de sortie

```text
================================
   16-BIT CPU SIMULATOR
================================

Cycle 1
FETCH  : MOV R1, 5

Cycle 2
FETCH  : MOV R2, 3
DECODE : MOV R1, 5

Cycle 3
FETCH  : ADD R3, R1, R2
DECODE : MOV R2, 3
EXECUTE: MOV R1, 5

...

--------------------------------
FINAL CPU STATE
--------------------------------

R1 = 5
R2 = 3
R3 = 8

RAM[10] = 8

CPU STATUS: HALTED
```

La sortie exacte dépend de l'implémentation du simulateur.

---

## Auteurs

Projet réalisé dans le cadre du cursus d'architecture des ordinateurs à l'**ESILV**.

* Kyllian Siewe Tiague
* Houssam Bendriouich
* Wassim Laarbi

Date de finalisation du projet : **25 mai 2026**.

---

## Contexte académique

Ce projet s'inscrit dans une démarche de compréhension du fonctionnement interne des processeurs et de l'interaction entre les couches matérielles et logicielles.

Il permet notamment de passer d'une représentation théorique d'un processeur à une **simulation fonctionnelle en langage C**, en prenant en compte les contraintes liées à l'exécution pipelinée et aux dépendances entre instructions.
