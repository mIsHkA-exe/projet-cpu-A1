// Ce ficheir est destiné à la troisième étape dans le cycle d'exéctuion d'une instruction : l'Execute
#include "init.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

ExecuteMemReg *Execute(CPU *cpu, DecodeExecuteReg *regD) // La méthode Execute prend en paramètre le CPU et l'objet de configuration crée dans l'étape précédente
{
    ExecuteMemReg *regE = (ExecuteMemReg *)malloc(sizeof(ExecuteMemReg)); // Création et allocation de la mémoire pour notre pointeur regE qui va référencer une structure ExecuteMemReg en mémoire
    if (!regE)                                                            // Vérification de la réussite de l'allocation de la mémoire
    {
        printf("Allocation echouee");
        exit(1); // Dans le cas échéant alors sortir du programme
    };
    // Récupération et initialisation des valeurs des propriétés de l'objet de configuration regD à des variables locales sur 16 bits
    uint16_t op = regD->code_op;         // Opération
    uint16_t regDes = regD->code_regDes; // Registre de destination
    uint16_t regSrc = regD->code_regSrc; // Registre Source
    uint16_t data = regD->code_data;     // Donnée sur 8 bits
    int mode = regD->mod;                // Mode d'opération
                                         // NB : en mode registre alors la data de l'instruction est considérée comme l'adresse d'un registre en mémoire c'est à dire on ne récupère que les 2 derniers bits sur les 8
                          // et en mode immédiat pas de souics la data reste sur 8 bits et les registre de destination et de registres source sur 2 bits
    uint16_t value = 0X00; // Conteneur de 16 bits de la valeur à calculer
    regE->bubble = false;  // Ici on initialise l'indicateur d'aléa à false car c'est dans le ficher .main que l'on va insérer la détection
    switch (op)            // Switch entre les potentielles opérations
    {
    case code_ADD:     // Addition
        if (mode == 0) // Mode registre
        {
            value = (cpu->reg[regSrc]) + (cpu->reg[data & 0X03]); // value = Reg_source + code_data (mais excluivement les 2 derniers bits)
        }
        else // Mode immédiat
        {
            value = (cpu->reg[regDes]) + data; // value = reg_destination + data
        }
        break;

    case code_SUB:
        if (mode == 0) // Mode registre
        {
            value = (cpu->reg[regSrc]) - (cpu->reg[data & 0X03]); // value = Reg_source - code_data (mais excluivement les 2 derniers bits)
        }
        else // Mode immédiat
        {
            value = (cpu->reg[regDes]) - data; // value = reg_destination - data
        }
        break;

    case code_MOV:
        if (mode == 0) // Mode registre
        {
            value = (cpu->reg[regSrc]);
        }
        else // Mode immédiat
        {
            value = data; // La valeur copie juste la data
        }
        break;

    case code_JMP:
        cpu->unit_command.PC = data; // L'IP du compteur de programme est directement initialisé à une autre position
        break;

//         Bits : [15  14  13  12]   [6   5   4   3   2   1   0]
// //             └──────┬───────┘   └───────────┬────────────┘
// //                   OPCODE                   DATA
// //                   (4 bits)               (7 bits)

    case code_LOAD:
        cpu->reg[regDes] = cpu->memoire[data]; // le contenu de la mémoire est chargée dans le registre concerné
        break;

    case code_STORE:
        cpu->memoire[data] = cpu->reg[regSrc]; // Le contenu du registre est stocké
        break;

    case code_HALT:
        cpu->unit_command.clock = false; // arrête la boucle while
        regE->bubble = true;             // aucune donnée à propager
        break;

    case code_NOP:
        regE->bubble = true; // ne fait rien, laisse passer
        break;
    };

    regE->code_op = op;
    regE->code_regDes = regDes;
    regE->code_regSrc = regSrc;
    regE->code_data = value;

    return regE; // Retourne la structure construite
}
//Bits : [0110]           [00]      [00]     [0000 0101]
//       └───┬────┘       └─┬─┘     └─┬─┘    └────┬──────┘
//             JMP       regDes   regSrc         DATA
//            (4 bits)     (2 bits) (2 bits)  (1 bit)           (7 bits)
// 