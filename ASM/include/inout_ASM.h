#include "funcs_ASM.h"

#ifndef INOUT_ASM_H
#define INOUT_ASM_H

int asmCtor(Asm_info *executable, char * asm_name, int maximum_quantity);
int writeAssemblerInFile(Asm_info * executable, FILE * txt_reserve);
int asmDtor(Asm_info *executable);

#endif