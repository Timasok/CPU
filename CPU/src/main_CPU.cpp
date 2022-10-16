#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

//todo use -I flag
#include "../include/funcs_CPU.h"

//todo we need to add log file in CPU struct

#define DEBUG_MODE

#ifdef DEBUG_MODE
#define DBG fprintf(stderr, "Compiled nicely -line: %d file: %s func: %s\n",            \
                                                __LINE__, __FILE__, __FUNCTION__)
#endif
#ifndef DEBUG_MODE
#define DBG
#endif


FILE *err_file = fopen("../../err_file.txt","w");

int main(int argc, const char* argv[])
{

    setvbuf(err_file, NULL, _IONBF, 0);

    FILE * asm_source = fopen("source.asm", "rb");

    FILE * log_file = fopen("log_file.txt", "w");        
    
    CPU_info cpu;
    CPU_Ctor(&cpu, asm_source);
    dump_CPU(&cpu, log_file);

    Stack cpu_stack = {};
    DBG;

    process(&cpu, &cpu_stack);
    // printStack(&cpu_stack);
    DBG;
    free(cpu.code);

    fcloseall();
    return EXIT_SUCCESS;
}

#undef DBG