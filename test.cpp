#include <stdio.h>
#include <stdlib.h>

int main()
{

    char * cmd;
    
    int size = sizeof("I'll get ya", "smoke some", "get lost", "die slow mthf");
    char ** text = (char **)calloc(4, sizeof(char *));

    text[0] = "I'll get ya";
    text[1] = "Smoke some";
    text[2] = "Get lost";
    text[3] = "Die slow mthf";

    for (int counter = 0; counter < 4; counter++)
    {
        // printf("%s\n", text[counter]);

        sscanf(text[counter], "%ms", &cmd);
        printf("%s ", cmd);

        sscanf(text[counter], "%ms%ms", &cmd, &cmd);
        printf("%s\n", cmd);

        // sscanf(text[counter], "%ms%ms%ms", &cmd, &cmd, &cmd);
        // printf("%s\n", cmd);

    }
    free(cmd);

    return 0;
}