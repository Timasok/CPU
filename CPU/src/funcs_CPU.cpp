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

// #define DEBUG_MODE

#ifdef DEBUG_MODE
#define DBG_OUT fprintf(stderr, "Compiled nicely -line: %d file: %s func: %s\n",            \
                                                __LINE__, __FILE__, __FUNCTION__)

#define PARSE_ERROR(cpuPtr, condition, error_specifier)                                        \
        cpuPtr->code_of_error  |= ((condition) & error_specifier);

#endif
#ifndef DEBUG_MODE
#define DBG_OUT
#endif

#define PRINT_ERR(...)                                                  \
        do {                                                            \
            fprintf(stderr,"\e[0;32mERROR: " );                         \
            fprintf(stderr, __VA_ARGS__);                               \
            fprintf(stderr,"\e[0m" );                                   \ 
        } while(0)

const int REG_POISON = -1;

#define FIRST_POP(cpuPtr)                                                         \
            do {                                                                  \
                    if (stackPop(&cpuPtr->stack, &first_popped) == EXIT_FAILURE)  \
                    {                                                             \
                        break;                                                    \
                    }                                                             \
                } while (0)

#define SECOND_POP(cpuPtr)                                                        \
            do {                                                                  \
                    if (stackPop(&cpuPtr->stack, &second_popped) == EXIT_FAILURE) \
                    {                                                             \
                        break;                                                    \
                    }                                                             \
                } while (0)    

#define ARITHM(operation, cpuPtr)                                                 \
            do {                                                                  \
                    stackPush(&cpuPtr->stack, second_popped operation second_popped); \
                                                                                  \
                } while (0)

#define SINGLE_POP(cpuPtr, poppedPtr)                                             \
            do {                                                                  \
                    if (stackPop(&cpuPtr->stack, poppedPtr) == EXIT_FAILURE)      \
                    {                                                             \
                        break;                                                    \
                    }                                                             \
                } while (0)

#define SINGLE_PUSH(cpuPtr, element)                                              \
            do {                                                                  \
                    stackPush(&cpuPtr->stack, element);                           \
                } while (0)

#define OUT(cpuPtr, element)                                                       \
                    do {                                                           \
                        fprintf(cpuPtr->log_file, "OUT: = %d", element);            \
                        printf("\e[0;32m\nOUT: = %d\e[0m\n", element);              \
                    } while (0)

    

#define ARITHM_DBG(operation)                                                       \ 
                fprintf(cpu->log_file, "%d %s %d\n", second_popped, #operation, first_popped)


#define JUMP(cpuPtr)                                                                                      \
        do {                                                                                              \
                if (cpuPtr->code[cpuPtr->ip] < 0 || cpuPtr->code[cpuPtr->ip] >= cpuPtr->quantity)         \
                    PRINT_ERR("LABEL POINTS OUTDOORS ip = %d", cpuPtr->ip);                               \
                cpuPtr->ip = cpuPtr->code[cpuPtr->ip];                                                    \
                cpuPtr->ip++;                                                                             \
            } while (0)

#define COND_JUMP(cpuPtr, condition)                                                \
        do {                                                                        \
                if (second_popped condition first_popped)                           \ 
                {                                                                   \
                    JUMP(cpuPtr);                                                   \
                    PRINT_ERR("Condition succeded\n");                              \
                } else {                                                            \                                                            
                    cpuPtr->ip++;                                                   \
                    PRINT_ERR("Condition failed\n");                                \
                }                                                                   \
            } while (0)

#define CALL(cpuPtr)                                                                \
        do {                                                                        \
                if (cpu->code[cpu->ip] < 0 || cpu->code[cpu->ip] >= cpu->quantity)  \
                {                                                                   \
                    break;                                                          \
                }                                                                   \
                stackPush(&cpuPtr->funcs_stack, cpu->ip);                           \ 
                cpu->ip = cpu->code[cpu->ip];                                       \
                /*fprintf(stderr, "CALL to ip = %d\n", cpu->ip); */                     \
                cpu->ip++;                                                          \
            } while (0)              

//add assert
#define RET(cpuPtr)                                                                 \
        do {                                                                        \
                elem_t ret_position;                                                \
                stackPop(&cpuPtr->funcs_stack, &ret_position);                      \
                /*fprintf(stderr, "RET to ip = %d\n", ret_position);*/                  \
                cpu->ip = ++ret_position;                                             \
                                                                                    \
            } while (0)   


bool checkSignature(CPU_info * cpu, FILE * asm_source)
{
    assert(cpu != nullptr);
    assert(asm_source != nullptr);
    fread(&cpu->signature, sizeof(int), 1, asm_source); 

    int correctSignature = 'C' + 'P'*256;

    bool matchSignature = (correctSignature == cpu->signature);

    if (matchSignature == false)
        PRINT_ERR("NOT CORRECT SIGNATURE = %s\n", &(cpu->signature));

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
    stackCtor(cpu->funcs_stack, MAX_NUM_OF_FUNCS);

    cpu->ip = 0;
    cpu->log_file = fopen("cpu_log.txt", "w+");
    fflush(cpu->log_file);

    return EXIT_SUCCESS;
}

#define DEF_CMD(name, num, arg, asmcode, cpucode)                            \
    case num:                                                                \
            cpucode                                                          \
            break;


int operateArgs(CPU_info *cpu, elem_t *argPtr)
{
    elem_t tmp_arg = 0;
    int reg_idx = REG_POISON;
    bool detectedPopToImmed = false;
    int num_of_comand = cpu->code[cpu->ip - 1];

    switch(num_of_comand & 0x1F)
    {

        case (CMD_PUSH & 0x1F):
            {
                if (num_of_comand & IMMED_MASK)
                {   
                    tmp_arg = cpu->code[cpu->ip++];     
                    
                }
                else if(num_of_comand & REG_MASK)
                {
                    int reg_idx = cpu->code[cpu->ip++];
                    tmp_arg = cpu->Reg[reg_idx];

                }else {

                    PRINT_ERR("WRONG PUSH MASK: -line: %d file: %s func: %s\n",  __LINE__, __FILE__, __FUNCTION__);

                    cpu->code_of_error |= CPU_ERROR_INCORRECT_PUSH_MASK;
                    return CPU_ERROR_INCORRECT_PUSH_MASK;
                }

                if(num_of_comand & MEM_MASK)
                {
                    pushToRam(cpu, tmp_arg);
                    return EXIT_SUCCESS;

                }else{

                    *argPtr = tmp_arg;

                }
                break;
            }    
        case (CMD_POP & 0x1F):
            {
                if(num_of_comand & REG_MASK)
                {
                    int reg_idx = cpu->code[cpu->ip++];
                    tmp_arg = cpu->Reg[reg_idx];

                }else if (num_of_comand & IMMED_MASK)
                {   
                    tmp_arg = cpu->code[cpu->ip++];
                    detectedPopToImmed = true;
                    
                }else {

                    PRINT_ERR("WRONG POP MASK: -line: %d file: %s func: %s\n",  __LINE__, __FILE__, __FUNCTION__);

                    cpu->code_of_error |= CPU_ERROR_INCORRECT_POP_MASK;
                    return CPU_ERROR_INCORRECT_POP_MASK;
                }


                if(num_of_comand & MEM_MASK)
                {
                    detectedPopToImmed = false;
                    popFromRam(cpu, tmp_arg);
                    return EXIT_SUCCESS;


                }else{

                    *argPtr = tmp_arg;

                }

                if (detectedPopToImmed == true)
                {
                    PRINT_ERR("CANNOT POP into IMMED  -line: %d file: %s func: %s\n",  __LINE__, __FILE__, __FUNCTION__);
                    cpu->code_of_error |= CPU_ERROR_ATTEMPT_TO_POP_INTO_IMMED;

                }
                break; 
            }

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

int popFromRam(CPU_info *cpu, elem_t tmp_arg)
{
    fprintf(stderr, "*****************************************************************************\n");

    sleep(0.5);

    if (cpu->RAM[tmp_arg] != NULL)
    {
        stackPush(&cpu->stack, cpu->RAM[tmp_arg]);
        return EXIT_SUCCESS;

    } else
    {
        PRINT_ERR("RAM[%d] is empty\n", tmp_arg);
        cpu->code_of_error |= CPU_ERROR_MASHING_RAM;
        return CPU_ERROR_MASHING_RAM;

    }

}

int pushToRam(CPU_info *cpu, elem_t tmp_arg)
{
    fprintf(stderr, "*****************************************************************************\n");

    sleep(0.5);

    if (cpu->RAM[tmp_arg] != NULL)
    {
        PRINT_ERR("RAM[%d] is already filled\n", tmp_arg);
        cpu->code_of_error |= CPU_ERROR_ACCESSING_TO_EMPTY_RAM;
        return CPU_ERROR_ACCESSING_TO_EMPTY_RAM;

    } else
    {
        stackPop(&cpu->stack, &cpu->RAM[tmp_arg]);
        return EXIT_SUCCESS;

    }

}

#undef DEF_CMD

#define DEF_CMD(name, num, arg, asmcode, cpucode)                                    \
        case num:                                                                \
            cpu->ip++;                                                           \
                                                                                 \
            if(num == CMD_PUSH || num == CMD_POP)                                \
            {                                                                    \
                operateArgs(cpu, &actual_arg);                                   \
                                                                                 \
            }else{                                                               \
                cpucode                                                          \
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
        elem_t actual_arg;

        // fprintf(stderr,"NUMBER OF PROCESSING COMAND %d ip - %d \n", num_of_comand, cpu->ip);

        switch(num_of_comand & 0x1F)
        {
            #include "comands.h"

            // default:
            //     cpu->ip++;

        }           

    }

    return EXIT_SUCCESS;
}

#undef DEF_CMD

int CPU_Dtor(CPU_info * cpu)
{
    fclose(cpu->log_file);
    free(cpu->code);
    return EXIT_SUCCESS;
}

#include "undefines_CPU.h"
#undef PARSE_ERROR
#undef DBG_OUT