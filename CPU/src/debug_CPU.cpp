#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

#include "debug_CPU.h"

int dump_CPU(CPU_info *cpu)
{
    fprintf(cpu->log_file, "\nSIGNATURE: %s\n"
                             "QUANTITY:  %d\n"
                             "NUMBER OF COMANDS: %d\n"
                      ,&(cpu->signature), cpu->quantity, cpu->number_of_comands);     

    for (int ip = 0; ip < cpu->quantity; ip++)
        fprintf(cpu->log_file, "%d\t", cpu->code[ip]);        

    fprintf(cpu->log_file, "\n");

}

//TODO Mark functions that change stack in defines
int dump_CMD_CPU(int ip, int CMD, CPU_info *cpu, bool stackChanged)
{
    fprintf(stderr, "\nComand - ");
    printBits(CMD);
    fprintf(stderr, "\tip - %d\n", ip);
    
    if (stackChanged)
    {
        fprintf(stderr, "STACK CHANGED!\n");
        printStack(&cpu->stack);
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