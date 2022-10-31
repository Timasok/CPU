#include <stdio.h>
#include <stdlib.h>

int main()
{
    system("./assembler source.txt b.code");
    system("./cpu b.code");
    return 1;
}