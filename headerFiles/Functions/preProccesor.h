
/**
 * Performs preprocessing on the input assembly file, expanding macros and creating an expanded source code file.
 *
 * @param inputFileName Pointer to a string representing the name of the input assembly file.
 * @return True if preprocessing is successful, False otherwise.
 */
int preProccesor(char *inputFileName);

/**
 * Splits a string into an array of strings based on space delimiters.
 *
 * @param str     Pointer to a string to be split.
 * @param counter Pointer to an integer to store the number of words split from the string.
 * @return An array of strings containing the split words from the input string.
 *         Returns NULL if memory allocation fails or if the input string is empty.
 */
char **splitStringLine(char *str,int *counter);

/**
 * Frees the memory allocated for an array of strings.
 *
 * @param splitString Pointer to an array of strings.
 * @param counter     Number of elements in the array.
 */
void freeAllSplitString(char **splitString, int counter);
