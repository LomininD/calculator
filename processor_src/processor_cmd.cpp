#include <math.h>

#include "stack/stack_dump.h"
#include "processor_cmd.h"


static int check_buffer();
static void clear_buffer(void);


err_t proc_push(proc_info* proc)
{
    assert(proc != NULL);

    printf("execute_cmd: began push\n");
    int number = proc->code[++proc->ip];
    st_return_err pushed = st_push(&proc->st, number);

    if (pushed == no_error)
    {
        st_dump(&proc->st);
        printf("execute_cmd: push succeeded\n");
        proc->ip++;
        return ok;
    }
    else
    {
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " push failed\n");
        return error;
    }
}


err_t proc_pushreg(proc_info* proc)
{
    assert(proc != NULL);

    printf("execute_cmd: began pushreg\n");

    int reg = proc->code[++proc->ip];

    if (reg < 0 || reg >= register_amount)
    {
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " PUSHREG failed (got non-existent register index %d)\n", reg);
        return error;
    }

    int number = 0;
    st_return_err got_number = st_pop(&proc->st, &number);

    if (got_number != no_error)
    {
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " PUSHREG failed (not enough data in stack)\n");
        return error;
    }

    proc->registers[reg] = number;
    printf("execute_cmd: PUSHREG succeeded\n");
    st_dump(&proc->st);
    proc->ip++;

    return ok;
}


err_t proc_popreg(proc_info* proc)
{
    assert(proc != NULL);

    printf("execute_cmd: began popreg\n");

    int reg = proc->code[++proc->ip];

    if (reg < 0 || reg >= register_amount)
    {
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " POPREG failed (got non-existent register index %d)\n", reg);
        return error;
    }

    st_return_err pushed = st_push(&proc->st, proc->registers[reg]);

    if (pushed != no_error)
    {
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " POPREG failed (could not push data to stack)\n");
        return error;
    }

    printf("execute_cmd: POPREG succeeded\n");
    st_dump(&proc->st);
    proc->ip++;

    return ok;
}


err_t proc_calc(proc_info* proc, proc_commands cmd)
{
    assert(proc != NULL);

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
            got_a = st_pop(&proc->st, &a);
            got_b = st_pop(&proc->st, &b);
            res = a + b;
            break;

        case SUB:
            got_a = st_pop(&proc->st, &a);
            got_b = st_pop(&proc->st, &b);
            res = b - a;
            break;

        case MULT:
            got_a = st_pop(&proc->st, &a);
            got_b = st_pop(&proc->st, &b);
            res = a * b;
            break;

        case DIV:
            got_a = st_pop(&proc->st, &a);
            got_b = st_pop(&proc->st, &b);

            if (a == 0)
            {
                printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " div failed - division by zero\n");
                return error;
            }

            res = b / a;
            break;

        case SQRT:
            got_a = st_pop(&proc->st, &a);

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
        st_return_err pushed = st_push(&proc->st, res);
        if (pushed == no_error)
        {
            st_dump(&proc->st);
            printf("execute_cmd: %s succeeded, result = %d\n", cmd_name, res);
            proc->ip++;
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


err_t proc_in(proc_info* proc)
{
    assert(proc != NULL);

    printf("execute_cmd: began in\n");

    int inp = 0;
    int scanned = scanf("%d", &inp);
    printf("scanned number  = %d\n", inp);
    if (scanned != 1 || !check_buffer()) // -1 if big numbers
    {
        clear_buffer();
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " in failed (could not get value from input stream)\n");
        return error;
    }

    st_return_err pushed = st_push(&proc->st, inp);

    if (pushed == no_error)
    {
        st_dump(&proc->st);
        printf("execute_cmd: in succeeded\n");
        proc->ip++;
        return ok;
    }
    else
    {
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " in failed (could not push read value to stack)\n");
        return error;
    }
}


err_t proc_out(proc_info* proc)
{
    assert(proc != NULL);

    printf("execute_cmd: began out\n");

    int el = 0;
    st_return_err got_el = st_pop(&proc->st, &el);

    if (got_el == no_error)
    {
        printf("%d\n", el);
        printf("execute_cmd: out succeeded\n");
        proc->ip++;
        return ok;
    }
    else
    {
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " out failed (not enough data in stack)\n");
        return error;
    }
}


err_t proc_jmp(proc_info* proc)
{
    assert(proc != NULL);

    printf("execute_cmd: began jmp\n");

    size_t new_pointer = proc->code[++proc->ip];
    proc->ip = new_pointer;

    if (new_pointer >= proc->prg_size || new_pointer < 0)
    {
        printf("proc_jmp: " MAKE_BOLD_RED("ERROR:") " new IP points on a non-existed position in code\n");
        return error;
    }

    printf("execute_cmd: done jmp (new_position = %zu)\n", new_pointer);
    return ok;
}


err_t proc_cond_jmp(proc_info* proc, proc_commands cmd)
{
    assert(proc != NULL);

    const char* cmd_name = decode_cmd(cmd);

    printf("execute_cmd: began conditional jmp (%s)\n", cmd_name);

    int a = 0;
    int b = 0;
    st_return_err got_a = st_pop(&proc->st, &a);
    st_return_err got_b = st_pop(&proc->st, &b);

    if (got_a != no_error || got_b != no_error)
    {
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " %s failed (not enough data in stack)\n", cmd_name);
        return error;
    }

    bool is_fulfilled = check_condition(cmd, a, b);

    if (is_fulfilled)
    {
        printf("proc_cond_jmp: condition is fulfilled\n");
        err_t jumped = proc_jmp(proc);
        if (jumped != ok)
            return error;
    }
    else
    {
        printf("proc_cond_jmp: condition is not fulfilled\n");
        proc->ip += 2;
    }

    printf("execute_cmd: done conditional jmp (%s)\n", cmd_name);
    return ok;
}


bool check_condition(proc_commands cmd, int a, int b)
{
    switch(cmd)
    {
        case JB:
            return b < a;
            break;
        case JBE:
            return b <= a;
            break;
        case JA:
            return b > a;
            break;
        case JAE:
            return b >= a;
            break;
        case JE:
            return b == a;
            break;
        case JNE:
            return b != a;
            break;
        default:
            return false;
            break;
    };
}


err_t proc_hlt(proc_info* proc)
{
    assert(proc != NULL);

    printf("execute_cmd: began hlt\n");

    st_return_err terminated = st_dtor(&proc->st);

    if (terminated == no_error)
    {
        printf("execute_cmd: stack destroyed, hlt succeeded\n");
        proc->ip++;
        return ok;
    }
    else
    {
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " hlt failed\n");
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


static int check_buffer()
{
    int c = 0;
    while ((c = getchar()) != EOF && c != '\n')
        if (c != ' ' && c != '\n' && c != '\t')
            return 0;
    return 1;
}


static void clear_buffer(void)
{
    int c = '\0';
    while ((c = getchar()) != '\n' && c != EOF);
}



