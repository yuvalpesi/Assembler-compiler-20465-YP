
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
