
/**
 * Ignores leading whitespace characters in a given string.
 * Returns a pointer to the first non-whitespace character in the string.
 *
 * @param str A pointer to the string to be processed.
 * @return A pointer to the first non-whitespace character in the string.
 *         If the string contains only whitespace characters or is empty, it returns a pointer to the end of the string.
 */
char* ignorSpace(char *str);

/**
 * Removes all Quotation Marks from a given string.
 * Modifies the string in-place by removing the unwanted characters.
 *
 * @param str A pointer to the string to be modified.
 */
void RemoveQuotationMarks(char *str);

/**
 * Removes all spaces, tabs, and newlines from a given string.
 * Modifies the string in-place by removing the unwanted characters.
 * @param str A pointer to the string to be modified.
 */
void RemoveAllSpaces(char *str);

/**
 * strFirstArgu - Extracts the first argument from a string delimited by a comma.
 *
 * This function extracts the first argument from a string delimited by a comma.
 * It allocates memory to store the extracted substring and copies the characters until
 * the first comma or the end of the string is encountered.
 * The extracted substring is returned.
 *
 * @param str: The input string from which the first argument is to be extracted.
 * @return: A pointer to the extracted substring.
 */
char *strFirstArgu(char *str);

/**
 * allocateStr - Concatenates two strings and allocates memory for the result.
 *
 * This function concatenates two strings and allocates memory to store the resulting string.
 * The allocated memory is the sum of the lengths of the two input strings plus 2 (for null terminator).
 * The function dynamically allocates memory and copies the concatenated string into it.
 * The concatenated string is returned.
 *
 * @param strFirst: The first input string.
 * @param strSec: The second input string.
 * @return: A pointer to the concatenated string.
 */
char *allocateStr(char *strFirst, char* strSec);

/**
 * allDigits - Checks if a string contains only digits.
 *
 * This function checks if the given string contains only numeric digits.
 * It iterates through each character in the string starting from the specified index,
 * and returns False if a non-digit character is encountered. If all characters in the
 * string are digits, it returns True.
 *
 * @param str: The input string to be checked.
 * @return: Returns 1 (True) if the string contains only digits, and 0 (False) otherwise.
 */
int allDigits(char *str);

/**
 * strDup - Creates a duplicate of a given string.
 *
 * This function creates a duplicate of the given string by allocating memory and copying
 * the contents of the original string. The duplicate string is returned.
 *
 * @param str: The input string to be duplicated.
 * @return: A pointer to the duplicated string.
 */
char* strDup(const char* str);

/**
 * thereIsSpace - Checks if a string contains whitespace characters.
 *
 * This function checks if the given string contains any whitespace characters.
 * It iterates through each character in the string and returns True if a whitespace
 * character is found, and False otherwise.
 *
 * @param str: The input string to be checked.
 * @return: Returns 1 (True) if the string contains whitespace characters, and 0 (False) otherwise.
 */
int thereIsSpace(char *str);

/**
 * getNumberFromData - Extracts a number from a string and updates the index.
 *
 * This function extracts a number from the given string, starting from the current index.
 * It supports negative numbers and considers ',' as a delimiter for the number.
 * The function dynamically allocates memory to store the extracted substring.
 *
 * @param str: The input string from which the number is to be extracted.
 * @param index: A pointer to an integer representing the current index in the string.
 *               This index is updated to the position after the extracted number.
 * @return: The extracted number as an integer.
 */
int getNumberFromData(char *str,int *index);

/**
 * checksString - Checks if a string starts with the ".string" substring.
 *
 * This function checks if the given string starts with the substring ".string".
 * It ignores leading whitespace characters before the substring.
 *
 * @param str: The input string to be checked.
 * @return: Returns 1 (True) if the string starts with ".string", and 0 (False) otherwise.
 */
int checkString(const char *str);

/**
 * checksData - Checks if a string starts with the ".data" substring.
 *
 * This function checks if the given string starts with the substring ".data".
 * It ignores leading whitespace characters before the substring.
 *
 * @param str: The input string to be checked.
 * @return: Returns 1 (True) if the string starts with ".data", and 0 (False) otherwise.
 */
int checkData(const char *str);

/**
 * checksEntry - Checks if a string starts with the ".entry" substring.
 *
 * This function checks if the given string starts with the substring ".entry".
 * It ignores leading whitespace characters before the substring.
 *
 * @param str: The input string to be checked.
 * @return: Returns 1 (True) if the string starts with ".entry", and 0 (False) otherwise.
 */
int checksEntry(const char *str);

/**
 * checksExtern - Checks if a string starts with the ".extern" substring.
 *
 * This function checks if the given string starts with the substring ".extern".
 * It ignores leading whitespace characters before the substring.
 *
 * @param str: The input string to be checked.
 * @return: Returns 1 (True) if the string starts with ".extern", and 0 (False) otherwise.
 */
int checksExtern(const char *str);

/**
 * Searches for a comma between 2 number in the given string.
 *
 * @param str Pointer to a string.
 * @return 1 if a comma is found in the string, 0 otherwise.
 */
int findComma(char *str);

/**
 * Searches for a number up to a comma in the given string.
 *
 * @param str Pointer to a string.
 * @return The index up to comma if a valid number is found in the string, ERROR -1 if it is not a valid number.
 */
int allDigitsForData(char *str);
