#include <math.h>

#include "stack/stack_dump.h"
#include "processor_cmd.h"

err_t proc_push(st_t* st, int* code, size_t* ip)
{

    assert(st != NULL);
    assert(code != NULL);
    assert(ip != NULL);

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
            st_dump(st);
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
        case JB:
            return "JB";
            break;
        case JBE:
            return "JBE";
            break;
        case JA:
            return "JA";
            break;
        case JAE:
            return "JAE";
            break;
        case JE:
            return "JE";
            break;
        case JNE:
            return "JNE";
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


err_t proc_jmp(st_t* st, int* code, int prg_size, size_t* ip)
{
    assert(st != NULL);
    assert(code != NULL);
    assert(ip != NULL);

    printf("execute_cmd: began jmp\n");

    size_t new_pointer = code[++*ip];
    *ip = new_pointer;

    if (new_pointer >= prg_size)
    {
        printf("proc_jmp: " MAKE_BOLD_RED("ERROR:") " new IP points on a non-existed position in code\n");
        return error;
    }

    printf("execute_cmd: done jmp (new_position = %zu)\n", new_pointer);
    return ok;
}


err_t proc_cond_jmp(st_t* st, int* code, int prg_size, size_t* ip, proc_commands cmd)
{
    assert(st != NULL);
    assert(code != NULL);
    assert(ip != NULL);

    const char* cmd_name = decode_cmd(cmd);

    printf("execute_cmd: began conditional jmp (%s)\n", cmd_name);

    int a = 0;
    int b = 0;
    st_return_err got_a = st_pop(st, &a);
    st_return_err got_b = st_pop(st, &b);

    if (got_a != no_error || got_b != no_error)
    {
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " %s failed (not enough data in stack)\n", cmd_name);
        return error;
    }

    int res = 0;

    switch(cmd)
    {
        case JB:
            res = b < a;
            break;
        case JBE:
            res = b <= a;
            break;
        case JA:
            res = b > a;
            break;
        case JAE:
            res = b >= a;
            break;
        case JE:
            res = b == a;
            break;
        case JNE:
            res = b != a;
            break;
        default:
            printf("proc_cond_jmp: could not recognize type of conditional jump\n");
            return error;
    };

    if (res)
    {
        printf("proc_cond_jmp: condition is fulfilled\n");
        err_t jumped = proc_jmp(st, code, prg_size, ip);
        if (jumped != ok)
            return error;

    }
    else
    {
        printf("proc_cond_jmp: condition is not fulfilled\n");
        *ip += 2;
    }

    printf("execute_cmd: done conditional jmp (%s)\n", cmd_name);
    return ok;
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



