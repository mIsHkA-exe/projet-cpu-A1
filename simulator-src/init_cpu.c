// Ce ficher nous permet d'initialiser le cpu pour chaque nouvelle simulation 
#include <stdio.h>
#include "init.h"
#include <string.h> // Bibliothèque utilisé pour la manipulation des chaines de caractères 

void init_cpu(CPU *cpu) // Cette méthode init_cpu prend en paramètre un pointeur vers la structure CPU 
{
    memset(cpu->memoire, 0, sizeof(cpu->memoire)); ; // Initialisation de toutes les cases à 0
    memset(cpu->reg, 0, sizeof(cpu->reg)) ; // Initialisation de toutes les cases à 0
    cpu->unit_command.PC = 0;
    cpu->unit_command.IR = 0;
    cpu->unit_command.clock = true;
    cpu->unit_Treatment.accumulator = 0;
    cpu->unit_Treatment.state_reg = 0 ;
    cpu->cycle = 0 ;
    cpu->regE.bubble = true ;
    cpu->regM.bubble = true ;
    cpu->regD.bubble = true ; 
    cpu->count_bubble = 0 ;
    //En gros tous les composants dans le CPU sont initialisé à 0 
}