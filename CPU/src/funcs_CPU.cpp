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

#define DEF_CMD(name, num, arg, ...)                                               \ 
        case num:                                                                  \
        __VA_ARGS__;                                                               \
        break;

#define FIRST_POP(stkPtr)                                                         \
                    do {                                                          \
                        stackPop(stkPtr, &first_popped);                          \
                    } while (0)                                                          


#define SECOND_POP(stkPtr)                                                        \
                    do {                                                          \
                        stackPop(stkPtr, &second_popped);                         \
                    } while (0)   

#define ADD(stkPtr)                                                               \
                    do {                                                          \
                        stackPush(stkPtr, first_popped + second_popped);          \
                                                                                  \
                    } while (0)

#define ARITHM_DBG()                                                              \ 
                fprintf(stderr, "add %d %d\n", first_popped, second_popped)
                
                
int CPU_Ctor(CPU_info * cpu, FILE * asm_source)
{

    DBG;
    fread(&cpu->signature, sizeof(int), 1, asm_source);

    //*((char *)&cpu.signature + 2) = '\0';
    // fprintf(stderr, "%s\n", &cpu.signature);    
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
        elem_t first_popped = 0, second_popped = 0;
        fprintf(stderr,"%d\n", cpu->code[cpu->ip]);
        switch(cpu->code[cpu->ip++])
        {
            case (CMD_PUSH | IMMED_MASK):
            {
                stackPush(cpu_stack, cpu->code[cpu->ip++]);

                fprintf(stderr, "push %d\n", cpu->code[cpu->ip - 1]);

                dump_CMD_CPU(cpu->ip - 1, cpu->code[cpu->ip - 1], cpu_stack, 1);
                break;
            }
            case CMD_ADD:
            {

                FIRST_POP(cpu_stack);
                SECOND_POP(cpu_stack);
                ADD(cpu_stack);
                ARITHM_DBG();
            //     elem_t first_popped = 0, second_popped = 0;
                
            //     stackPop(cpu_stack, &first_popped);
            //     stackPop(cpu_stack, &second_popped);

            //     fprintf(stderr, "add %d %d\n",first_popped, second_popped);
                
            //     stackPush(cpu_stack, first_popped + second_popped);

                dump_CMD_CPU(cpu->ip - 1, cpu->code[cpu->ip - 1], cpu_stack, 1);
                break;
            }
            case CMD_OUT:
            {
                elem_t out = 0;

                stackPop(cpu_stack, &out);

                fprintf(stderr, "%d\n", out);
                dump_CMD_CPU(cpu->ip - 1, cpu->code[cpu->ip - 1], cpu_stack, 1);                    
                break;
            }
            case CMD_HLT:
            {
                dump_CMD_CPU(cpu->ip - 1, cpu->code[cpu->ip - 1], cpu_stack, 0);
                stackDtor(cpu_stack);
                break;
            }
            

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
    fprintf(stderr, "\nComand - %d ip - %d\n", CMD, ip);
    
    if (stackChanged)
    {
        fprintf(stderr, "STACK CHANGED!\n");
        printStack(cpu_stack);
    }
    
}


#undef FIRST_POP
#undef SECOND_POP
#undef ADD
#undef ARITHM_DBG
#undef DEF_CMD
#undef DBG