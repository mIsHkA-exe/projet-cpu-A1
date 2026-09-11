// Le fichier init ou initialisation est le fichier dans lequel l'on va initer tous les composants du CPU pour cette simulation
// Importation des bibliothèques nécessaires pour la manipulation des différentes structures
#ifndef INIT_H // Définir le fichier
#define INIT_H
// Bibliothèque nécessaire pour la manipulation des entrées sorties
#include <stdint.h>  // Bibliothèque nécessaire pour la manipulation des entiers avec des tailles exactes (16 bits dans notre cas)
#include <stdbool.h> // Bibliothèque nécessaire pour la gestion des booléennes en C
// Initialisation du jeu d'instruction donnée dans la consigne du projet
// Nous sommes passez par l'utilisation des codes bianires simple car il n'existe pas de code binaire universel pour tous les systèmes
#define code_NOP 0X00   // Correspondant à 0000
#define code_ADD 0X01   // Correspondant à 0001
#define code_SUB 0X02   // Correspondant à 0010
#define code_MOV 0X03   // Correspondant à 0011
#define code_STORE 0X04 // Correspondant à 0100
#define code_LOAD 0X05  // Correspondant à 0101
#define code_JMP 0X06   // Correspondant à 0110
#define code_HALT 0X0F  // Correspondant à 0111

// Initialisation des composants du CPU
// Les mémoires et registres
#define mem_size 32 // Taille réservée pour la mémoire
#define reg_size 4  // Taille réservée pour le banc de registre

// Unité de traitement du CPU où l'on va effectuer les opérations
typedef struct
{
    uint16_t accumulator; // Accumulateur situé dans l'unité de traitement pour stocker les opérandes et les resultats des opérations
    uint16_t state_reg;   // Registre d'état situé dans l'unité de traitement pour déterminer l'état de la sortie des données
} unitTreatment;

// Unité de commande du CPU où l'on va Fecther les instructions , séquencer les cycles et décoder les instructions
typedef struct
{
    uint16_t IR; // Registre d'instruction où l'on va stocker les instruction
    uint16_t PC; // Compteur de programme qui contient l'adresse de la prochaine instruction
    bool clock;  // Horloge qui permet de séquencer les cycles d'instructions
} unitCommand;

// Registre de transition entre la Fetch et le Decode
typedef struct
{
    uint16_t instruction; // Instruction Fecthée codée sur 16 bits
    uint16_t PC;          // Valuer actuelle du compteur de programme
    int index;            // Valeur du compteur de porgramme
} FetchDecodeReg;

// Registre de transition entre le Decode et l'Execute
typedef struct
{
    uint16_t code_op;     // Code de l'opération sur 4 bits (15 à 12)
    uint16_t code_regDes; // Code du registre de destination sur 2 bits (11 à 10)
    uint16_t code_regSrc; // Code du registre source sur 2 bits (9 à 8)
    uint16_t code_data;   // Code de la donnée à utilisée lors de l'opération sur 8 bits (7 à 0)
    int mod;              // indicateur du mod de l'instruction (mod = 1 alors mode immédiat  ou mod = 0 alors mode registre)
    int index;            // Valeur du compteur de programme
    bool bubble;          // Indicateur de la présence de potentiel aléas
} DecodeExecuteReg;

// Registre de transition entre l'Execute et le WriteBack
typedef struct
{
    uint16_t code_op;     // <--
    uint16_t code_regDes; //    |-------Indication idem que précédement
    uint16_t code_regSrc; //     |
    uint16_t code_data;   //       |
    bool bubble;          //           <--|
} ExecuteMemReg;

// Registre de WriteBack
typedef struct
{
    uint16_t value;       // resultat de l'opération à reécriree dans le registre concerné
    uint16_t code_regDes; // index du registre dans lequel on va reécrire la valeur
    bool bubble;          // Indicateur de présence de potentiel aléa
} MemWriteBackReg;

// Structure du CPU : c'est l'architecture entière de tout le système
typedef struct
{
    unitCommand unit_command;
    uint16_t memoire[mem_size]; // La mémoire a été assimilée à une array de 32 cases
    uint16_t reg[reg_size];     // Le banc de registre a été assimilé à une array de 4 cases
    unitTreatment unit_Treatment;
    FetchDecodeReg regF;
    DecodeExecuteReg regD;
    ExecuteMemReg regE;
    MemWriteBackReg regM;
    int cycle;        // Décompteur du nombres de cycle effecué
    int count_bubble; // Décompteur du nombre d'aléa rencontré
} CPU;

#endif
