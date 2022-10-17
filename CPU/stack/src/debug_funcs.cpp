#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <stdio.h>

#include "../include/debug_funcs.h"

#define LOCATION(reason) fprintf(stderr, "%s: file: %s func: %s line: %d\n", reason, \
                                 __FILE__, __FUNCTION__, __LINE__)

#define ASSERT_OK(stkPtr)                                     \
   do{                                                        \
      if (returnStackError(stkPtr))                           \
         stackDump(stkPtr, __FILE__, __FUNCTION__, __LINE__); \
   } while (0)

void hardAssert(bool condition)//works the wrong way
{
   // if (!condition)
   // {
   //    fputs("CRITICAL ERROR\n", stderr);
   //    LOCATION("Terminated");
   //    abort();
   // }
}

// void verifyStack(Stack *stk, int mode)
// {
   
//    ASSERT_OK(stk);
   
// }

// #undef LOCATION(reason)

int returnStackError(Stack *stk)
{

   stk->code_of_error |= ((!stk->data) * STACK_ERROR_NULL);

   stk->code_of_error |= ((stk->size < 0) * STACK_ERROR_SIZE_BELOW_ZERO);

   stk->code_of_error |= ((stk->capacity < 0) * STACK_ERROR_CAPACITY_BELOW_ZERO);

   stk->code_of_error |= ((stk->size > stk->capacity) * STACK_ERROR_OVERSIZED);

   // stk->code_of_error |= ((!isLeftBufCanary(stk)) * STACK_ERROR_LEFTBUF_CANARY_DIED);

   // stk->code_of_error |= ((!isRightBufCanary(stk)) * STACK_ERROR_RIGHTBUF_CANARY_DIED);

   // stk->code_of_error |= ((!isLeftStructCanary(stk)) * STACK_ERROR_LEFTSTRUCT_CANARY_DIED);

   // stk->code_of_error |= ((!isRightStructCanary(stk)) * STACK_ERROR_RIGHTSTRUCT_CANARY_DIED);

   return stk->code_of_error;
}

const char *stackStrError(Stack *stk)
{
   // add light maybe
   char *result = (char *)calloc(100, sizeof(char));

   if (stk->code_of_error & STACK_ERROR_NULL)
      strcat(result, "ERROR: Data pointer = NULL\n");

   if (stk->code_of_error & STACK_ERROR_SIZE_BELOW_ZERO)
      strcat(result, "ERROR: Size < 0\n");

   if (stk->code_of_error & STACK_ERROR_CAPACITY_BELOW_ZERO)
      strcat(result, "ERROR: Capacity < 0\n");

   if (stk->code_of_error & STACK_ERROR_OVERSIZED)
      strcat(result, "ERROR: Size > capacity\n");

   // if (stk->code_of_error & STACK_ERROR_LEFTBUF_CANARY_DIED)
   //    strcat(result, "ERROR: LEFT BUF CANARY IS DEAD\n");

   // if (stk->code_of_error & STACK_ERROR_RIGHTBUF_CANARY_DIED)
   //    strcat(result, "ERROR: RIGHT BUF CANARY IS DEAD\n");

   // if (stk->code_of_error & STACK_ERROR_LEFTSTRUCT_CANARY_DIED)
   //    strcat(result, "ERROR: RIGHT STRUCT CANARY IS DEAD\n");

   // if (stk->code_of_error & STACK_ERROR_RIGHTSTRUCT_CANARY_DIED)
   //    strcat(result, "ERROR: RIGHT STRUCT CANARY IS DEAD\n");

   // if (stk->code_of_error & STACK_ERROR_WRONG_BUF_HASH)
   //    strcat(result, "ERROR: BUF HASH WAS UNPREDICTABLY CHANGED\n");

   // if (stk->code_of_error & STACK_ERROR_WRONG_HASH)
   //    strcat(result, "ERROR: HASH WAS UNPREDICTABLY CHANGED\n");

   if (stk->code_of_error == 0)
      strcat(result, "(ok)");

   return result;
}

//todo printElementType
static void printElement(Stack *stk, int index)
{
   fprintf(stderr, "\t*[%d] = %d\n", index, *(stk->data + index)); 

}

void printStack(Stack *stk)
{
   fprintf(stderr, "{\n");
   fprintf(stderr, "size = %d\n", stk->size);
   fprintf(stderr, "capacity = %d\n", stk->capacity);
   fprintf(stderr, "data[%p]\n" , stk->data); 
   fprintf(stderr, "code_of_error = %d\n", stk->code_of_error);
   fprintf(stderr, "{\n\t{\n");

   int counter = stk->capacity;
   while(--counter >= 0)
   {
      if(counter == stk->size - 1)
         fputs(POISON_BORDER, stderr);

      printElement(stk, counter);

   }

   fprintf(stderr, "\t}\n}\n");
   
}

void printVarInfo(Stack *stk)
{
   ASSERT_OK(stk);
   fprintf(stderr, "Stack[%p] "
                     "%s"
                     " at %s at %s(%d)\n",
           stk,
           stk->var_info.name_of_var, stk->var_info.name_of_func,
           stk->var_info.name_of_file, stk->var_info.number_of_line);

}

void stackDump(Stack *stk, const char *name_of_file, const char *name_of_func,
                      int number_of_line)
{

   fprintf(stderr, "%s at %s(%d)\n", name_of_func, name_of_file, number_of_line);

   fprintf(stderr, "\e[0;31m%s\e[0m", stackStrError(stk));

   printVarInfo(stk);

   printStack(stk);

}

#undef ASSERT_OK(stkPtr)