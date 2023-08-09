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
 * @param table Pointer to the symbol table to store and manage symbols.
 */
void handleEntry(char *line, char *sourceFile, int lineNumber, int *errorCounter,symbolTable *table);

/**
 * Handles the extern command, which marks and adds a symbol as an extern point in the symbol table.
 *
 * @param Line The line containing the entry command.
 * @param sourceFile The name of the source file being processed.
 * @param lineNumber The line number of the entry command in the source file.
 * @param errorCounter Pointer to the error counter, used to track the number of errors encountered.
 * @param table Pointer to the symbol table to store and manage symbols.
 */
void handleExtern(char *line, char *sourceFile, int lineNumber, int *errorCounter,symbolTable *table);
