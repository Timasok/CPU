#include <stdio.h>

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


enum ASM_ERRORS
{
    ERROR_INVALID_FILE_POINTER          = 0x1,
    ERROR_STAT_IS_NULL                  = 0x2,
    ERROR_LABEL_IS_DEFINED_TWICE        = 0x4,
    ERROR_ATTEMP_TO_USE_UNDEFINED_LABEL = 0x8,
    ERROR_INCORRECT_JMPARG_FORMAT       = 0x10

};