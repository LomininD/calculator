IN
POPREG AX

PUSHREG AX
PUSH 0
JB :3           ; if number is negative

CALL :1
PUSHREG AX
OUT
HLT


:3              ; number is negative

PUSH -1
OUT
HLT


:1

PUSHREG AX
PUSH 1
JBE :2


PUSHREG AX      ; save value
POPREG BX

PUSHREG AX      ; decrement value and call function
PUSH 1
SUB
POPREG AX
CALL :1

PUSHREG AX      ; multiply new value and previous valaue
PUSHREG BX
MULT
POPREG AX

PUSHREG BX      ; restore saved value
PUSH 1
ADD
POPREG BX
RET


:2              ; exit from recursion
PUSH 1
POPREG AX
RET

