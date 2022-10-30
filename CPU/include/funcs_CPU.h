#ifndef FUNCS_CPU_H
#define FUNCS_CPU_H

#include "stack_funcs.h"

const int MAX_STACK_SIZE = 1024;
const int REG_CAPACITY   = 5;
const int RAM_CAPACITY   = 4096;

#define DEF_CMD(name, num, arg, ...) \
            CMD_##name = num,


struct CPU_info
{
    // int labels[LABELS_SIZE] = {};
    // int version;
    FILE * log_file;
    Stack stack;
    Stack funcs_stack;
    unsigned int code_of_error;
    int signature;
    int number_of_comands;
    int ip;
    int quantity;
    elem_t * code;
    elem_t Reg[REG_CAPACITY] = {};
    elem_t RAM[RAM_CAPACITY] = {};
    //todo add sleep to underline it's slowness mthf

};

enum CMD
{
    #include "comands.h"
};

enum MASKS
{
    IMMED_MASK = 1 << 5,
    REG_MASK   = 1 << 6,
    MEM_MASK   = 1 << 7,

};

#undef DEF_CMD
bool checkSignature(CPU_info * cpu, FILE * asm_source);
int CPU_Ctor(CPU_info * cpu, FILE * asm_source);
int operateWithRam(CPU_info *cpu, elem_t tmp_arg, elem_t *arg);
int operateArgs(CPU_info *cpu, elem_t *arg);
int process(CPU_info * cpu);
int CPU_Dtor(CPU_info * cpu);

#endif