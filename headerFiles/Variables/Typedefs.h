typedef enum{
    True=1,False=0
}boolean;

typedef struct firstOp{
    unsigned int ARE:2;
    unsigned int targetOperand:3;
    unsigned int opcode:4;
    unsigned int sourceOperand:3;

}firstOp;

typedef struct DirectImmediateOp{
    unsigned int ARE:2;
    unsigned int Operand:10;
}DirectImmediate;

typedef struct DirectAddressOp{
    unsigned int ARE:2;
    unsigned int Operand:10;
}DirectAddress;

typedef struct DirectRegisterAddressOp{
    unsigned int ARE:2;
    unsigned int FirstRed:5;
    unsigned int SecRed:5;
}DirectRegisterAddress;

typedef struct DirectiveSentence{
    unsigned int directive:12;
}DirectiveSentence;

typedef struct Operand{
    char *lableName;
    union {
        firstOp *first;
        DirectImmediate *immediate;
        DirectAddress *address;
        DirectRegisterAddress *registerAddress;
        DirectiveSentence *directiveSentence;
    }operandStucter;
}Operand;


