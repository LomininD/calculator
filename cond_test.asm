PUSH 1
PUSH 2
JB 6        ; goes to next instruction
PUSH 1
PUSH 2
JA 0        ; cond not fulfilled
PUSH 2
PUSH 0
JB 2        ; cond not fulfilled
PUSH 2
PUSH 0
JBE 2       ; cond not fulfilled
PUSH 5
PUSH 5
JAE 31      ; skips next instruction
ADD
HLT
