IN              ; begin input
PUSHREG AX
IN
PUSHREG BX
IN
PUSHREG CX      ; done input

POPREG AX
POPREG BX
POPREG CX

JA 26           ; if B > C
POPREG CX       ; C >= B
JA 35           ; if A > C
POPREG CX       ; C >= A
OUT             ; prints c
JMP 38

POPREG BX       ; if B > C jump here
JA 315          ; if A > B
POPREG BX       ; B > A
OUT
JMP 38

POPREG AX       ; if A > B jump here
OUT


HLT


