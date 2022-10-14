#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../include/funcs_CPU.h"
#include "../stack/include/stack_funcs.h"

FILE *err_file = fopen("../../err_file.txt","w");

int main()
{
/*
    // // const char file_name[] = "source.txt";
    // const char file_name[] = "/../../source.txt";

    // struct stat data = {};
    // stat(file_name, &data);
    // size_t buf_length = data.st_size;

    // FILE * asm_file = fopen(file_name, "r");     
    
    // char * buf = (char *)calloc(buf_length + 2, sizeof(char));
    // fread(buf, sizeof(char), buf_length, asm_file);
    // fclose(asm_file);
    // buf[buf_length] = '\0';

    // int number_of_lines = 1;
    // size_t counter = 0;
    // while (counter < buf_length)
    // {
    //     if(buf[counter] == '\n')
    //     {
    //         buf[counter] = '\0';
    //         number_of_lines++;
    //     }
    //     ++counter;
    // }
*/
    setvbuf(err_file, NULL, _IONBF, 0);

    Stack stk1 = {};


    elem_t value;  

    stackCtor(stk1, 8);
    printStack(&stk1);

    stackPush(&stk1, 1.7);
    printStack(&stk1);

    stackDtor(&stk1);

    fclose(err_file);       

    return EXIT_SUCCESS;
}