
enum operationName{
    mov,
    cmp,
    add,
    sub,
    not,
    clr,
    lea,
    inc,
    dec,
    jmp,
    bne,
    red,
    prn,
    jsr,
    rts,
    stop
};

enum are{
    A=0,
    R=2,
    E=1
};

enum AddressingMethods{
    immediateAddressing=1,
    directAddressing=3,
    registerDirectAddressing=5
};


int firstPass(char *argv,LineHolder **nodeHead,symbolTable *symbolTabl,int *IC,int*DC);

int commandType(char *str);

Operand *instalBinary(Operand *temp,char *name,unsigned int opcode,unsigned int source,unsigned int target,unsigned int ARE);

void freeBinery(Operand *temp);

int checkRegister(char *str);

int checkLabel(char *str);

char *labelName(char *str);

