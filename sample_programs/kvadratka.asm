; KVADRATKA

; ax^2 + bx + c = 0
; 8 - inf number of roots
; -1 no roots


; input

IN
POPREG AX
IN
POPREG BX
IN
POPREG CX


PUSHREG AX      ; if a == 0
PUSH 0
JE :0

PUSHREG BX      ; a != 0 started calculating discriminant
PUSHREG BX
MULT
PUSH 4
PUSHREG AX
PUSHREG CX
MULT
MULT
SUB             ; finished calculating discriminant
POPREG DX

DMP             ; delete it

PUSHREG DX
PUSH 0
JA :3           ; if D > 0

PUSHREG DX      ; D <= 0
PUSH 0
JE :4           ; if D == 0

PUSH -1         ; D < 0
OUT             ; no roots
HLT


:0              ; case a == 0
PUSHREG BX      ; if b == 0
PUSH 0
JE :1

PUSH -1         ; b != 0
PUSHREG CX
PUSHREG BX
DIV
MULT
OUT             ; x = -c/b
HLT


:1              ; case a == 0 b == 0
PUSHREG CX      ; if c == 0
PUSH 0
JE :2

PUSH -1         ; c != 0
OUT             ; no roots
HLT


:2              ; case a == 0 b == 0 c == 0
PUSH 8          ; infinite number of roots
OUT
HLT


:3              ; case D > 0

PUSH -1         ; root 1
PUSHREG BX
MULT
PUSHREG DX
SQRT
SUB
PUSHREG AX
PUSH 2
MULT
DIV

PUSH -1         ; root 2
PUSHREG BX
MULT
PUSHREG DX
SQRT
ADD
PUSHREG AX
PUSH 2
MULT
DIV

OUT
OUT
HLT


:4              ; case D == 0

PUSH -1         ; single root
PUSHREG BX
MULT
PUSH 2
PUSHREG AX
MULT
DIV
OUT
HLT
