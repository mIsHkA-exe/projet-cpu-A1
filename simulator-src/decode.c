// Ce ficher est destiné à la deuxième étape dans le cycle d'exécution d'une instruction : Decode
#include "init.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

DecodeExecuteReg *Decode(FetchDecodeReg *regF, CPU *cpu) // La méthode Decode va prendre en paramètre directement tout le CPU et l'objet regF restitué par la fonction fecth précédente
{
    DecodeExecuteReg *regD = (DecodeExecuteReg *)malloc(sizeof(DecodeExecuteReg)); // Création et allocation de la méémoire pour notre pointeur regD qui va référencer une structure DecodeExecuteReg en mémoire
    if (!regD)                                                                     // Vérification de la réussite de l'allocation de la mémoire
    {
        printf("Allocatiuon echouee");
        exit(1); // Dans le cas échéant alors sortir du programme
    };

    uint16_t instruction = regF->instruction;         // Initialisation de l'instruction sur une varaible de 16 bits
    regD->code_op = ((instruction >> 12) & 0X0F);     // Décalage à droite de 12 positions pour conserver les 4 bits concernés et utilisation d'un masque binaire de sécurité 0X0F avec une porte AND pour conserver les bits concernés
    regD->code_regDes = ((instruction >> 10) & 0X03); // Décalage à droite de 10 positions pour conserver les 2 bits concernés et utilisation d'un masque binaire de sécurité 0X03 avec une porte AND pour conserver les bits concernés
    regD->code_regSrc = ((instruction >> 8) & 0X03);  // Décalage à droite de 8 positions pour conserver les 2 bits concernés et utilisation d'un masque binaire de sécurité 0X03 avec une porte AND pour conserver les bits concernés
    regD->mod = ((instruction >> 7) & 0X01);          // Afin de déterminier si nous sommes en mode immédiat ou en mode registre (0 = mode registre et 1 = mode immédiat)
    regD->code_data = (instruction & 0X7F);           // Décalage à droite de 12 positions pour conserver les 4 bits concernés et utilisation d'un masque binaire de sécurité 0X07 avec une porte AND pour conserver les bits concernés
    regD->index = regF->index;                        // Récupération de l'index du PC
    regD->bubble = false;                             // Ici on initialise l'indicateur d'aléa à false car c'est dans le ficher .main que l'on va insérer la détection
    return regD;                                      // Retourne l'objet construit
}
//Bits : [15  14  13  12] [11  10] [9   8]  [  7  ]  [6   5   4   3   2   1   0]
//       └──────┬───────┘ └───┬──┘ └──┬──┘  └──┬──┘  └───────────┬────────────┘
//             OPCODE       regDes   regSrc     MOD               DATA
//            (4 bits)     (2 bits) (2 bits)  (1 bit)           (7 bits)
// 
// 0000 00 00 0011
// AND 
// 0000 00 00 1111
// _______________
// 0000 00 00 0011
