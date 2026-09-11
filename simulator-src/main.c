// Ce fichier est réservé à la structuration des méthodes dans les différents fichiers afin d'implémenter la simulation
// Une simulation séquentielle et pipelinée ont été réalisée pour comparer les performances
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "init.h"
#include "prototype.h"

int main()
{ // Exécuttion séquentielle secondaire car notre simulation est principalement pipelinée
    // CPU cpu;
    // init_cpu(&cpu);
    // init_mem(&cpu);
    // Display_cpu_status(&cpu);
    // printf("\n");

    // while (cpu.unit_command.clock)
    // {
    //     FetchDecodeReg *regF = fetch(&cpu);

    //     DecodeExecuteReg *regD = Decode(regF, &cpu);

    //     ExecuteMemReg *regE = Execute(&cpu, regD);

    //     MemWriteBackReg *regM = MemWriteBack(regE, &cpu);

    //     Display_cpu_status(&cpu);
    //     printf("\n");
    //     cpu.cycle += 1;
    //     free(regE);
    //     free(regD);
    //     free(regF);
    // }
    // return 0;
    // Exécution pipelinée
    return pipeline();
}