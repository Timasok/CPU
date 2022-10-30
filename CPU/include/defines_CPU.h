#define FIRST_POP(cpuPtr)                                                         \
            do {                                                                  \
                    if (stackPop(&cpuPtr->stack, &first_popped) == EXIT_FAILURE)   \
                    {                                                             \
                        break;                                                    \
                    }                                                             \
                } while (0)

#define SECOND_POP(cpuPtr)                                                        \
            do {                                                                  \
                    if (stackPop(&cpuPtr->stack, &second_popped) == EXIT_FAILURE)  \
                    {                                                             \
                        break;                                                    \
                    }                                                             \
                } while (0)    

#define ARITHM(operation, cpuPtr)                                                 \
            do {                                                                  \
                    stackPush(&cpuPtr->stack, second_popped operation second_popped); \
                                                                                  \
                } while (0)

#define SINGLE_POP(cpuPtr, poppedPtr)                                             \
            do {                                                                  \
                    if (stackPop(&cpuPtr->stack, poppedPtr) == EXIT_FAILURE)              \
                    {                                                             \
                        break;                                                    \
                    }                                                             \
                } while (0)

#define SINGLE_PUSH(cpuPtr, element)                                              \
            do {                                                                  \
                    stackPush(&cpuPtr->stack, element);                                   \
                } while (0)

#define OUT(cpuPtr, poppedPtr)                                                                 \
                    do {                                                         \
                        fprintf(cpu->log_file, "OUT: = %d", *poppedPtr);              \
                                                                                 \
                    } while (0)

#define ARITHM_DBG(operation)                                                       \ 
                fprintf(stderr, "%d %s %d\n", second_popped, #operation, first_popped)


#define JUMP(cpuPtr)                                                                \
        do {                                                                        \
                if (cpu->code[cpu->ip] < 0 || cpu->code[cpu->ip] >= cpu->quantity)  \
                    fprintf(stderr, "LABEL POINTS OUTDOORS ip = %d", cpu->ip);      \
                cpu->ip = cpu->code[cpu->ip++];                                     \
            } while (0)

#define COND_JUMP(cpuPtr, condition)                                            \
        do {                                                                    \
                if (second_popped condition first_popped){                      \                   
                    JUMP(cpuPtr);                                               \
                }else                                                           \
                {                                                               \
                    cpu->ip++;                                                  \
                }                                                               \
            } while (0)
