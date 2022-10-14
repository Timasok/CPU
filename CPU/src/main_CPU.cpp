#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../include/funcs_CPU.h"
#include "../stack/include/stack_funcs.h"
// #include "../stack/include/debug_funcs.h"

#define DBG fprintf(stderr, "Compiled nicely -line: %d file: %s func: %s\n",            \
                                                __LINE__, __FILE__, __FUNCTION__)


int process(CPU_info * cpu, Stack * cpu_stack);

FILE *err_file = fopen("../../err_file.txt","w");

int main()
{

    setvbuf(err_file, NULL, _IONBF, 0);

    FILE * asm_source = fopen("../../source.asm", "rb");

    CPU_info cpu;
    DBG;
    fread(&cpu.version, sizeof(int), 1, asm_source);
    fread(&cpu.quantity, sizeof(int), 1, asm_source);    
    fread(&cpu.number_of_comands, sizeof(int), 1, asm_source);
    DBG;
    cpu.code = (int *)calloc(cpu.quantity, sizeof(int));
    DBG;
    fread(&cpu.code, sizeof(int), cpu.quantity, asm_source);
    DBG;
    Stack cpu_stack = {};

    elem_t value;  

    stackCtor(cpu_stack, 8);
    printStack(&cpu_stack);

    stackPush(&cpu_stack, 1.7);
    printStack(&cpu_stack);

    stackDtor(&cpu_stack);
    fclose(err_file);       

    return EXIT_SUCCESS;
}

int process(CPU_info * cpu, Stack * cpu_stack)
{
    cpu->ip = 0;

    while (cpu->ip < cpu->quantity)
    {
        fprintf(stderr, "\n%d", cpu->code[cpu->ip++]);

    }


    return EXIT_SUCCESS;
}