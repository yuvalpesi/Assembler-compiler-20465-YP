
/**
 * Enumeration defining the operation names for assembly instructions.
 */
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

/**
 * Enumeration defining the addressing-relocation-external (ARE) types.
 */
enum are{
    A=0,
    R=2,
    E=1
};

/**
 * Enumeration defining the addressing methods for operands in assembly instructions.
 */
enum AddressingMethods{
    immediateAddressing=1,
    directAddressing=3,
    registerDirectAddressing=5
};

/**
 * This function reads the assembly source file and performs the first pass of the assembly process.
 * It processes each line of the source file, checks for syntax errors, handles labels and directives,
 * and generates the binary representation of the commands and operands.
 *
 * @param argv: The name of the assembly source file.
 * @param nodeHead: A pointer to the head of the LineHolder linked list.
 * @param symbolTabl: A pointer to the symbol table.
 * @param IC: A pointer to the instruction counter.
 * @param DC: A pointer to the data counter.
 * @return: True if the first pass is successful and no errors are encountered, False otherwise.
 */
int firstPass(char *argv,LineHolder **nodeHead,symbolTable *symbolTabl,int *IC,int*DC);

/**
 * Determines the type of a command based on the given string.
 * Checks if the string matches any predefined functions and returns the corresponding type.
 *
 * @param str A pointer to the string representing the command.
 * @return The type of the command if it matches a predefined function, or ERROR if no match is found.
 */
int commandType(char *str);

/**
 * This function installs a binary representation of an operand based on its type and values.
 * It allocates memory for the Operand structure and its corresponding fields depending on
 * the type of the operand (directive, command, immediate, register, or address).
 * The function returns a pointer to the installed Operand structure.
 *
 * @param name: The name of the operand.
 * @param opcode: The opcode value.
 * @param source: The source operand value.
 * @param target: The target operand value.
 * @param ARE: The ARE value.
 * @return: A pointer to the installed Operand structure.
 */
Operand *instalBinary(char *name,unsigned int opcode,unsigned int source,unsigned int target,unsigned int ARE);

/**
 * This function frees the memory allocated for an Operand structure and its corresponding fields.
 * The memory is freed based on the type of the operand (directive, command, immediate, register, or address).
 *
 * @param temp: A pointer to the Operand structure to be freed.
 */
void freeBinery(Operand *temp);

/**
 * Determines the register type based on the given string.
 * Returns a register number to the corresponding register.
 *
 * @param str The string representing the register number.
 * @return A integer number of the register number corresponding to the given string.
 *         Returns ERROR (-1) if the string doesn't match any register number.
 */
int checkRegister(char *str);

/**
 * This function extracts the label name from the given string. The label name is
 * defined as the substring from the beginning of the string up to the first occurrence
 * of either a colon (':') character, a space character (' '), or a newline character ('\n').
 * The function dynamically allocates memory to store the extracted substring and returns it.
 *
 * @param str: The input string from which the label name is to be extracted.
 * @return: A pointer to the extracted label name substring, or NULL if no label name is found.
 */
char *labelName(char *str);

/**
 * This function checks if the given string is a valid label for a command.
 * It verifies that the first character is an alphabet character and
 * the subsequent characters (up to a maximum length) are alphanumeric.
 *
 * @param str: The input string to be checked as a command label.
 * @return: Returns 1 (True) if the string is a valid command label, and 0 (False) otherwise.
 */
int checkLabelInCommand(char *str);

/**
 * Handles string command, which processes and adds string data to the data memory.
 *
 * @param Line The line containing the data command.
 * @param sourceFile The name of the source file being processed.
 * @param lineNumber The line number of the data command in the source file.
 * @param errorCounter Pointer to the error counter, used to track the number of errors encountered.
 * @param DC Pointer to the data counter, used to keep track of the current position in the data memory.
 * @param head Pointer to the head of the linked list that holds the data memory.
 */
void handleString(char *line, char *sourceFile, int lineNumber, int *errorCounter,int *DC,LineHolder **head);

/**
 * Handles data command, which processes and adds data values to the data memory.
 *
 * @param Line The line containing the data command.
 * @param sourceFile The name of the source file being processed.
 * @param lineNumber The line number of the data command in the source file.
 * @param errorCounter Pointer to the error counter, used to track the number of errors encountered.
 * @param DC Pointer to the data counter, used to keep track of the current position in the data memory.
 * @param head Pointer to the head of the linked list that holds the data memory.
 */
void handleData(char *line, char *sourceFile, int lineNumber, int *errorCounter,int *DC,LineHolder **head);

/**
 * Handles the entry command, which marks and adds a symbol as an entry point in the symbol table.
 *
 * @param Line The line containing the entry command.
 * @param sourceFile The name of the source file being processed.
 * @param lineNumber The line number of the entry command in the source file.
 * @param errorCounter Pointer to the error counter, used to track the number of errors encountered.
 * @param head Pointer to the head of the linked list that holds the program's code and data memory.
 * @param table Pointer to the symbol table to store and manage symbols.
 */
void handleEntry(char *line, char *sourceFile, int lineNumber, int *errorCounter,LineHolder **head,symbolTable *table);

/**
 * Handles the extern command, which marks and adds a symbol as an extern point in the symbol table.
 *
 * @param Line The line containing the entry command.
 * @param sourceFile The name of the source file being processed.
 * @param lineNumber The line number of the entry command in the source file.
 * @param errorCounter Pointer to the error counter, used to track the number of errors encountered.
 * @param head Pointer to the head of the linked list that holds the program's code and data memory.
 * @param table Pointer to the symbol table to store and manage symbols.
 */
void handleExtern(char *line, char *sourceFile, int lineNumber, int *errorCounter,LineHolder **head,symbolTable *table);

/**
 * Handles no-operand instructions (commands that require no operands) and generates their machine code.
 *
 * @param command The opcode of the instruction.
 * @param function The function name of the instruction.
 * @param Line The line containing the instruction.
 * @param sourceFile The name of the source file being processed.
 * @param lineNumber The line number of the instruction in the source file.
 * @param errorCounter Pointer to the error counter, used to track the number of errors encountered.
 * @param IC Pointer to the instruction counter, used to keep track of the memory address for the current instruction.
 * @param head Pointer to the head of the linked list that holds the program's code and data memory.
 */
void handleNoOperand(int command,char *function, char *line, char *sourceFile, int lineNumber, int *errorCounter, int* IC,LineHolder **head);

/**
 * Handles one-operand instructions (commands that require one operands) and generates their machine code.
 *
 * @param command The opcode of the instruction.
 * @param function The function name of the instruction.
 * @param Line The line containing the instruction.
 * @param sourceFile The name of the source file being processed.
 * @param lineNumber The line number of the instruction in the source file.
 * @param errorCounter Pointer to the error counter, used to track the number of errors encountered.
 * @param IC Pointer to the instruction counter, used to keep track of the memory address for the current instruction.
 * @param head Pointer to the head of the linked list that holds the program's code and data memory.
 */
void handleOneOperand(int command,char *function, char *Line, char *sourceFile, int lineNumber, int *errorCounter, int* IC,LineHolder **head);

/**
 * Handles two-operand instructions (commands that require two operands) and generates their machine code.
 *
 * @param command The opcode of the instruction.
 * @param function The function name of the instruction.
 * @param Line The line containing the instruction.
 * @param sourceFile The name of the source file being processed.
 * @param lineNumber The line number of the instruction in the source file.
 * @param errorCounter Pointer to the error counter, used to track the number of errors encountered.
 * @param IC Pointer to the instruction counter, used to keep track of the memory address for the current instruction.
 * @param head Pointer to the head of the linked list that holds the program's code and data memory.
 */
void handleTowOperand(int command,char *function, char *Line, char *sourceFile, int lineNumber, int *errorCounter, int* IC,LineHolder **head);
