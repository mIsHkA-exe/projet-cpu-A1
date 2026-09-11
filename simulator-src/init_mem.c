// Ce fichier nous permet de remplir la mémoire avec le jeu d'instruction à simuler 
#include <stdio.h>
#include "init.h"

void init_mem(CPU *cpu) // Notre fonction int_mem prend en paramètre un pointeur vers notre structure CPU
{
    cpu->memoire[0] = 0x3485; // MOV R1 , 5    = 0011 01 00 1000 0101
    cpu->memoire[1] = 0x3883;
    cpu->memoire[2] = 0x1D02;
    cpu->memoire[3] = 0x438A;
    cpu->memoire[4] = 0xF000 ;

    //En gros toutes les instructions sont initialisée dans une case de la mémoire
    //Les codes Hex des instrutions ont été illustrés en fonction des valeurs que l'on avait mise aux instructions dans le fichier init.h
    //Exemple : MOV R1,5 
    // MOV => 0011 (3) R1 => 0100 data = 1000 0101 (5) NB : le 1 au début du code de la data correspond au mode 
}