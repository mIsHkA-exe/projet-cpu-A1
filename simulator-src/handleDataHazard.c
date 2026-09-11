// Ce fichier est réservée à la détection et la gestion des aléa de donnée
// Cet aléa se produit lorsque on écrit avec une un registre de valeur obselète
// Dans notre architecture elle est détectée lorsque la méthode Decode obtient un registre source à l'instant n+1 qui est registre de destination dans l'instruction Execute précedente à l'instant n en cours
// n : instant précédent et n+1 : instant courant
#include "init.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

bool detectDataHazard(CPU *cpu)
{

    if (!cpu->regE.bubble &&
        (cpu->regE.code_op == code_ADD ||
         cpu->regE.code_op == code_SUB ||
         cpu->regE.code_op == code_MOV ||
         cpu->regE.code_op == code_LOAD))
    {

        uint16_t dest = cpu->regE.code_regDes; // Macro pour la destination car elle ne change pratiquement pas dans tout le fichier

        if (cpu->regD.mod == 0) // Mode regitre NB : en mode regitre on peut avoir 1 à 2 registre source (celui de base et la (data & 0X03))
        {
            if (cpu->regD.code_regSrc == dest) // SI le code du registre source à l'instant n+1 = code du registre desination de l'instruction précédente à l'instant n alors aléa trouvé (true)
            {
                return true;
            }
            if ((cpu->regD.code_data & 0x03) == dest) // Comme expliqué précedeement alors dans le cas du mode registre alors le code de
                                                      // la data correspond au code du registre source2 mais avec un masque de sécurité et donc comme expliqué dans le cas précédent il y a possibilité d'un aléa
            {
                return true;
            }
        }
        else // Mode immédiat : C'est directement la destination qui est utilsée (ex : successivement   MOV R1,4   et  MOV R1,5)
        {
            if (cpu->regD.code_regDes == dest)
            {
                return true;
            }
        }
    }

    if (!cpu->regM.bubble &&
        (cpu->regM.code_regDes != 0)) // Dans le cas où l'aléa est entre le Decode et le MenWriteBack
    {
        // Les explications sont les mêmes que dans le cas précédent (entre Decode et Execute)
        uint16_t dest = cpu->regM.code_regDes;

        if (cpu->regD.mod == 0) // Mode registre
        {
            if (cpu->regD.code_regSrc == dest)
            {
                return true;
            }
            if ((cpu->regD.code_data & 0x03) == dest)
            {
                return true;
            }
        }
        else // Mode immédiat
        {
            if (cpu->regD.code_regDes == dest)
            {
                return true;
            }
        }
    }

    return false; // Dans le cas où il n'y a pas d'aléa détecté alors la fonction retourne false
}

void handleDataHazard(CPU *cpu) // Cette méthode nous permet de gérer l'aléa de donnée sans perte de performance
{
    if (cpu->regD.mod == 0) // Mode registre
    {
        if (cpu->regD.code_regSrc == cpu->regE.code_regDes) // Cas 1 : registre source1 de Decode à n+1 = registre destination de Execute à n
        {
            cpu->reg[cpu->regD.code_regSrc] = cpu->regE.code_data; // on remplie directement ce registre source avec la sortie de l'Execute du registre de destination correpondant
        }
        else if (!cpu->regM.bubble && cpu->regD.code_regSrc == cpu->regM.code_regDes) // Cas 2 : registre source1 de Decode à n+1 = registre destination de MemWriteBack à n
        {
            cpu->reg[cpu->regD.code_regSrc] = cpu->regM.value; // on remplie directement ce registre source avec la sortie de la memWriteBack du registre de destination correpondant
        }
        if ((cpu->regD.code_data & 0x03) == cpu->regE.code_regDes) // Cas 1.2 :  registre source2 (code_data & 0X03) de Decode à n+1 = registre destination de Execute à n
        {
            cpu->reg[(cpu->regD.code_data & 0x03)] = cpu->regE.code_data; // on remplie directement ce registre source2 avec la sortie de l'Execute du registre de destination correpondant
        }
        else if (!cpu->regM.bubble && (cpu->regD.code_data & 0x03) == cpu->regM.code_regDes) // Cas 2.2 : registre source2 de Decode à n+1 = registre destination de MemWriteBack à n
        {
            cpu->reg[(cpu->regD.code_data & 0x03)] = cpu->regM.value; // on remplie directement ce registre2 source avec la sortie de la memWriteBack du registre de destination correpondant
        }
    }
    else // Mode immédiat
    {
        if (cpu->regD.code_regDes == cpu->regE.code_regDes) // Cas 1 : Le registre de destination de Decode à n+1 = registre destination de Execute à n
        {
            cpu->reg[cpu->regD.code_regDes] = cpu->regE.code_data; // on remplie directement ce registre source avec la sortie de l'Execute du registre de destination correpondant
        }
        else if (!cpu->regM.bubble && cpu->regD.code_regDes == cpu->regM.code_regDes) // Cas 2 : registre destination de Decode à n+1 = registre destination de MemWriteBack à n
        {
            cpu->reg[cpu->regD.code_regDes] = cpu->regM.value; // on remplie directement ce registre source avec la sortie de la memWriteBack du registre de destination correpondant
        }
    }
}