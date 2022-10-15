#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <errno.h>

#include "../include/debug_funcs.h"

#define ASSERT_OK(stkPtr)                                        \
//    do{                                                        \
//       if (returnStackError(stkPtr))                           \
//          stackDump(stkPtr, __FILE__, __FUNCTION__, __LINE__); \
//    } while (0)

static void * getStackElement(Stack * stk, int index)
{
    hardAssert(stk != NULL);
    return (stk->data + index);

}

static void setStackElement(Stack * stk, int index, elem_t element)
{
    hardAssert(element != NULL);
    hardAssert(stk != NULL);

    *(stk->data + index) = element;
      
}

static void fillWithPoison(Stack *stk, size_t size, size_t capacity)
{
    // memset(getStackElement(stk, size), POISON, capacity - size -1 );

    for (size_t counter = stk->size; counter < stk->capacity; counter++)
        setStackElement(stk, counter, POISON);
}

void stackResize(Stack * stk, size_t new_capacity)
{
    ASSERT_OK(stk);

    if (new_capacity <= stk->capacity)
        return ;

    elem_t* tmp_ptr = (elem_t*)realloc(stk->data, new_capacity * sizeof(elem_t));

    hardAssert(tmp_ptr != NULL);

    stk->data = tmp_ptr;
    stk->capacity = new_capacity;

    fillWithPoison(stk, stk->size, stk->capacity);

    ASSERT_OK(stk);
     
}

void stack_Ctor(Stack *stk, size_t capacity, const char * name_of_var, 
                                        const char * name_of_file, const char * name_of_func, int number_of_line)
{   
    ASSERT_OK(stk);

    stk->var_info = {.name_of_var  = name_of_var, 
    .name_of_file = name_of_file, .name_of_func = name_of_func, .number_of_line  = number_of_line};

    // stk->var_info.name_of_var  = name_of_var;
    // stk->var_info.name_of_file = name_of_file;
    // stk->var_info.name_of_func = name_of_func;
    // stk->var_info.number_of_line  = number_of_line;

    hardAssert(stk != NULL);

    stk->data = (elem_t *)calloc(capacity , sizeof(elem_t));
    hardAssert(stk->data != NULL);

    stk->code_of_error = 0;
    stk->capacity = capacity;
    stk->size = 0;    

    fillWithPoison(stk, stk->size, stk->capacity);

    ASSERT_OK(stk);

}

void stackPush(Stack *stk, elem_t element)
{
   
    ASSERT_OK(stk);

    if (stk->size >= stk->capacity)
        stackResize(stk, stk->capacity * STACK_RESIZE_IF_PUSH); 

    setStackElement(stk, stk->size, element);
    stk->size++;

    ASSERT_OK(stk);

}

void stackPop(Stack *stk, elem_t * element) 
{
    ASSERT_OK(stk);

    if (stk->size == 0)
    {
        fprintf(stderr,"You attained the end of stack\n");
        return ;
    }

    if (stk->size * STACK_RESIZE_IF_POP < stk->capacity)
        stackResize(stk, stk->capacity);

    elem_t result = *(elem_t *)getStackElement(stk, stk->size-1);

    setStackElement(stk,--stk->size, POISON);

    ASSERT_OK(stk);

}

void stackDtor(Stack *stk)
{
    ASSERT_OK(stk);

    fillWithPoison(stk, stk->size, stk->capacity);
    free((void *)stk->data);

}

#undef ASSERT_OK(stkPtr)