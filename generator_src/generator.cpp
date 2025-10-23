#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    FILE* in_fp = fopen(argv[1], "r");
    FILE* out_fp = fopen(argv[2], "w");

    char c = 0;
    int i = 0;

    fprintf(out_fp, "PUSH 0\n");
    fprintf(out_fp, "POPREG AX\n\n");

    int scanned = fscanf(in_fp, "%c", &c);
    while (scanned != -1)
    {
        //printf("%d %c\n", i, c);

        if (c != '\n' && c != '\r' /*&& i % 2 == 0*/)
        {
            printf("%d %c\n", i, c);
            fprintf(out_fp, "PUSH %d\n", c);
            fprintf(out_fp, "POPM [AX]\n\n");

            fprintf(out_fp, "PUSHREG AX\n");
            fprintf(out_fp, "PUSH 1\n");
            fprintf(out_fp, "ADD\n");
            fprintf(out_fp, "POPREG AX\n\n");

        }

        i += 1;
        scanned = fscanf(in_fp, "%c", &c);
    }

    fprintf(out_fp, "DRAW\n");
    fprintf(out_fp, "HLT\n");

    return 0;
}






