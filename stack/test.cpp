#include <stdio.h>
#include <stdlib.h>


int main()
{
    int* arr = (int*) calloc(5, sizeof(int));
    arr[0] = 0;
    arr[1] = 1;
    arr[2] = 2;
    arr[3] = 3;
    arr[4] = 4;

    for (int i = 0; i < 5; i++)
    {
        printf("%d\n", arr[i]);
    }

    arr = (int*) realloc(arr, 10 * sizeof(int));

    arr[5] = 5;
    arr[6] = 6;
    arr[7] = 7;
    arr[8] = 8;
    arr[9] = 9;

    printf("--------\n");

    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", arr[i]);
    }


    return 0;
}
