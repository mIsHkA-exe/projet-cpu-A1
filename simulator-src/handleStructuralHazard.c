// Ce ficher est destiné à la détection et la gestion de l'aléa de structure
// Cet aléa occure lorsque on essaye de lire et écrire dans la mémoire simultané 
// Pour l'a détecter alors on vérifie si on a une instruction LOAD dans l'Execute à n et une instruction STORE dans le Decode  à n+1
#include "init.h"
#include "prototype.h"
#include <stdio.h>
#include <stdint.h>

bool detectStructuralHazard(CPU *cpu)
{ // Cette méthode prend en paramètre un pointeur vers la structure CPU et permet de détecter la présence d'aléa

    if (!cpu->regE.bubble &&
        cpu->regE.code_op == code_LOAD &&
        !cpu->regD.bubble &&
        cpu->regD.code_op == code_STORE) // Vérification de que l'opération est un LOAD dans le Execute et un STORE dans le Decode
    {
        return true; //Si vraie alors aléa trouvé 
    }
    return false; // Si faux alors absence d'aléa
}

void handleStructuralHazard(CPU *cpu) // Cette méthode permet la gestion de l'aléa de structure en prenant en paramètre un pointeur vers la structure CPU
{
    cpu->regD.bubble = true;
    cpu->regD.code_op = code_NOP; // On la résouds en insérant une instruction afin que rien ne se passe 
    cpu->unit_command.PC -= 1;
    cpu->count_bubble += 1;
}