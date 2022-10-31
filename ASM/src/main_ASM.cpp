#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "funcs_ASM.h"
#include "inout_ASM.h"
#include "debug_ASM.h"

FILE *text_logs;

int main(int argc, char ** argv)
{
    char * text_name = "source.txt";
    char * asm_name = "a.code";

    char asm_reserve[] = "auf.txt";
    FILE * txt_reserve = fopen(asm_reserve, "w+");
    openAsmLogs();

    if (argc == 3)
    {
        text_name = strdup(argv[1]);
        asm_name  = strdup(argv[2]);
    }

    Text_info source = {};
    textCtor(&source, text_name); 
    printText(&source);

    Asm_info executable = {};
    DBG_OUT;
    asmCtor(&executable, asm_name, 2 * (source.number_of_lines + 1));

    DBG_OUT;
    assert(executable.asm_log != NULL);
    compile(&source, &executable);
    if (executable.compile_once == 0)
    {
        executable.ip = 0;
        compile(&source, &executable);        
    }

    DBG_OUT;

    for (int counter = 0; counter < executable.ip; counter++)
        fprintf(executable.asm_log, "%d\t", executable.code[counter]);
    DBG_OUT;
    writeAssemblerInFile(&executable, txt_reserve);
    DBG_OUT;
    asmDtor(&executable);
    DBG_OUT;

    closeAsmLogs();

    return EXIT_SUCCESS;
}
