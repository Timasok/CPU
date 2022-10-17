#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../include/funcs_CPU.h"

#define DEBUG_MODE

#ifdef DEBUG_MODE
#define DBG fprintf(stderr, "Compiled nicely -line: %d file: %s func: %s\n",            \
                                                __LINE__, __FILE__, __FUNCTION__)
//todo #define DBG_OUT:

#endif
#ifndef DEBUG_MODE
#define DBG
#endif

const int REG_POISON = -1;

extern FILE * log_file;

//todo stack in cpu and change stkPtr to cpuPtr everywhere
//todo add log_file in struct of cpu

#define DEF_CMD(name, num, arg, ...)                                             \
        case num:                                                                \
            cpu->ip++;                                                           \
            __VA_ARGS__;                                                         \
                                                                                 \
            if (!(num == CMD_DMP || num == CMD_HLT                               \
                                 || num == CMD_IN || num == CMD_JMP))            \
            {                                                                    \
                dump_CMD_CPU(cpu->ip - 1, cpu->code[cpu->ip - 1], cpu_stack, 1); \
            }else                                                                \
            {                                                                    \
                dump_CMD_CPU(cpu->ip - 1, cpu->code[cpu->ip - 1], cpu_stack, 0); \
            }                                                                    \
            break;                                                               \

#define FIRST_POP(stkPtr)                                                         \
            do {                                                                  \
                    if (stackPop(stkPtr, &first_popped) == EXIT_FAILURE)          \
                    {                                                             \
                        break;                                                    \
                    }                                                             \
                } while (0)

#define SECOND_POP(stkPtr)                                                        \
            do {                                                                  \
                    if (stackPop(stkPtr, &second_popped) == EXIT_FAILURE)         \
                    {                                                             \
                        break;                                                    \
                    }                                                             \
                } while (0)    

#define ARITHM(operation, stkPtr)                                                 \
            do {                                                                  \
                        stackPush(stkPtr, second_popped operation second_popped); \
                                                                                  \
                } while (0)

//todo replace stderr with cpu->logfile

#define SINGLE_POP(stkPtr, poppedPtr)                                             \
            do {                                                                  \
                    if (stackPop(stkPtr, poppedPtr) == EXIT_FAILURE)              \
                    {                                                             \
                        break;                                                    \
                    }                                                             \
                } while (0)

#define SINGLE_PUSH(stkPtr, element)                                              \
            do {                                                                  \
                    stackPush(stkPtr, element);                                   \
                } while (0)

#define OUT(cpuPtr, poppedPtr)                                                                 \
                    do {                                                         \
                        fprintf(log_file, "OUT: = %d", *poppedPtr);              \
                                                                                 \
                    } while (0)

#define ARITHM_DBG(operation)                                                       \ 
                fprintf(stderr, "%d %s %d\n", second_popped, #operation, first_popped)


#define JUMP(cpuPtr)                                                                \
        do {                                                                        \
                if (cpu->code[cpu->ip] < 0 || cpu->code[cpu->ip] >= cpu->quantity)  \
                    fprintf(stderr, "LABEL POINTS OUTDOORS ip = %d", cpu->ip);      \
                cpu->ip = cpu->code[cpu->ip++];                                     \
            } while (0)

#define COND_JUMP(cpuPtr, condition)                                            \
        do {                                                                    \
                if (second_popped condition first_popped){                      \                   
                    JUMP(cpuPtr);                                               \
                }else                                                           \
                {                                                               \
                    cpu->ip++;                                                  \
                }                                                               \
            } while (0)

#define EXIT()


int CPU_Ctor(CPU_info * cpu, FILE * asm_source)
{
    DBG;
    fread(&cpu->signature, sizeof(int), 1, asm_source);

    //*((char *)&cpu.signature + 2) = '\0';
    // fprintf(stderr, "%s\n", &cpu.signature);    
    //todo assert NULL PTR ...
    DBG;
    fread(&cpu->quantity, sizeof(int), 1, asm_source);    
    DBG;    
    fread(&cpu->number_of_comands, sizeof(int), 1, asm_source);
    DBG;
    cpu->code = (int *)calloc(cpu->quantity, sizeof(int));
    fread(cpu->code, sizeof(int), cpu->quantity, asm_source);

    return EXIT_SUCCESS;
}

int process(CPU_info * cpu, Stack * cpu_stack)
{   

    cpu->ip = 0;
    stackCtor(* cpu_stack, cpu->number_of_comands);

    while (cpu->ip < cpu->quantity)
    {
        int num_of_comand = cpu->code[cpu->ip];
        elem_t first_popped = 0, second_popped = 0;
        elem_t out = 0;

        fprintf(stderr,"NUMBER OF PROCESSING COMAND %d ip - %d \n", num_of_comand, cpu->ip);
        switch(num_of_comand)
        {
            #include "../../inc/comands.h"

            default:
                if((num_of_comand & CMD_PUSH) || (num_of_comand & CMD_POP))
                {                
                    //Here code will break if we change typedef RAM[double] for example                                         
                    elem_t arg = 0;
                    int reg_idx = REG_POISON;

                    if (num_of_comand & IMMED_MASK)
                    {   
                        cpu->ip++;
                        arg = cpu->code[cpu->ip++];     
                        
                    }else if(num_of_comand & REG_MASK)
                    {
                        cpu->ip++;
                        int reg_idx = cpu->code[cpu->ip++];
                        arg = cpu->Reg[reg_idx];

                    }else{
                        cpu->ip++;
                        fprintf(stderr, "Error: WRONG CMD MASK: -line: %d file: %s func: %s\n",  __LINE__, __FILE__, __FUNCTION__);
                        dump_CMD_CPU(cpu->ip - 1, num_of_comand, cpu_stack, 0);
                        continue;
                    }

                    if(num_of_comand & MEM_MASK)
                    {
                        if (num_of_comand & CMD_PUSH)
                        {
                            //func for pushing from Ram

                            if (cpu->RAM[arg] != NULL)
                            {
                                stackPush(cpu_stack, cpu->RAM[arg]);
                                fprintf(stderr, "push [%d] \n", arg);  

                            }else
                            {
                                fprintf(stderr, "RAM[%d] is empty\n", arg);
                                dump_CMD_CPU(cpu->ip - 1, num_of_comand, cpu_stack, 0);
                                break;

                            }

                        }else if (num_of_comand & CMD_POP)
                        {
                            stackPop(cpu_stack, &cpu->RAM[arg]);
                            fprintf(stderr, "pop [%d]\n", arg);

                        }

                        dump_CMD_CPU(cpu->ip - 1, num_of_comand, cpu_stack, 1);

                    }else
                    {
                        if (num_of_comand & CMD_PUSH)
                        {
                            stackPush(cpu_stack, arg);
                            fprintf(stderr, "push value %d\n", arg);
                            dump_CMD_CPU(cpu->ip - 1, num_of_comand, cpu_stack, 1);

                        }else if (num_of_comand & CMD_POP)
                        {
                            if (reg_idx != REG_POISON)
                            {
                                stackPop(cpu_stack, &cpu->Reg[reg_idx]);
                                fprintf(stderr, "pop to REG[%d]\n", reg_idx);
                                dump_CMD_CPU(cpu->ip - 1, num_of_comand, cpu_stack, 1);
                            }

                            fprintf(stderr, "Error: CANNOT POP into IMMED  -line: %d file: %s func: %s\n",  __LINE__, __FILE__, __FUNCTION__);
                            break;

                        }

                    }
                                                                        
                }
                break;
        }
         
    }

    return EXIT_SUCCESS;
}


int dump_CPU(CPU_info *cpu, FILE * log_file)
{
    fprintf(log_file, "\nSIGNATURE: %s\n"
                        "QUANTITY:  %d\n"
                        "NUMBER OF COMANDS: %d\n"
                      ,&(cpu->signature), cpu->quantity, cpu->number_of_comands);     

    for (int ip = 0; ip < cpu->quantity; ip++)
        fprintf(log_file, "%d\t", cpu->code[ip]);        

    fprintf(log_file, "\n");

}

int dump_CMD_CPU(int ip, int CMD, Stack * cpu_stack, bool stackChanged)
{
    fprintf(stderr, "\nComand - ");
    printBits(CMD);
    fprintf(stderr, "\tip - %d\n", ip);
    
    if (stackChanged)
    {
        fprintf(stderr, "STACK CHANGED!\n");
        printStack(cpu_stack);
    }
    
}

int printBits(int x)
{
    do{
        fprintf(stderr, "%d", x%2);
        x /= 2;

    }while(x);

    return EXIT_SUCCESS;
}


#undef SINGLE_POP
#undef JUMP
#undef COND_JUMP
#undef FIRST_POP
#undef SECOND_POP
#undef ARITHM
#undef ARITHM_DBG
#undef DEF_CMD
#undef DBG