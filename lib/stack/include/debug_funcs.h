#include "../include/objects_structs_constants.h"

void hardAssert(bool condition); 
// void verifyStack(Stack * stk, int mode);

const char * stackStrError(Stack *stk);
int returnStackError(Stack *stk);

void printVarInfo(Stack *stk);

void stackDump(Stack *stk, const char * name_of_file, 
                                            const char * name_of_func, int number_of_line);                                   