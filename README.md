# Nachos — Projet Systèmes d'Exploitation

Implémentation de fonctionnalités noyau (appels système, threads utilisateurs, pagination) sur **Nachos**, le système d'exploitation pédagogique conçu à Berkeley. Projet réalisé en **binôme** (avec Lilian Limousy) dans le cadre du cours « Projet Systèmes d'Exploitation », UF Informatique, **Université de Bordeaux**, d'octobre à décembre 2025.

## Fonctionnalités implémentées

- **Appels système console** : `GetChar`, `PutString`/`GetString`, `PutInt`/`GetInt`, copie sécurisée de chaînes entre mémoire noyau et mémoire utilisateur MIPS (`copyStringFromMachine`/`copyStringToMachine`)
- **Threads utilisateurs** : `ThreadCreate`/`ThreadExit`, allocation des piles utilisateur par bitmap (8 piles de 256 octets), synchronisation de la console par sémaphore
- **Pagination et multiprogrammation** : chargement des exécutables via `ReadAtVirtual` (table de pages, mapping non identitaire), classe `PageProvider` (bitmap des pages physiques), appel système `ForkExec` pour exécuter plusieurs programmes simultanément, gestion de plusieurs threads par processus

## Technologies

- **Langage** : C++ (sous-ensemble Nachos), assembleur MIPS pour les programmes de test
- **Build** : `make` (noyau compilé nativement en x86_64, pas de cross-compilation nécessaire)
- **Compilateur** : g++, avec AddressSanitizer/UndefinedBehaviorSanitizer activés
- **Simulateur** : machine MIPS simulée en logiciel (`machine/mipssim.cc`)

## Installation et lancement

Aucune dépendance externe : juste `make` et `g++`.

```bash
cd code/threads   # ou code/userprog pour les fonctionnalités de ce projet
make
./nachos
```

**Build vérifié sur cette version** : `code/threads` et `code/userprog` compilent sans erreur ni avertissement (g++ 11.4, `-Wall -Wextra -Wshadow`).

## Tests et validation

Pas de suite de tests automatisée (ce n'est pas l'usage pour ce type de projet système) : validation par des programmes de test exécutés dans le simulateur, et par 3 rapports rendus (`rendus/rapport1.pdf`, `rapport2.pdf`, `rapport3.pdf`).

**Résultats mesurés et documentés dans les rapports** :
- Test de stress `dozen_threads_launcher` : **12 processus × 12 threads = 144 threads simultanés**, système stable
- Analyse mémoire (LeakSanitizer) : fuite résiduelle de 1 344 136 octets sur 117 allocations, identifiée et expliquée (pile du dernier thread noyau non libérée avant l'arrêt de la machine — pas un vrai bug fonctionnel)

## Auteurs

| Contributeur | Rôle |
|---|---|
| **Kousay Amara** | Étudiant — voir « Ce que j'ai réalisé » |
| Lilian Limousy | Étudiant, binôme |
| Samuel Thibault | Enseignant — auteur/mainteneur du code de base Nachos pour ce cours |
| Raymond Namyst | Enseignant |
| Emmanuel Agullo | Encadrant (Inria) |

*(Les adresses email des contributeurs autres que moi ont été anonymisées lors de la migration de ce dépôt.)*

## Ce que j'ai réalisé (Kousay Amara)

Travail réalisé en binôme avec Lilian Limousy, d'après l'historique Git et les 3 rapports co-signés :

- **TP1 (appels système console)** : implémentation des fonctions de copie sécurisée entre mémoire noyau et mémoire utilisateur (`copyStringFromMachine`/`copyStringToMachine`), avec gestion de buffer borné pour éviter les débordements.
- **TP2 (threads utilisateurs)** : ajout des appels système `ThreadCreate`/`ThreadExit`, allocation des piles utilisateur par bitmap.
- **TP3 (pagination)** : implémentation de `ReadAtVirtual` pour le chargement via table de pages, participation à `PageProvider` (allocation des pages physiques).

Fichiers principalement modifiés : `code/userprog/userthread.cc`, `code/userprog/exception.cc`, `code/userprog/consoledriver.cc`, `code/userprog/addrspace.cc`.

## Origine du code et licence

Ce dépôt contient le **code de base Nachos**, développé à l'origine par UC Berkeley et maintenu pour ce cours par Samuel Thibault et l'équipe enseignante de l'Université de Bordeaux. Il est couvert par la licence académique permissive présente dans le fichier [COPYRIGHT](COPYRIGHT) (University of California, 1992-1993), qui autorise explicitement la copie, la modification et la distribution à condition d'en conserver la notice. **Ce fichier n'a pas été modifié et doit être conservé tel quel.**

Les fonctionnalités listées dans « Ce que j'ai réalisé » ci-dessus sont nos ajouts (Lilian Limousy et moi) par-dessus ce code de base.
