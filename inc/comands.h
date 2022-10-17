DEF_CMD(PUSH, 1, 1 , { })

DEF_CMD(POP, 2, 1 , { })

DEF_CMD (ADD, 3, 0 , 
{ 
    FIRST_POP(cpu_stack);
    SECOND_POP(cpu_stack);                
    ARITHM( + , cpu_stack);
    ARITHM_DBG(+);
})

DEF_CMD (SUB, 4, 0 , 
{
    FIRST_POP(cpu_stack);
    SECOND_POP(cpu_stack);                
    ARITHM( - , cpu_stack);
    ARITHM_DBG(-);
})

DEF_CMD (MUL, 5, 0 , 
{
    FIRST_POP(cpu_stack);
    SECOND_POP(cpu_stack);                
    ARITHM( * , cpu_stack);
    ARITHM_DBG(*);
})

DEF_CMD (DIV, 6, 0 , 
{
    FIRST_POP(cpu_stack);
    SECOND_POP(cpu_stack);                
    ARITHM( /, cpu_stack);
    ARITHM_DBG(/);    
    
})

DEF_CMD(OUT, 7, 0 , 
{
    SINGLE_POP(cpu_stack, &out);
    OUT(cpu, &out);

})

DEF_CMD(DUP, 8, 0 , 
{
    elem_t tmp;
    SINGLE_POP(cpu_stack, &tmp);
    SINGLE_PUSH(cpu_stack, tmp);
    SINGLE_PUSH(cpu_stack, tmp);

})

DEF_CMD(IN, 9, 0, { })

DEF_CMD(DMP, 10 , 0, { })

DEF_CMD(JMP, 11 , 1, 
{
    JUMP(cpu);
})

DEF_CMD(JB, 12, 1, 
{ 
    FIRST_POP(cpu_stack);
    SECOND_POP(cpu_stack);
    COND_JUMP(cpu, <);

})

DEF_CMD(JBE, 13 , 1, 
{
    FIRST_POP(cpu_stack);
    SECOND_POP(cpu_stack);
    COND_JUMP(cpu, <=);
})

DEF_CMD(JA, 14, 1,  
{
    FIRST_POP(cpu_stack);
    SECOND_POP(cpu_stack);
    COND_JUMP(cpu, >);
})

DEF_CMD(JAE, 15 , 1,  
{
    FIRST_POP(cpu_stack);
    SECOND_POP(cpu_stack);
    COND_JUMP(cpu, >=);
})

DEF_CMD(JE, 16, 1,  
{
    FIRST_POP(cpu_stack);
    SECOND_POP(cpu_stack);
    COND_JUMP(cpu, ==);
})

DEF_CMD(JNE, 17 , 1,  
{
    FIRST_POP(cpu_stack);
    SECOND_POP(cpu_stack);
    COND_JUMP(cpu, !=);
})

DEF_CMD(SHOW, 18, 0, { })

DEF_CMD(HLT, 0, 0, { })