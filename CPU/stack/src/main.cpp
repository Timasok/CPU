#include <stdio.h>

#include "../include/stack_funcs.h"

FILE *err_file = fopen("err_file.txt","w");
int main()
{

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