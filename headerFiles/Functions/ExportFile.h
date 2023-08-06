
void printObjFile(char *argv,LineHolder *head,int IC,int DC);

/**
 * Prints the entry symbols and their corresponding addresses to an entry symbols file.
 *
 * @param argv Pointer to a string representing the name of the input assembly file.
 * @param head Pointer to the head of the linked list containing the assembly lines.
 */
void printEntFile(char *argv,EnNode *head);

/**
 * Prints the extern symbols and their corresponding addresses to an extern symbols file.
 *
 * @param argv Pointer to a string representing the name of the input assembly file.
 * @param head Pointer to the head of the linked list containing the assembly lines.
 */
void printExtFile(char *argv,ExNode *head);

/**
 * Prints the object code, using Base64 encoding, to the object file.
 *
 * @param argv Pointer to a string representing the name of the input assembly file.
 * @param head Pointer to the head of the linked list containing the assembly lines.
 * @param IC   The value of the instruction counter (IC) indicating the start address of the code.
 * @param DC   The value of the data counter (DC) indicating the start address of the data.
 */
void printObjFileBase64(char *argv,LineHolder *head,int IC,int DC);

/**
 * Converts a binary string to a Base64-encoded string.
 *
 * @param binary Pointer to a null-terminated string representing the binary data.
 * @return Pointer to a dynamically allocated null-terminated string containing the Base64-encoded data.
 *         It is the responsibility of the caller to free the memory allocated for the returned string.
 */
char *binaryToBase64(const char *binary);
