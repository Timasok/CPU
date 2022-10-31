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

int dump_CMD_CPU(int ip, int CMD, CPU_info *cpu, bool stackChanged)
{
    fprintf(cpu->log_file, "\nComand - ");
    // printBits(CMD);
    fprintf(cpu->log_file, "\tip - %d\n", ip);
    
    if (stackChanged)
    {
        fprintf(cpu->log_file, "STACK CHANGED!\n");
        printStack(&cpu->stack, cpu->log_file);
    }
    
}

int printBits(int x)
{
    // do{
    //     printf("%d", x%2);
    //     x /= 2;

    // }while(x);

    return EXIT_SUCCESS;
}