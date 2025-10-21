; AX = x
; BX = y
; CX = conter
; DX = r
; EX = dx
; FX = dy
; GX - SCREEN WIDTH
; HX - SCREEN HIGHT

PUSH 127
POPREG GX       ; screen width
PUSH 78
POPREG HX       ; screen height

IN
POPREG DX       ; gets r
IN
POPREG EX       ; gets dx
IN
POPREG FX       ; gets dy

PUSH 0
POPREG CX       ; initialing counter

:1

PUSHREG CX
PUSHREG GX
DIV
POPREG BX       ; gets y

PUSHREG CX
PUSHREG BX
PUSHREG GX
MULT
SUB
POPREG AX       ; gets x

PUSHREG AX
PUSHREG EX
SUB
POPREG AX

PUSHREG AX
PUSHREG AX
MULT
POPREG AX


PUSHREG BX
PUSHREG FX
SUB
POPREG BX

PUSHREG BX
PUSHREG BX
MULT
POPREG BX


PUSHREG AX
PUSHREG BX
ADD
PUSHREG DX
PUSHREG DX
MULT
JBE :2          ; main check


PUSH 46         ; not passed '.'
POPM [CX]
JMP :3


:2              ;  passed '#'
PUSH 35
POPM [CX]

:3

DMPM

PUSHREG CX      ; increment counter
PUSH 1
ADD
POPREG CX


PUSHREG GX
PUSHREG HX
MULT
PUSHREG CX
JA :1

DMPM

DRAW
HLT
