; error in the mcro name
    .entry LENGTH
    .extern W
MAIN:   mov  @r3 ,LENGTH
LOOP:   jmp  L1
mcro mov
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
m2
END:    stop
STR:    .string "abcdef"
LENGTH: .data 6,-9,15
mov
K:      .data 22
    .extern L3
