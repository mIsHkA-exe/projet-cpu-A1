//Ce fichier permet de mettre les prototypes de toutes les méthodes de chaque fichier afin de les utiliser n'importe où
#ifndef PROTOTYPE_H
#define PROTOTYPE_H
#include "init.h"
void Display_cpu_status(CPU *cpu);
void init_cpu(CPU *cpu);
void init_mem(CPU *cpu);
FetchDecodeReg *fetch(CPU *cpu);
DecodeExecuteReg *Decode(FetchDecodeReg *regF, CPU *cpu);
ExecuteMemReg *Execute(CPU *cpu, DecodeExecuteReg *regD);
MemWriteBackReg *MemWriteBack(ExecuteMemReg *regE, CPU *cpu);
bool detectDataHazard(CPU *cpu);
void handleDataHazard(CPU *cpu);
bool detectStructuralHazard(CPU *cpu);
void handleStructuralHazard(CPU *cpu);
int pipeline();
#endif