#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

#include "calculator_cmd.h"
#include "stack/stack.h"
#include "stack/font_styles.h"
#include "stack/stack_dump.h"

void execute_cmd(st_t* st, int* code, int* ip, calc_commands cmd);
st_t initialise_stack(size_t capacity);

void calc_push(st_t* st, int* code, int* ip);
void calc_add(st_t* st);
void calc_sub(st_t* st);
void calc_mult(st_t* st);
void calc_div(st_t* st);
void calc_sqrt(st_t* st);
void calc_out(st_t* st);
void calc_hlt(st_t* st);


//TODO: split output streams
//TODO: normal push input

int main()
{
    printf(MAKE_BOLD("+++ PROCESSOR +++\n"));

    size_t capacity = 10;
    st_t st = initialise_stack(capacity);

    FILE* fp = fopen("program.out", "r");

    int code[50] = {};
    int cmd = 0;
    int i = 0;
    int scanned = 0;

    while (true)
    {
        scanned = fscanf(fp, "%d", &cmd);

        //printf("scanned_cmd: %d\n", cmd);

        if (scanned == -1)
            break;

        code[i] = cmd;

        i++;
    }


    int ip = 0;
    calc_commands current_cmd = UNKNOWN;

    while ((current_cmd = (calc_commands) code[ip]) != HLT)
    {
        printf("current_cmd = %d\n", current_cmd);

        execute_cmd(&st, code, &ip, current_cmd);

        getchar();

        ip++;
    }

    printf("main: shutting down calculator\n");
    return 0;


    // printf("process terminated manually, no HLT got\n");
    // return 0;
}


void execute_cmd(st_t* st, int* code, int* ip, calc_commands cmd)
{
    assert(st != NULL);

    switch (cmd)
    {
        case PUSH:
            calc_push(st, code, ip);
            break;

        case ADD:
            calc_add(st);
            break;

        case SUB:
            calc_sub(st);
            break;

        case MULT:
            calc_mult(st);
            break;

        case DIV:
            calc_div(st);
            break;

        case SQRT:
            calc_sqrt(st);
            break;

        case OUT:
            calc_out(st);
            break;

        case HLT:
            calc_hlt(st);
            break;

        default:
            printf("execute_cmd: unknown command (cmd = %d, ip = %d), cannot execute\n", cmd, *ip);
    }
}


st_t initialise_stack(size_t capacity)
{
    st_t st = {};
    st_return_err created = st_ctor(&st, capacity);

    if (created == no_error)
        printf("initialise_stack: stack created\n");
    else
        printf("initialise_stack: problem occurred while creating stack\n");

    return st;
}


void calc_push(st_t* st, int* code, int* ip)
{
    assert(code != NULL);
    assert(code != ip);

    printf("execute_cmd: began push\n");
    int number = code[++*ip];

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


void calc_sqrt(st_t* st)
{
    assert(st != NULL);

    printf("execute_cmd: began sqrt\n");

    int a = 0;
    st_return_err got_a = st_pop(st, &a);
    int res = sqrt(a);

    if (got_a == no_error)
    {
        st_return_err pushed = st_push(st, res);
        if (pushed == no_error)
            printf("execute_cmd: sqrt succeeded, result = %d\n", res);
        else
            printf("execute_cmd: sqrt failed (failed to push the result)\n");
    }
    else
    {
        printf("execute_cmd: sqrt failed (not enough data in stack)\n");
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



