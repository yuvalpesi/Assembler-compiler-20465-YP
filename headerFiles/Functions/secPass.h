
/**
 * This function performs the second pass of the assembly process. It iterates over each line
 * of the assembly code generated in the first pass, resolves symbols, and generates the final
 * binary representation of the commands and operands. It also checks for errors and generates
 * output files for entry and extern labels.
 *
 * @param argv: The name of the assembly source file.
 * @param head: The head of the LineHolder linked list containing the binary representation of the code.
 * @param symbol: The symbol table.
 * @param IC: The instruction counter value.
 * @param DC: The data counter value.
 * @return: True if the second pass is successful and no errors are encountered, False otherwise.
 */
int secPass(char *argv,LineHolder *head, symbolTable *symbol,int IC,int DC,ExNode **Extern,EnNode **Entry);

/**
 * This function checks the symbol table for symbols marked as entry symbols (type sENTRY) and
 * creates a linked list of entries with their corresponding addresses. The linked list is
 * represented by the LineHolder structure. The address of each entry is calculated based on the
 * symbol's address in the symbol table and the value of IC. The function returns Entry linked node if at least
 * one entry is found, and NULL otherwise.
 * if the function find a entry that not in use it return error and add to the error flag one
 *
 * @param Entry: A entry linked list pointer.
 * @param argv: A pointer to the file name.
 * @param table: The symbol table.
 * @param head: A pointer to the head of the LineHolder linked list (output parameter).
 * @param IC: The current value of the instruction counter (IC).
 * @param errorsCounter: An errors counter flag
 */
void checkEntry(EnNode **Entry,char *argv,symbolTable *table,int IC,int *errorsCounter);

/**
 * This function checks the symbol table for symbols marked as external symbols (type sEXETRN) that
 * are referenced in the given LineHolder linked list. It creates a linked list of externals with
 * their corresponding addresses. The linked list is represented by the LineHolder structure. The
 * function returns Extern linked node if at least one external symbol is found, and NULL otherwise.
 * if the error flag is not 0 it will return empty extern node
 *
 * @param Extern: A extern linked list pointer.
 * @param table: The symbol table.
 * @param curr: The current LineHolder node being processed.
 * @param errorsCounter: An errors counter flag
 */
void checkExtern(ExNode **Extern,symbolTable *table,LineHolder **curr,int errorsCounter);
