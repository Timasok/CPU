DEF_CMD(PUSH, 1, 1 , 
{
if (strchr(arg_string, '[') != nullptr && strchr(arg_string, ']') != nullptr)
    {
        isMemory = true;    
        output->code[output->ip - 1] |= MEM_MASK;
        *strchr(arg_string, '[') = ' ';
        *strchr(arg_string, ']') = '\0';
    }

    fprintf(stderr, "%s\n", arg_string);
    fprintf(stderr, "%d\n", atoi(arg_string));

    if(sscanf(arg_string, " %d", &argument) == 1)
    {
        pushDmp(argument, isRegister, isMemory);
        output->code[output->ip - 1] |= IMMED_MASK;
        output->code[output->ip++] = argument;
        break;

    } else {

        isRegister = true;
        char * reg_var;
        //CRINGE
        sscanf(arg_string, "%ms", &reg_var);
        fprintf(stderr, "%s\n", reg_var);
        GET_REG;

        pushDmp(argument, isRegister, isMemory);
        output->code[output->ip - 1] |= REG_MASK;
        output->code[output->ip++] = argument;
        break;
    }

    fprintf(stderr, "GET HIGH POPING");
} 
,
{ 
    if (num_of_comand & IMMED_MASK)
    {   
        tmp_arg = cpu->code[cpu->ip++];     
        
    }
    else if(num_of_comand & REG_MASK)
    {
        int reg_idx = cpu->code[cpu->ip++];
        tmp_arg = cpu->Reg[reg_idx];

    }else {

        fprintf(stderr, "Error: WRONG PUSH MASK: -line: %d file: %s func: %s\n",  __LINE__, __FILE__, __FUNCTION__);

        cpu->code_of_error |= CPU_ERROR_INCORRECT_PUSH_MASK;
        return CPU_ERROR_INCORRECT_PUSH_MASK;
    }

    if(num_of_comand & MEM_MASK)
    {
        operateWithRam(cpu, tmp_arg, argPtr);

    }else{

        *argPtr = tmp_arg;

    }

})

DEF_CMD(POP, 2, 1 ,
{
if (strchr(arg_string, '[') != nullptr && strchr(arg_string, ']') != nullptr)
    {
        isMemory = true;    
        output->code[output->ip - 1] |= MEM_MASK;
        *strchr(arg_string, '[') = ' ';
        *strchr(arg_string, ']') = '\0';
    }

    fprintf(stderr, "%s\n", arg_string);
    fprintf(stderr, "%d\n", atoi(arg_string));

    if(sscanf(arg_string, " %d", &argument) == 1)
    {
        pushDmp(argument, isRegister, isMemory);
        output->code[output->ip - 1] |= IMMED_MASK;
        output->code[output->ip++] = argument;
        break;

    } else {

        isRegister = true;
        char * reg_var;
        //CRINGE
        sscanf(arg_string, "%ms", &reg_var);
        fprintf(stderr, "%s\n", reg_var);
        GET_REG;

        pushDmp(argument, isRegister, isMemory);
        output->code[output->ip - 1] |= REG_MASK;
        output->code[output->ip++] = argument;
        break;
    }
   
    fprintf(stderr, "GET HIGH PUSHING");
}
, 
{ 

    if(num_of_comand & REG_MASK)
    {
        int reg_idx = cpu->code[cpu->ip++];
        tmp_arg = cpu->Reg[reg_idx];

    }else if (num_of_comand & IMMED_MASK)
    {   
        tmp_arg = cpu->code[cpu->ip++];
        detectedPopToImmed = true;
        
    }else {

        fprintf(stderr, "Error: WRONG POP MASK: -line: %d file: %s func: %s\n",  __LINE__, __FILE__, __FUNCTION__);

        cpu->code_of_error |= CPU_ERROR_INCORRECT_POP_MASK;
        return CPU_ERROR_INCORRECT_POP_MASK;
    }


    if(num_of_comand & MEM_MASK)
    {
        detectedPopToImmed = false;
        operateWithRam(cpu, tmp_arg, argPtr);

    }else{

        *argPtr = tmp_arg;

    }

    if (detectedPopToImmed == true)
    {
        fprintf(stderr, "Error: CANNOT POP into IMMED  -line: %d file: %s func: %s\n",  __LINE__, __FILE__, __FUNCTION__);
        cpu->code_of_error |= CPU_ERROR_ATTEMPT_TO_POP_INTO_IMMED;

    }

})

DEF_CMD (ADD, 3, 0 , {}, 
{ 
    FIRST_POP(cpu);
    SECOND_POP(cpu);                
    ARITHM( + , cpu);
    ARITHM_DBG(+);
})

DEF_CMD (SUB, 4, 0 , {},  
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);                
    ARITHM( - , cpu);
    ARITHM_DBG(-);
})

DEF_CMD (MUL, 5, 0 , {},  
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);                
    ARITHM( * , cpu);
    ARITHM_DBG(*);
})

DEF_CMD (DIV, 6, 0 , {}, 
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);                
    ARITHM( /, cpu);
    ARITHM_DBG(/);    
    
})

DEF_CMD(OUT, 7, 0 , {}, 
{
    SINGLE_POP(cpu, &out);
    OUT(cpu, &out);

})

DEF_CMD(DUP, 8, 0 ,  {}, 
{
    elem_t tmp;
    SINGLE_POP(cpu, &tmp);
    SINGLE_PUSH(cpu, tmp);
    SINGLE_PUSH(cpu, tmp);

})

DEF_CMD(IN, 9, 0,  {}, { })

DEF_CMD(DMP, 10 , 0,  {}, { })

DEF_CMD(JMP, 11 , 1, 
{
    
    if (strchr(arg_string, ':') != nullptr)
    {
        output->compile_once *= 0;
        DBG;
        printf("%s\n", arg_string);
        // assert((strchr(arg_string, ':') + 1) != NULL);
        sscanf(strchr(arg_string, ':'), " :%s", label_name);        
        DBG;
        getFromLabels(output, arg_string, &argument);
        //free(label_name);
    }
    else
    {
        sscanf(arg_string, " %d", &argument);
    }

    output->code[output->ip++] = argument;

}, 

{
    JUMP(cpu);
})

DEF_CMD(JB, 12, 1,  
{
    if (strchr(arg_string, ':') != nullptr)
    {
        output->compile_once *= 0;
        DBG;
        printf("%s\n", arg_string);
        // assert((strchr(arg_string, ':') + 1) != NULL);
        sscanf(strchr(arg_string, ':'), " :%s", label_name);        
        DBG;
        getFromLabels(output, arg_string, &argument);
        //free(label_name);
    }
    else
    {
        sscanf(arg_string, " %d", &argument);
    }

    output->code[output->ip++] = argument;
    }, 
{ 
    FIRST_POP(cpu);
    SECOND_POP(cpu);
    COND_JUMP(cpu, <);

})

DEF_CMD(JBE, 13 , 1,  
{
    if (strchr(arg_string, ':') != nullptr)
    {
        output->compile_once *= 0;
        DBG;
        printf("%s\n", arg_string);
        // assert((strchr(arg_string, ':') + 1) != NULL);
        sscanf(strchr(arg_string, ':'), " :%s", label_name);        
        DBG;
        getFromLabels(output, arg_string, &argument);
        //free(label_name);
    }
    else
    {
        sscanf(arg_string, " %d", &argument);
    }

    output->code[output->ip++] = argument;
}, 
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);
    COND_JUMP(cpu, <=);
})

DEF_CMD(JA, 14, 1, 
{
    if (strchr(arg_string, ':') != nullptr)
    {
        output->compile_once *= 0;
        DBG;
        printf("%s\n", arg_string);
        // assert((strchr(arg_string, ':') + 1) != NULL);
        sscanf(strchr(arg_string, ':'), " :%s", label_name);        
        DBG;
        getFromLabels(output, arg_string, &argument);
        //free(label_name);
    }
    else
    {
        sscanf(arg_string, " %d", &argument);
    }

    output->code[output->ip++] = argument;
}, 
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);
    COND_JUMP(cpu, >);
})

DEF_CMD(JAE, 15 , 1, 
{
    if (strchr(arg_string, ':') != nullptr)
    {
        output->compile_once *= 0;
        DBG;
        printf("%s\n", arg_string);
        // assert((strchr(arg_string, ':') + 1) != NULL);
        sscanf(strchr(arg_string, ':'), " :%s", label_name);        
        DBG;
        getFromLabels(output, arg_string, &argument);
        //free(label_name);
    }
    else
    {
        sscanf(arg_string, " %d", &argument);
    }

    output->code[output->ip++] = argument;
}, 
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);
    COND_JUMP(cpu, >=);
})

DEF_CMD(JE, 16, 1, 
{
    if (strchr(arg_string, ':') != nullptr)
    {
        output->compile_once *= 0;
        DBG;
        printf("%s\n", arg_string);
        // assert((strchr(arg_string, ':') + 1) != NULL);
        sscanf(strchr(arg_string, ':'), " :%s", label_name);        
        DBG;
        getFromLabels(output, arg_string, &argument);
        //free(label_name);
    }
    else
    {
        sscanf(arg_string, " %d", &argument);
    }

    output->code[output->ip++] = argument;
},
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);
    COND_JUMP(cpu, ==);
})

DEF_CMD(JNE, 17 , 1, 
{
    if (strchr(arg_string, ':') != nullptr)
    {
        output->compile_once *= 0;
        DBG;
        printf("%s\n", arg_string);
        // assert((strchr(arg_string, ':') + 1) != NULL);
        sscanf(strchr(arg_string, ':'), " :%s", label_name);        
        DBG;
        getFromLabels(output, arg_string, &argument);
        //free(label_name);
    }
    else
    {
        sscanf(arg_string, " %d", &argument);
    }

    output->code[output->ip++] = argument;
}, 
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);
    COND_JUMP(cpu, !=);
})

DEF_CMD(SHOW, 18, 0, {}, { })

DEF_CMD(HLT, 0, 0, {}, 
{
    return 0;
})