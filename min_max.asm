IN              ; begin input
POPREG AX
IN
POPREG BX
IN
POPREG CX      ; done input

PUSHREG AX
PUSHREG BX
PUSHREG CX
JA :0           ; if B > C

PUSHREG CX
JA :1           ; if A > C

PUSHREG CX
OUT
HLT

:0
PUSHREG BX
JA :1           ; if A > B

PUSHREG BX
OUT
HLT

:1
PUSHREG AX
OUT
HLT


