#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "text_funcs.h"
#include "debug_ASM.h"

#define DBG fprintf(stderr, "Compiled nicely -line: %d file: %s func: %s\n",            \
                                                __LINE__, __FILE__, __FUNCTION__)

// todo add call/ ret(от return) use another stack for that

#define DEF_CMD(name, num, arg, asmcode, ...)                                                                 \
    if (strcasecmp(cmd, #name) == 0)                                                                          \
    {                                                                                                         \
        output->number_of_comands++;                                                                          \
        output->code[output->ip] = num;                                                                       \
        output->ip++;                                                                                         \
                                                                                                              \
        if (arg)                                                                                              \
        {                                                                                                     \
            getArgs(output, arg_string);                                                                      \
            fprintf(asm_listing, "%04x\t%02x\t  %02x\t%s\n",                                                  \
            output->ip - 2, num, output->code[output->ip - 2], input->lines[line]);                           \
            dumpCmd(line, cmd, 1, arg);                                                                       \                                                                                
                                                                                                              \
        }                                                                                                     \
        else                                                                                                  \
        {                                                                                                     \
                                                                                                              \
            fprintf(asm_listing, "%04x\t%02x  \t\t%s\n", output->ip - 1,                                      \
            output->code[output->ip - 1], input->lines[line]);                                                \
            dumpCmd(line, cmd, 0, arg);                                                                       \
        }                                                                                                     \
    }                                                                                                         \
    else


#define ASSERT_OK(asmPtr)                                                   \
   do{                                                                      \
      if (returnAsmError(asmPtr) != 0)                                      \
       {                                                                    \
           dumpAsm(asmPtr, __FILE__, __FUNCTION__, __LINE__);               \
       }                                                                    \
   } while (0)

    int compile(Text_info * input, Asm_info * output)
    {

        ASSERT_OK(output);

        FILE *asm_listing = fopen("source.lst", "w+");
        setvbuf(asm_listing, NULL, _IONBF, 0);
        
        fprintf(asm_listing, "adr \tcmd   arg \tsource\n");
    
        do
        {
            char *cmd = NULL;
            char *arg_string = NULL;

            for (int line = 0; line < input->number_of_lines; line++)
            {
                if (*input->lines[line] == '\0')
                    continue;

                sscanf(input->lines[line], "%ms%ms", &cmd, &arg_string);
                int len = strlen(cmd);
                int last_char = cmd[len-1];

                fprintf(stderr, "COMAND - %s COMAND_LENGTH - %d ARG - %s\n", cmd, len, arg_string);
    
                if (last_char == ':')
                {
                    fprintf(stderr, "command_len == %d\n", len);
                    cmd[len - 1] = '\0';
                    
                    writeLabelInFile(line, cmd, input, output, asm_listing);
                }else
                
                #include "comands.h"                
                {}

            }

            free(cmd);
            free(arg_string);

        } while (0);

        for (int counter = 0; counter < LABELS_SIZE; counter++)
            fprintf(stderr, "\tLabel[%d] = {%d, %s}\n", counter,
             output->labels[counter].address, output->labels[counter].name);

        ASSERT_OK(output);

        fclose(asm_listing);

        return EXIT_SUCCESS; // TODO use your enum with errors
    }

#undef DEF_CMD

int getFromLabels(Asm_info *output, char * label_name, int * argument)
{
    ASSERT_OK(output);

    // fprintf(stderr, "OOOOOOOOOOOOOOOOOOOOOOOOOO %s\n", label_name);

    for (int counter = 0; output->labels[counter].address < LABEL_POISON; counter++)
    {
        if (strcasecmp(label_name, output->labels[counter].name) == 0)
        {
            if (output->labels[counter].address == LABEL_POISON)
                return ASM_ERROR_ATTEMP_TO_USE_UNDEFINED_LABEL;

            *argument = output->labels[counter].address;
            break;
        }

    }

    ASSERT_OK(output);
    output->code_of_error |= ASM_ERROR_ATTEMP_TO_USE_UNDEFINED_LABEL;

    return ASM_ERROR_ATTEMP_TO_USE_UNDEFINED_LABEL;

}

int writeLabelInFile(int number_of_line, char *cmd, Text_info *input, Asm_info *output, FILE *asm_listing)
{
    ASSERT_OK(output);

    int label_value = output->ip;
    
    if (output->labels[LABELS_SIZE - 1].address != LABEL_POISON)
    {
        fprintf(stderr, "THERE IS NO MEMORY FOR ANOTHER TEXT LABEL\n");
        return ASM_ERROR_NO_MEMORY_FOR_TXT_LABEL;

    }

    int label_idx;
    
    for (label_idx = 0; output->labels[label_idx].address != LABEL_POISON; label_idx++)
    {
        if (output->labels[label_idx].address == label_value && strcasecmp(output->labels[label_idx].name, cmd) == 0)
        {
            goto labelfound;
        }
    }


    output->labels[label_idx].address = label_value;
    output->labels[label_idx].name = cmd;

    labelfound:

    fprintf(asm_listing, "%04x\tlb\t  %02x\t%s\n", output->ip, label_value, input->lines[number_of_line]);
    dumpCmd(number_of_line, cmd, label_idx, 1);

    output->ip++;  

    ASSERT_OK(output);

    return EXIT_SUCCESS;
}


#define DEF_CMD(name, num, arg, asmcode, ...)                           \
    case num:                                                           \
            asmcode                                                     \
            break;                                                  


int getArgs(Asm_info *output, char *arg_string)
{
    DBG;
    ASSERT_OK(output);
    DBG;
    int argument = 0;
    bool isMemory = false;
    bool isRegister = false;
    char label_name[64] = {};
    int num_of_comand = output->code[output->ip - 1];
    DBG;
    switch (num_of_comand & 0x1F)
    {
        #include "comands.h"

        default:
            fprintf(stderr, "GET HIGH 2");
            break;
    }

        ASSERT_OK(output);
        return EXIT_SUCCESS;
    }

#undef GET_REG
#undef DEF_CMD
#undef ASSERT_OK
#undef DBG