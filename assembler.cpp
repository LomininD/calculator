#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack/font_styles.h"
#include "calculator_cmd.h"

calc_commands determine_cmd(char* cmd);
err_t read_number_asm(int* number);
void clear_buffer(void);

int main()
{
    //printf(MAKE_BOLD("+++ ASSEMBLER +++\n"));

    char raw_cmd[10] = ""; // bad solution
    int scanned = scanf("%s", raw_cmd);

    while (scanned != -1)
    {
        clear_buffer();

        calc_commands cmd = determine_cmd(raw_cmd);

        if (cmd == HLT)
        {
            //printf("main: shutting down calculator\n");
            return 0;
        }

        scanned = scanf("%s", raw_cmd);
    }
    return 0;
}


err_t read_number_asm(int* number)
{
    assert(number != NULL);

    int success = scanf("%d", number);

    if (success == 1)
    {
        printf("%d\n", *number);
        return ok;
    }
    else
    {
        printf("read_number: failed to get number\n");
        clear_buffer();
        return wrong_number;
    }
}


calc_commands determine_cmd(char* cmd)
{
    assert(cmd != NULL);

    if (strcmp("PUSH", cmd) == 0)
    {
        printf("%d\n", PUSH);

        int number = 0;
        read_number_asm(&number);

        return PUSH;
    }
    else if (strcmp("ADD", cmd) == 0)
    {
        printf("%d\n", ADD);
        return ADD;
    }
    else if (strcmp("SUB", cmd) == 0)
    {
        printf("%d\n", SUB);
        return SUB;
    }
    else if (strcmp("DIV", cmd) == 0)
    {
        printf("%d\n", DIV);
        return DIV;
    }
    else if (strcmp("MULT", cmd) == 0)
    {
        printf("%d\n", MULT);
        return MULT;
    }
    else if (strcmp("SQRT", cmd) == 0)
    {
        printf("%d\n", SQRT);
        return SQRT;
    }
    else if (strcmp("OUT", cmd) == 0)
    {
        printf("%d\n", OUT);
        return OUT;
    }
    else if (strcmp("HLT", cmd) == 0)
    {
        printf("%d\n", HLT);
        return HLT;
    }

    printf("determine_cmd: unknown command\n");
    return UNKNOWN;
}


void clear_buffer(void)
{
    int c = '\0';
    while ((c = getchar()) != '\n' && c != EOF);
}




