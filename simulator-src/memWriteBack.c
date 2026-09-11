#include "init.h"
#include "prototype.h"
#include <stdio.h>
#include <stdlib.h>

MemWriteBackReg *MemWriteBack(ExecuteMemReg *regE, CPU *cpu) // La méthode MemWriteBAck va prendre en paramètre directement tout le CPU et l'objet de configuration envoyé par Execute
{
    if (!regE->bubble &&
        (regE->code_op == code_ADD ||
         regE->code_op == code_SUB ||
         regE->code_op == code_MOV)) // Cette condition permet de vérifier l'absence d'aléa et si l'instructuion manipulée est différente d'un LOAD ou d'un STORE car dans ces 2 cas on a pas besoin de Write Back la valeur
    {
        cpu->reg[regE->code_regDes] = regE->code_data; // WriteBack de la valeur dans le registre concerné
    }
    MemWriteBackReg *regM = (MemWriteBackReg *)malloc(sizeof(MemWriteBackReg)); // Création et allocation de la mémoire pour notre pointeur regM qui va référencer une structure MemWriteBackReg en mémoire
    if (!regM)                                                                  // Vérification de la réussite de l'allocation de la mémoire
    {
        printf("Allocation echouee");
        exit(1); // Dans le cas échéant alors sortir du programme
    }
    else
    {

        regM->value = regE->code_data;
        regM->code_regDes = regE->code_regDes;
        regM->bubble = regE->bubble;
        return regM; // Renvoie de l'objet configuré
    }
}