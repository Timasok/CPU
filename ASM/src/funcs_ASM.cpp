#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../include/funcs_ASM.h"

#define ASSERT_OK(textPtr)                                              \
   do{                                                                  \
      if (returnTextError(textPtr) != 0)                                \
       {  fprintf(stderr, "DUMP_CALLED: file: %s func: %s line: %d\n",  \ 
                                 __FILE__, __FUNCTION__, __LINE__);     \
           printText(textPtr);                                          \
       }                                                                \
   } while (0)

#define DBG fprintf(stderr, "Compiled nicely -line: %d file: %s func: %s\n",            \
                                                __LINE__, __FILE__, __FUNCTION__)

#define DEF_CMD(name, num, arg)                                                 \
        if (strIcmp(cmd, #name) == 0)                                               \
    {                                                                           \        
                                                                                \
    output->number_of_comands++;                                                \
    output->code[output->ip++] = num;                                           \
                                                                                \
    if (arg)                                                                    \
    {                                                                           \
        getArgs(output, input->lines[line]+ shift);                             \
        fprintf(asm_listing, "%04x\t%02x\t  %02x\t%s\n",                        \
        output->ip - 1, num, output->code[output->ip - 2], input->lines[line]); \
                                                                                \
    }else{                                                                      \
                                                                                \
        fprintf(asm_listing, "%04x\t%02x  \t\t%s\n", output->ip,                \
         output->code[output->ip -1], input->lines[line]);                      \
    }                                                                           \
                                                                                \
    dumpCmd(line, cmd, 0, arg);                                                 \ 
    }else


static const char* TXT_BORDER = "************************************************************";

int returnTextError(Text_info * text)
{
    //todo как побитово заставить ошибки обнуляться?
    
    // text->code_of_error |= ((!text->source_file) * TEXT_ERROR_SOURCE_FILE_IS_NULL);

    // text->code_of_error |= ((!text->lines) * TEXT_ERROR_LINES_IS_NULL);

    // text->code_of_error |= ((text->number_of_lines < 1) * TEXT_ERROR_NUMBER_OF_LINES_IS_INAPROPRIATE);

    // text->code_of_error |= ((!text->buf) * TEXT_ERROR_BUF_IS_NULL);

    // return text->code_of_error;
    return 0;
}

int textCtor(Text_info * text, const char * file_name)
{

    struct stat data = {};
    stat(file_name, &data);
    text->buf_length = data.st_size;

    text->source_file = fopen(file_name, "r");     

    ASSERT_OK(text);

    text->buf = (char *)calloc(text->buf_length + 2, sizeof(char));
    
    ASSERT_OK(text);

    fread(text->buf, sizeof(char), text->buf_length, text->source_file);

    ASSERT_OK(text);

    fclose(text->source_file);

    ASSERT_OK(text);

    text->buf[text->buf_length] = '\0';

    ASSERT_OK(text);

    text->number_of_lines = 1;
    for (size_t counter = 0;counter <= text->buf_length; counter++)
    {
        if(text->buf[counter] == '\n')
        {
            text->buf[counter] = '\0';
            text->number_of_lines++;
        }
        
    }

    ASSERT_OK(text);
    DBG;
    text->lines = (const char **)calloc(text->number_of_lines, sizeof(char *));
    DBG;
    ASSERT_OK(text);

    int buf_idx = 0;
    text->lines[0] = text->buf;
    
    ASSERT_OK(text);

    for (int line = 1; line < text->number_of_lines; line++)
    {
        while (text->buf[buf_idx++] != '\0')
            ;
        text->lines[line] = (text->buf + buf_idx);

    }

    ASSERT_OK(text);

    return 0;
}

int printText(Text_info * text)
{
    fprintf(stderr, "\n%s\n", TXT_BORDER);
    fprintf(stderr, "  code_of_error = %d\n", text->code_of_error);
    fprintf(stderr, "  buf_length = %d\n  number_of_lines = %d\n  buf: \n%s\n", text->buf_length, text->number_of_lines, text->buf);

    if (text->number_of_lines != 0)
        fprintf(stderr, "\tLines: \n");
    for (int counter = 0; counter < text->number_of_lines; counter++)
        printf("%s\n", text->lines[counter]);

    fprintf(stderr, "\n%s\n", TXT_BORDER);

    return 0;
}

int strIcmp(char *s, char *t)
{

    for(; tolower(*s) == tolower(*t); ++s, ++t)
    {
        if (*s == '\0')
        {
            return 0;
        }
    }

    return s - t;

}

int strChr(char *cs, char c)
{
    assert(cs != nullptr);

    int result = 0;

    while (*cs != c && *cs != '\0')
     {
        ++cs;
        result++;
     }

    if (*cs == '\0')
        return NULL;
    
    return result;

}


static int getFromLabels(Asm_info *output, int label_idx, int *argument)
{
    if (output->labels[label_idx] == 0)
    {
        return ERROR_ATTEMP_TO_USE_UNDEFINED_LABEL;

    }else{

        *argument = output->labels[label_idx];
    }
    return EXIT_SUCCESS;
}

static int writeLabelInFile(int number_of_line, char * cmd, Text_info *input, Asm_info *output, FILE * asm_listing)
{
    //assert - for ERROR is label right
    int len = strlen(cmd);
    *(cmd + len) == '\0';
    int label_idx = atoi(cmd);
    
    int label_value = output->ip++;

    if (output->labels[label_idx] = LABEL_POISON)
    {
        output->labels[label_idx] = label_value;
    
    }else{
    
        return ERROR_LABEL_IS_DEFINED_TWICE;
        
    }

    fprintf(asm_listing, "%04x\tlb\t  %02x\t%s\n", 
    output->ip, label_idx, input->lines[number_of_line]);

    dumpCmd(number_of_line, "label", label_idx, 1);

    return EXIT_SUCCESS;
}

int getArgs(Asm_info *output, const char * arg_beginning)
{

    int argument = 0;
    
    switch(output->code[output->ip - 1])
    {
        case CMD_PUSH:
                sscanf(arg_beginning, "%d", &argument);            
                output->code[output->ip++] = argument;
                break;

        case CMD_JMP:
                if (sscanf(arg_beginning, " :%d", &argument) == 1)
                {   
                    output->DNNTMSP *= 0;
                    int line_idx = argument;
                    getFromLabels(output, line_idx, &argument);
                
                }else {

                    sscanf(arg_beginning, " %d", &argument);

                }

                output->code[output->ip++] = argument;
                break;

        default:
                fprintf(stderr, "GET HIGH");
                break;
    }

    return EXIT_SUCCESS;
}

int compile(Text_info *input, Asm_info *output)
{
    
    int DNNTMSP = true; //do_not_need_to_make_second_passage

    FILE * asm_listing = fopen("listing.asm", "w");
    fprintf(asm_listing, "adr \tcmd   arg \tsource\n");

    char * cmd = (char *)calloc(1, sizeof(char *));

    do {

        output->ip = 0;
        output->number_of_comands = 0;
        int shift = 0; 

        for (int line = 0; line < input->number_of_lines; line++)
        {           

            sscanf(input->lines[line], "%s%n", cmd, &shift);
            int len = strlen(cmd);
// //assert for cmd syntax 

            if (strIcmp(cmd, "push") == 0)
            {
                
                output->number_of_comands++;                
                output->code[output->ip++] = CMD_PUSH;

                getArgs(output, input->lines[line]+ shift);

                dumpCmd(line, cmd, output->code[output->ip - 2], 1);
                fprintf(asm_listing, "%04x\t%02x\t  %02x\t%s\n", output->ip, CMD_PUSH, output->code[output->ip - 2], input->lines[line]);
                                

            }else if (strIcmp(cmd, "jmp") == 0)
            {
                
                output->number_of_comands++;
                output->code[output->ip++] = CMD_JMP;

                getArgs(output, input->lines[line]+ shift);

                dumpCmd(line, cmd, output->code[output->ip - 2], 1);
                fprintf(asm_listing, "%04x\t%02x\t  %2d\t%s\n", output->ip-1, CMD_JMP, output->code[output->ip - 2], input->lines[line]);
                

            }else if (strIcmp(cmd, "add") == 0)
            {

                output->number_of_comands++;
                output->code[output->ip++] = CMD_ADD;
                
                dumpCmd(line, cmd, 0, 0);
                fprintf(asm_listing, "%04x\t%02x  \t\t%s\n", output->ip, output->code[output->ip -1], input->lines[line]);

            // }else if (strIcmp(cmd, "sub") == 0){
            
            //     writeNonArgCmdInFile(line, cmd, argument, hasArg, input, output, asm_listing, CMD_SUB);
            //     continue;

            // }else if (strIcmp(cmd, "mul") == 0){

            //     writeNonArgCmdInFile(line, cmd, argument, hasArg, input, output, asm_listing, CMD_MUL);
            //     continue;

            // }else if (strIcmp(cmd, "div") == 0){

            //     writeNonArgCmdInFile(line, cmd, argument, hasArg, input, output, asm_listing, CMD_DIV);
            //     continue;

            // }else if (strIcmp(cmd, "out") == 0){

            //     writeNonArgCmdInFile(line, cmd, argument, hasArg, input, output, asm_listing, CMD_OUT);
            //     continue;

            }else if (strIcmp(cmd, "hlt") == 0){

                output->number_of_comands++;
                output->code[output->ip++] = CMD_HLT;
                
                dumpCmd(line, cmd, 0, 0);
                fprintf(asm_listing, "%04x\t%02x  \t\t%s\n", output->ip, output->code[output->ip -1], input->lines[line]);

            }else if ((*(cmd + len) == ':') == 0){
                
                writeLabelInFile(line, cmd, input, output, asm_listing);
            
            // }else if(strchr(input->lines[line], ';') != NULL)
            // {
            // }

            *cmd = -1;

            }
    }

    }  while(0);

    for (int counter = 0; counter < LABELS_SIZE; counter++)
        fprintf(stderr, "%d\t", output->labels[counter]);

    free(cmd);

    fclose(asm_listing);

    return EXIT_SUCCESS;
}


// int dumpCmd(int number_of_line, char * cmd, int argument, bool hasArg)
// {
//     switch(hasArg)
//     {
//        case true:  fprintf(stderr, "line - %d comand - %s argument - %d\n", number_of_line, cmd, argument);
//                 break;
//        case false:  fprintf(stderr, "line - %d comand - %s\n", number_of_line, cmd);
//                 break;
//        default: fprintf(stderr, "ERROR\n");
//                 break;
//     }
//     return 0;
    
// }

// int dumpAsm(Asm_info *output)
// {


// };

//todo write strIcmp



#undef DEF_CMD
#undef ASSERT_OK
#undef DBG