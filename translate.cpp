#include <stdio.h>

int main()
{
    FILE * translated_file = fopen("transcription_result.txt", "w");

    char input[1024];
    int counter = 0;
    do
    {   
        input[counter] = getchar();

    }while(input[counter++] != '\n');
    input[counter] = '\0';

    for (int idx = 0; idx < counter; idx++)
    {
        fprintf(translated_file, "push [%d]\n", input[idx]);
    }

    fclose(translated_file);

    return 0;
}