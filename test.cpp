#include <stdio.h>
#include <stdlib.h>



int main()
{
    char* s = (char*) "JA :0g    ";
    int number = 0;

    char bad_symb[5] = {};
    int success = 0;

    success = sscanf(s, "%*s :%d%1s", &number, bad_symb);
    printf("%d %d %s\n", success, number, bad_symb);



    return 0;
}
