#ifndef DEBUG_ASM_H
#define DEBUG_ASM_H

#include "funcs_ASM.h"
#include <stdlib.h>

#define DBG fprintf(stderr, "Compiled nicely -line: %d file: %s func: %s\n",            \
                                                __LINE__, __FILE__, __FUNCTION__)

enum ASM_ERRORS
{
    ASM_ERROR_INVALID_FILE_POINTER          = 1 << 0,
    ASM_ERROR_STAT_IS_NULL                  = 1 << 1,
    ASM_ERROR_LABEL_IS_DEFINED_TWICE        = 1 << 2,
    ASM_ERROR_ATTEMP_TO_USE_UNDEFINED_LABEL = 1 << 3,
    ASM_ERROR_INCORRECT_JMP_ARG_FORMAT      = 1 << 4,
    ASM_ERROR_INCORRECT_SIGNATURE           = 1 << 5,
    ASM_ERROR_INCORRECT_VERSION             = 1 << 6,
    ASM_ERROR_INVALID_SECOND_PASS_BOOL      = 1 << 7,
    ASM_ERROR_INCORRECT_IP                  = 1 << 8,
    ASM_ERROR_INCORRECT_NUMBER_OF_COMANDS   = 1 << 9,
    ASM_ERROR_CODE_IS_NULL                  = 1 << 10,
    ASM_ERROR_NO_MEMORY_FOR_TXT_LABEL       = 1 << 11,

};

int returnAsmError(Asm_info *output);
int strAsmError(int code_of_error);
int dumpAsm(Asm_info *output, const char *name_of_file, const char *name_of_func, int number_of_line);
int dumpCmd(int number_of_line, char * cmd, int argument, bool hasArg);
int pushDmp(int argument, bool isRegister, bool isMemory);

#endif