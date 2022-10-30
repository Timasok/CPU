#include <stdlib.h>
#include <string.h>

#include "funcs_ASM.h"
#include "inout_ASM.h"

#define DBG printf("Compiled nicely -line: %d file: %s func: %s\n",                 \
                                                __LINE__, __FILE__, __FUNCTION__)

int main(int argc, char ** argv)
{
    char * text_name = "source.txt";
    char * asm_name = "a.code";

    char asm_reserve[] = "auf.txt";
    FILE * txt_reserve = fopen(asm_reserve, "w+");

    if (argc == 3)
    {
        text_name = strdup(argv[1]);
        asm_name  = strdup(argv[2]);
    }

    Text_info source = {};
    textCtor(&source, text_name); 
    printText(&source);

    Asm_info executable = {};
    DBG;
    asmCtor(&executable, asm_name, 2 * (source.number_of_lines + 1));
    DBG;

    printf("BUFFER SIZE: %d\n", 2 * (source.number_of_lines + 1));
    compile(&source, &executable);
    if (executable.compile_once == 0)
    {
        executable.ip = 0;
        compile(&source, &executable);        
    }

    DBG;

    for (int counter = 0; counter < executable.ip; counter++)
        fprintf(stderr, "%d\t", executable.code[counter]);
    DBG;
    writeAssemblerInFile(&executable, txt_reserve);
    DBG;
    asmDtor(&executable);
    DBG;

    return EXIT_SUCCESS;
}

#undef DBG