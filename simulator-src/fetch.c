// Ce fichier est destiné à l'implémentation de la première étape lors de l'excution d'une instruction : la Fetch
#include "init.h"
#include <stdio.h>
#include <stdlib.h> // Bibliothèque nécessaire pour la manipulation de la mémoire

FetchDecodeReg *fetch(CPU *cpu) // La méthode fetch va prendre en paramètre directement tout le CPU
{
    FetchDecodeReg *regF = (FetchDecodeReg *)malloc(sizeof(FetchDecodeReg)); // Création et allocation de la méémoire pour notre pointeur regF qui va référencer une structure FechDecodeReg en mémoire
    if (!regF)                                                               // Vérification de la réussite de l'allocation de la mémoire
    {
        printf("Allocation echouee");
        exit(1); // Dans le cas échéant alors sortir du programme
    }
    if (cpu->unit_command.PC >= mem_size) // Vérification que le PC n'est pas hors scope dans la mémoire
    {
        printf("PC hors limites : %d\n", cpu->unit_command.PC);
        exit(1); // Dans les cas échéant alors sortir du programme
    }
    else // Dans la mesure où nous sommes toujours dans le scope de la mémoire
    {
        regF->instruction = cpu->memoire[cpu->unit_command.PC]; // Initialisation de la propriété instruction de notre structure FetchDecode à l'instruction courante en mémoire
        regF->index = cpu->unit_command.PC;                     // Initialisation de la propriété index de notre structure FetchDecode à l'index courant du PC
        cpu->unit_command.PC += 1;                              // Incrémentation du compteur de programme à la prochaine instruction en mémoire
        return regF;                                            // Retourner l'objet construit
    }
}

