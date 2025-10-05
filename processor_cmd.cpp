#include <math.h>

#include "stack/stack_dump.h"
#include "processor_cmd.h"

err_t proc_push(st_t* st, int* code, size_t* ip)
{
    assert(code != NULL);

    printf("execute_cmd: began push\n");
    int number = code[++*ip];
    st_return_err pushed = st_push(st, number);

    if (pushed == no_error)
    {
        st_dump(st);
        printf("execute_cmd: push succeeded\n");
        return ok;
    }
    else
    {
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " push failed\n");
        return error;
    }
}


err_t proc_calc(st_t* st, proc_commands cmd)
{
    assert(st != NULL);

    const char* cmd_name = decode_cmd(cmd);
    printf("execute_cmd: began %s \n", cmd_name);

    int a = 0;
    int b = 0;
    st_return_err got_a = no_error;
    st_return_err got_b = no_error;
    int res = 0;

    switch(cmd)
    {
        case ADD:
            got_a = st_pop(st, &a);
            got_b = st_pop(st, &b);
            res = a + b;
            break;

        case SUB:
            got_a = st_pop(st, &a);
            got_b = st_pop(st, &b);
            res = b - a;
            break;

        case MULT:
            got_a = st_pop(st, &a);
            got_b = st_pop(st, &b);
            res = a * b;
            break;

        case DIV:
            got_a = st_pop(st, &a);
            got_b = st_pop(st, &b);

            if (a == 0)
            {
                printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " div failed - division by zero\n");
                return error;
            }

            res = b / a;
            break;

        case SQRT:
            got_a = st_pop(st, &a);

            if (a < 0)
            {
                printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " sqrt failed - cannot determine root of negative number\n");
                return error;
            }

            res = sqrt(a);
            break;
    }

    if (got_a == no_error && got_b == no_error)
    {
        st_return_err pushed = st_push(st, res);
        if (pushed == no_error)
        {
            printf("execute_cmd: %s succeeded, result = %d\n", cmd_name, res);
            return ok;
        }
        else
        {
            printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " %s failed (failed to push the result)\n", cmd_name);
            return error;
        }
    }
    else
    {
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " %s failed (not enough data in stack)\n", cmd_name);
        return error;
    }
}


const char* decode_cmd (proc_commands cmd)
{
    switch (cmd)
    {
        case ADD:
            return "add";
            break;
        case SUB:
            return "sub";
            break;
        case MULT:
            return "mult";
            break;
        case DIV:
            return "div";
            break;
        case SQRT:
            return "sqrt";
            break;
        default:
            return "unknown command";
            break;
    }
}

err_t proc_out(st_t* st)
{
    assert(st != NULL);

    printf("execute_cmd: began out\n");

    int el = 0;
    st_return_err got_el = st_pop(st, &el);

    if (got_el == no_error)
    {
        printf("%d\n", el);
        printf("execute_cmd: out succeeded\n");
        return ok;
    }
    else
    {
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " out failed (not enough data in stack)\n");
        return error;
    }
}


err_t proc_hlt(st_t* st)
{
    assert(st != NULL);

    printf("execute_cmd: began hlt\n");

    st_return_err terminated = st_dtor(st);

    if (terminated == no_error)
    {
        printf("execute_cmd: stack destroyed, hlt succeeded\n");
        return ok;
    }
    else
    {
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " hlt failed\n");
        return error;
    }
}



