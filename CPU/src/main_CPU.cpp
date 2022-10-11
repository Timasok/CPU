#include <stdio.h>
#include <stdlib.h>

#include "../include/funcs.h"

int main()
{

    // const char file_name[] = "source.txt";
    const char file_name[] = "/../../source.txt";

    struct stat data = {};
    stat(file_name, &data);
    size_t buf_length = data.st_size;

    FILE * asm_file = fopen(file_name, "r");     
    
    char * buf = (char *)calloc(buf_length + 2, sizeof(char));
    fread(buf, sizeof(char), buf_length, asm_file);
    fclose(asm_file);
    buf[buf_length] = '\0';

    int number_of_lines = 1;
    size_t counter = 0;
    while (counter < buf_length)
    {
        if(buf[counter] == '\n')
        {
            buf[counter] = '\0';
            number_of_lines++;
        }
        ++counter;
    }

    return EXIT_SUCCESS;
}