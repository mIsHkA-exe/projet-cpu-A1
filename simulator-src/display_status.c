// Ce fichier est destiné à l'affichage des status du CPU à chaque cycle
#include <stdio.h>
#include <stdint.h>
#include "init.h"

void Display_cpu_status(CPU *cpu) // Cette méthode init_cpu prend en paramètre un pointeur vers la structure CPU
{
    printf("|-----------Status-------------|\n");
    printf("        |---Registres---|\n"); // Affichage des registres
    for (int i = 0; i < reg_size; i++)     // Paroours du banc des registres
    {
        printf(" R%d : %04d ", i, cpu->reg[i]); // Affichage de chaque contenu de chaque registre
    }
    printf("\n\n");                                                 // Double retour à la ligne pour des questions de lisibilité
    printf("         |---UnitCommand---|\n");                       // Affichage de l'unité de commande
    printf("Program counter [PC] : %04d\n", cpu->unit_command.PC);  // Affichage de la position actuelle du PC
    printf("Current instruction : %04X\n\n", cpu->unit_command.IR); // Affichage de l'instruction courante
    printf("         |---Memory---|\n");                            // Affichage de la mémoire
    for (int i = 0; i < mem_size; i++)                              // Parcours de la mémoire
    {
        printf("[%02d]=%04X  ", i, cpu->memoire[i]); // Affichage de chaque emplacement en mémoire
        if ((i + 1) % 8 == 0)                        // Pour des questions de lisibilité alors on affiche 8 cases par ligne
        {
            printf("\n"); // LISIBILITEEEEEEEEEEEEEEEEEE
        }
    }
}