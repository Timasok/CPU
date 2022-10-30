#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

// #include "funcs_CPU.h"
#include "debug_CPU.h"

#define DEBUG_MODE

#ifdef DEBUG_MODE
#define DBG_OUT fprintf(stderr, "Compiled nicely -line: %d file: %s func: %s\n",            \
                                                __LINE__, __FILE__, __FUNCTION__)
#endif
#ifndef DEBUG_MODE
#define DBG_OUT
#endif

int main(int argc, const char* argv[])
{
    
    FILE * asm_source = fopen("a.code", "rb");

    CPU_info cpu;
    CPU_Ctor(&cpu, asm_source);
    dump_CPU(&cpu);
    DBG_OUT;

    process(&cpu);
    // printStack(&cpu->stack);

    fcloseall();
    CPU_Dtor(&cpu);
    

    return EXIT_SUCCESS;
}

#undef DBG_OUT