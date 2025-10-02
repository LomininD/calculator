// #include "calculator_cmd.h"
#include <stdio.h>
#include <assert.h>


void calc_push(st_t* st, int* code, int* ip)
{
    assert(code != NULL);
    assert(code != ip);

    printf("execute_cmd: began push\n");
    int number = code[*(++ip)];

    // add number check

    st_return_err pushed = st_push(st, number);

    if (pushed == no_error)
    {
        st_dump(st);
        printf("execute_cmd: push succeeded\n");
    }
    else
        printf("execute_cmd: push failed\n");
}


void calc_add(st_t* st)
{
    assert(st != NULL);

    printf("execute_cmd: began add\n");

    int a = 0;
    int b = 0;
    st_return_err got_a = st_pop(st, &a); // assert?
    st_return_err got_b = st_pop(st, &b);
    int res = a + b;

    if (got_a == no_error && got_b == no_error)
    {
        st_return_err pushed = st_push(st, res);
        if (pushed == no_error)
            printf("execute_cmd: add succeeded, result = %d\n", res);
        else
            printf("execute_cmd: add failed (failed to push the result)\n");
    }
    else
    {
        printf("execute_cmd: add failed (not enough data in stack)\n");
    }
}


void calc_sub(st_t* st)
{
    assert(st != NULL);

    printf("execute_cmd: began sub\n");

    int a = 0;
    int b = 0;
    st_return_err got_a = st_pop(st, &a); // assert?
    st_return_err got_b = st_pop(st, &b);
    int res = b - a;

    if (got_a == no_error && got_b == no_error)
    {
        st_return_err pushed = st_push(st, res);
        if (pushed == no_error)
            printf("execute_cmd: sub succeeded, result = %d\n", res);
        else
            printf("execute_cmd: sub failed (failed to push the result)\n");
    }
    else
    {
        printf("execute_cmd: sub failed (not enough data in stack)\n");
    }
}


void calc_mult(st_t* st)
{
    assert(st != NULL);

    printf("execute_cmd: began mult\n");

    int a = 0;
    int b = 0;
    st_return_err got_a = st_pop(st, &a); // assert?
    st_return_err got_b = st_pop(st, &b);
    int res = a * b;

    if (got_a == no_error && got_b == no_error)
    {
        st_return_err pushed = st_push(st, res);
        if (pushed == no_error)
            printf("execute_cmd: mult succeeded, result = %d\n", res);
        else
            printf("execute_cmd: mult failed (failed to push the result)\n");
    }
    else
    {
        printf("execute_cmd: mult failed (not enough data in stack)\n");
    }
}


void calc_div(st_t* st)
{
    assert(st != NULL);

    printf("execute_cmd: began div\n");

    int a = 0;
    int b = 0;

    st_return_err got_a = st_pop(st, &a); // assert?
    st_return_err got_b = st_pop(st, &b);
    int res = b / a;

    if (got_a == no_error && got_b == no_error)
    {
        st_return_err pushed = st_push(st, res);
        if (pushed == no_error)
            printf("execute_cmd: div succeeded, result = %d\n", res);
        else
            printf("execute_cmd: div failed (failed to push the result)\n");
    }
    else
    {
        printf("execute_cmd: div failed (not enough data in stack)\n");
    }
}


void calc_out(st_t* st)
{
    assert(st != NULL);

    printf("execute_cmd: began out\n");

    int el = 0;
    st_return_err got_el = st_pop(st, &el);

    if (got_el == no_error)
    {
        printf("%d\n", el);
        printf("execute_cmd: out succeeded\n");
    }
    else
    {
        printf("execute_cmd: out failed (not enough data in stack)\n");
    }
}


void calc_hlt(st_t* st)
{
    assert(st != NULL);

    printf("execute_cmd: began hlt\n");

    st_return_err terminated = st_dtor(st);

    if (terminated == no_error)
    {
        printf("execute_cmd: stack destroyed, hlt succeeded\n");
    }
    else
    {
        printf("execute_cmd: hlt failed\n");
    }
}


// err_t read_number(int* number)
// {
//     assert(number != NULL);
//
//     int success = scanf("%d", number);
//
//     if (success == 1)
//     {
//         printf("read_number: got number: %d\n", *number);
//         return ok;
//     }
//     else
//     {
//         printf("read_number: failed to get number\n");
//         clear_buffer();
//         return wrong_number;
//     }
// }


// void clear_buffer(void)
// {
//     int c = '\0';
//     while ((c = getchar()) != '\n' && c != EOF);
// }
