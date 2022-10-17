#include "../stack/include/stack_funcs.h"

const int MAX_STACK_SIZE = 1024;

#define DEF_CMD(name, num, arg, ...) \
            CMD_##name = num,


struct CPU_info
{
    // int labels[LABELS_SIZE] = {};
    // const char* signature;
    // int version;
    int signature;
    int number_of_comands;
    int ip;
    int quantity;
    elem_t * code;
    elem_t Reg[5]    = {};
    elem_t RAM[4096] = {};//todo add sleep to underline it's slowness mthf

};

enum CMD
{
    #include "../../inc/comands.h"
};

enum MASKS
{
    IMMED_MASK = 0x20,
    REG_MASK   = 0x40,
    MEM_MASK   = 0x80,

};

enum CPU_Errors
{
    
};


#undef DEF_CMD

int printBits(int x);
int dump_CMD_CPU(int ip, int CMD, Stack * cpu_stack, bool stackChanged);
int CPU_Ctor(CPU_info * cpu, FILE * asm_source);
int dump_CPU(CPU_info *cpu, FILE * log_file);
int process(CPU_info * cpu, Stack * cpu_stack);