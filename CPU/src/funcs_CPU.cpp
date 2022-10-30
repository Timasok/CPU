#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "funcs_CPU.h"
#include "debug_CPU.h"
#include "stack_funcs.h"

#define DEBUG_MODE

#ifdef DEBUG_MODE
#define DBG_OUT fprintf(stderr, "Compiled nicely -line: %d file: %s func: %s\n",            \
                                                __LINE__, __FILE__, __FUNCTION__)

#endif
#ifndef DEBUG_MODE
#define DBG_OUT
#endif

const int REG_POISON = -1;

#include "defines_CPU.h"

bool checkSignature(CPU_info * cpu, FILE * asm_source)
{
    assert(cpu != nullptr);
    assert(asm_source != nullptr);
    fread(&cpu->signature, sizeof(int), 1, asm_source); 

    fprintf(stderr, "SIGNATURE = %s\n", &(cpu->signature));

    int correctSignature = 'C' + 'P'*256;

    bool matchSignature = (correctSignature == cpu->signature);
    
    return matchSignature;

}

int CPU_Ctor(CPU_info * cpu, FILE * asm_source)
{
    DBG_OUT;

    cpu->code_of_error = 0;

    if (checkSignature(cpu, asm_source) == false)
    {   
        cpu->code_of_error |= CPU_ERROR_INCORRECT_SIGNATURE;
        return EXIT_FAILURE;
    } 

    DBG_OUT;
    fread(&cpu->quantity, sizeof(int), 1, asm_source);    
    DBG_OUT;    
    fread(&cpu->number_of_comands, sizeof(int), 1, asm_source);
    DBG_OUT;
    cpu->code = (int *)calloc(cpu->quantity, sizeof(int));
    fread(cpu->code, sizeof(int), cpu->quantity, asm_source);

    stackCtor(cpu->stack, cpu->number_of_comands);
    cpu->ip = 0;
    cpu->log_file = fopen("cpu_log.txt", "w+");
    setvbuf(cpu->log_file, NULL, _IONBF, 0);

    return EXIT_SUCCESS;
}

#define DEF_CMD(name, num, arg, asmcode, ...)                                \
    case num:                                                                \
            __VA_ARGS__                                                      \
            break;


int operateArgs(CPU_info *cpu, elem_t *argPtr)
{
    elem_t tmp_arg = 0;
    int reg_idx = REG_POISON;
    bool detectedPopToImmed = false;
    int num_of_comand = cpu->code[cpu->ip - 1];

    switch(num_of_comand & 0x1F)
    {

        #include "comands.h"

        default: 
            printf("Go fuck yourself with such args");

    }
    if (num_of_comand & CMD_PUSH)
    {
        stackPush(&cpu->stack, *argPtr);
        //add dump                        
                                        
    } else if (num_of_comand & CMD_POP)
    {
        stackPop(&cpu->stack, argPtr);
        //add dump
    
    }

    return EXIT_SUCCESS;

}

int operateWithRam(CPU_info *cpu, elem_t tmp_arg, elem_t *argPtr)
{
    fprintf(stderr, "*****************************************************************************\n");

    sleep(0.5);

    if (cpu->RAM[tmp_arg] != NULL)
    {
        *argPtr = cpu->RAM[tmp_arg];
        return EXIT_SUCCESS;

    } else
    {
        fprintf(stderr, "RAM[%d] is empty\n", tmp_arg);
        cpu->code_of_error |= CPU_ERROR_ACCESSING_TO_EMPTY_RAM;
        return CPU_ERROR_ACCESSING_TO_EMPTY_RAM;

    }

}

#undef DEF_CMD

#define DEF_CMD(name, num, arg, asmcode, ...)                                    \
        case num:                                                                \
            cpu->ip++;                                                           \
                                                                                 \
            if(num == CMD_PUSH || num == CMD_POP)                                \
            {                                                                    \
                operateArgs(cpu, &actual_arg);                                   \
                                                                                 \
            }else{                                                               \
                __VA_ARGS__                                                      \
            }                                                                    \
                                                                                 \
            if (!(num == CMD_DMP || num == CMD_HLT                               \
                                 || num == CMD_IN || num == CMD_JMP))            \
            {                                                                    \
                dump_CMD_CPU(cpu->ip - 1, cpu->code[cpu->ip - 1], cpu, 1);       \
            }else                                                                \
            {                                                                    \
                dump_CMD_CPU(cpu->ip - 1, cpu->code[cpu->ip - 1], cpu, 0);       \
            }                                                                    \
            break;                                                               \


int process(CPU_info * cpu)
{   

    while (cpu->ip < cpu->quantity)
    {
        int num_of_comand = cpu->code[cpu->ip];
        elem_t first_popped = 0, second_popped = 0;
        elem_t out = 0;
        elem_t actual_arg;

        fprintf(stderr,"NUMBER OF PROCESSING COMAND %d ip - %d \n", num_of_comand, cpu->ip);

        switch(num_of_comand & 0x1F)
        {
            #include "comands.h"

        }

    }

    return EXIT_SUCCESS;
}

#undef DEF_CMD

int CPU_Dtor(CPU_info * cpu)
{
    free(cpu->code);
    return EXIT_SUCCESS;
}

#include "undefines_CPU.h"
#undef DBG_OUT