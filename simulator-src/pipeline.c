// Ce fichier est destinée exclusivement à la structuration de l'exécution pour l'implémentation du pipeline
// Comme expliqué dans le support l'implémentation du pipeline consiste juste à inversé l'ordre d'exécution afin que les fonctions ne dépendent des valeurs précédentes mais suivantes
#include "init.h"
#include "prototype.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int pipeline()
{
    CPU cpu;
    init_cpu(&cpu);           // Initialisation du CPU
    init_mem(&cpu);           // Initialisation de la mémoire
    Display_cpu_status(&cpu); // Afichage des status initials
    printf("\n");

    // Dans cette partie chaque étapes dépendra de la valeur suivantes à travers les registres inter-étages du CPU
    while (cpu.unit_command.clock || cpu.regE.code_op != code_HALT) // Boucle principale avec les tops d'horloge
    {
        MemWriteBackReg *regM = MemWriteBack(&cpu.regE, &cpu);
        cpu.regM = *regM;
        free(regM); // Libéré

        ExecuteMemReg *regE = Execute(&cpu, &cpu.regD);
        cpu.regE = *regE;
        free(regE); // Libéré

        DecodeExecuteReg *regD = Decode(&cpu.regF, &cpu);
        cpu.regD = *regD;
        if (detectStructuralHazard(&cpu)) // Recherche de la présence d'aléa de donnée
        {
            handleStructuralHazard(&cpu); // Méthode de gestion de l'aléa de donnée
        }
        if (detectDataHazard(&cpu)) // Recherche de la présnce de l'aléa de structure
        {
            handleDataHazard(&cpu); // Gestion de l'aléa de structure
        }
        free(regD); // Libération de la mémoire pour éviter des pointeurs sauvages

        FetchDecodeReg *regF = fetch(&cpu);
        cpu.regF = *regF;
        cpu.unit_command.IR = cpu.regF.instruction;
        free(regF); // Libéré

        Display_cpu_status(&cpu);
        printf("\n");
        cpu.cycle += 1;
    }
    return 0;
}
