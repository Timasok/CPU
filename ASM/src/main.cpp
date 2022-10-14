#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/funcs_ASM.h"

#define DBG printf("Compiled nicely -line: %d file: %s func: %s\n",                 \
                                                __LINE__, __FILE__, __FUNCTION__)

int main()
{
//todo rename main
//todo add assert for apropriate name of source file 
    const char text_name[] = "source.txt";

    Text_info source;
    DBG;
    textCtor(&source, text_name); 
    DBG;
    printText(&source);
    DBG;
    Asm_info executable;
    DBG;
    memset(executable.labels, LABEL_POISON, LABELS_SIZE * sizeof(int));
    DBG;
    executable.code = (int *)calloc(2 * source.number_of_lines + 1, sizeof(int));
    DBG;
    compile(&source, &executable);
    DBG;
    // fprintf(stderr, "%d %d\n", executable.ip, executable.number_of_comands);
    DBG;

    if (executable.DNNTMSP == 0)
        compile(&source, &executable);


    const char asm_name[] = "source.asm";
    FILE * asm_file = fopen(asm_name, "wb");
    
    // for (int counter = 0; counter < executable.ip; counter++)
    //     fprintf(asm_file, "\t%d\n", executable.code[counter]);
    
    DBG;
    fwrite ("CP", sizeof(int), 1, asm_file);
    fwrite (&executable.ip, sizeof(int), 1, asm_file);
    fwrite (&executable.number_of_comands, sizeof(int), 1, asm_file);
    fwrite (executable.code, sizeof(int), executable.ip, asm_file);
    
    DBG;
    free(executable.code);

    return EXIT_SUCCESS;
}

#undef DBG

// fprintf(asembler.asm_file, "%s %d %d\n", "CP", asembler.version, asembler.number_of_comands);
