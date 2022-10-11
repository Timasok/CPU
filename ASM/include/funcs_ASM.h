#include <stdio.h>
#include <sys/types.h>

const int LABELS_SIZE  = 5;
const int LABEL_POISON = 0xFF;

struct  Text_info
{
    FILE * source_file;
    char *buf;
    size_t buf_length;
    int number_of_lines;
    const char ** lines;
    int code_of_error;

};

struct Asm_info
{
    int labels[LABELS_SIZE] = {};
    int version;
    int number_of_comands;
    int ip;
    int * code;
    int DNNTMSP = 1;
    //add number of filled labels
};

enum TEXT_ERRORS
{
    TEXT_ERROR_SOURCE_FILE_IS_NULL              = 0x1,
    TEXT_ERROR_BUF_IS_NULL                      = 0x2,
    TEXT_ERROR_LINES_IS_NULL                    = 0x4,
    TEXT_ERROR_NUMBER_OF_LINES_IS_INAPROPRIATE  = 0x8
};

enum CMD
{
    CMD_PUSH = 1,
    CMD_ADD  = 2,
    CMD_SUB  = 3,
    CMD_MUL  = 4,
    CMD_DIV  = 5,
    CMD_OUT  = 6,
    CMD_DUP  = 7,
    CMD_IN   = 8,
    CMD_DMP  = 9,
    CMD_JMP  = 10,
    CMD_HLT  = 0
};

enum ASM_ERRORS
{
    ERROR_INVALID_FILE_POINTER          = 0x1,
    ERROR_STAT_IS_NULL                  = 0x2,
    ERROR_LABEL_IS_DEFINED_TWICE        = 0x4,
    ERROR_ATTEMP_TO_USE_UNDEFINED_LABEL = 0x8,
    ERROR_INCORRECT_JMPARG_FORMAT       = 0x10

};

int textDtor(Text_info * text);
int printText(Text_info * text);

//todo extract in module
int strIcmp(char *s, char *t);
int strChr(char *cs, char c);


int textCtor(Text_info * text, const char * file_name);
int compile(Text_info *input, Asm_info *output);

int dumpAsm(Asm_info *output);
int dumpCmd(int number_of_line, char * cmd, int argument, bool hasArg);
int returnTextError(Text_info * text);

int getArgs(Asm_info *output, const char * arg_beginning);