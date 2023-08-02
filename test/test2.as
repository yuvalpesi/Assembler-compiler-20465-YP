; file ps.as
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
    mcro m2
sub  @r1,@r0
    prn 6
endmcro
bne  L3
L1:     inc  K
    .entry LOOP
jmp  W
END:    stop
STR:    .string "abcdef"
LENGTH: .data 6,-9,15
m1
K:      .data 22
m2
    .extern L3
