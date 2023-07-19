
/**
 * Enumeration defining boolean values.
 */
typedef enum{
    True=1,False=0
}boolean;

/**
 * Bitfield structure defining the first operand in an assembly instruction.
 */
typedef struct firstOp{
    unsigned int ARE:2;
    unsigned int targetOperand:3;
    unsigned int opcode:4;
    unsigned int sourceOperand:3;
}firstOp;

/**
 * Bitfield structure defining a direct immediate operand in an assembly instruction.
 */
typedef struct DirectImmediateOp{
    unsigned int ARE:2;
    unsigned int Operand:10;
}DirectImmediate;

/**
 * Bitfield structure defining a direct address operand in an assembly instruction.
 */
typedef struct DirectAddressOp{
    unsigned int ARE:2;
    unsigned int Operand:10;
}DirectAddress;

/**
 * Bitfield structure defining a direct register address operand in an assembly instruction.
 */
typedef struct DirectRegisterAddressOp{
    unsigned int ARE:2;
    unsigned int FirstRed:5;
    unsigned int SecRed:5;
}DirectRegisterAddress;

/**
 * Bitfield structure defining a directive sentence in an assembly instruction.
 */
typedef struct DirectiveSentence{
    unsigned int directive:12;
}DirectiveSentence;

/**
 * Bitfield structure defining an operand in an assembly instruction.
 */
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

