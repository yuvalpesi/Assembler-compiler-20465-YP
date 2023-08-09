; error in the entry not in use 
    .entry notUse
    .entry LENGTH
    .extern W
MAIN:   mov  @r3 ,LENGTH
LOOP:   jmp  L1
mcro m1
sub  @r2,@r3
    bne END
endmcro
prn  -5
bne  W
sub  @r1,@r4
m1
bne  L3
L1:     inc  K
    .entry LOOP
jmp  W
END:    stop error
STR:    .string "abcdef"
LENGTH: .data 6,-9,15
K:      .data 22
    .extern L3
