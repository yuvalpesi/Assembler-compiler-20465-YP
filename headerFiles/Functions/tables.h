
/**
 * Structure defining a node in the macro table.
 */
typedef struct mcroTable{
    struct mcroTable *next;
    char *mcroName;
    char *mcroContent;

}mcroTable;

/**
 * Structure defining the macro table.
 */
typedef struct{
    mcroTable **entry;
}mcro;

/**
 * Enumeration defining the symbol types for the symbol table.
 */
enum enrtyExternIcDc{
    sIC,
    sDC,
    sENTRY=2,
    sEXETRN=3
};

/**
 * Structure defining a node in the symbol table.
 */
typedef struct symbolTableStart{
    struct symbolTableStart *next;
    int  symbolLoc;
    char *symbolName;
    int symbolType;

}symbolTableStart;

/**
 * Structure defining the symbol table.
 */
typedef struct{
    symbolTableStart **entry;
}symbolTable;

/**
 * This function calculates the hash value for a given string using the djb2 hash algorithm.
 * It iterates over each character of the string, performs a multiplication and addition
 * operation on the hash value, and takes the modulo operation to ensure the resulting value
 * is between 0 and TABLE_SIZE. The calculated hash value is returned as an unsigned integer.
 *
 * @param name: The input string to calculate the hash value for.
 * @return: The calculated hash value as an unsigned integer.
 */
unsigned int hash(const char *name);

/**
 * This function initializes a macro table by allocating memory for the table structure and its
 * entries. It first allocates memory for the macro table structure itself and checks if the allocation
 * was successful. Then, it allocates memory for the entries using calloc to initialize them to 0.
 * If the allocation of the entries fails, the previously allocated memory for the macro table is freed.
 * The function returns a pointer to the initialized macro table if successful, or NULL if any allocation
 * fails.
 *
 * @return: A pointer to the initialized macro table, or NULL if memory allocation fails.
 */
mcro *initMcroTable(void);

/**
 * This function performs a lookup for a macro name in the macro table using the provided hash function.
 * It calculates the hash value of the macro name to determine the index in the macro table. It then
 * traverses the linked list of entries at that index and compares each entry's macro name with the
 * provided macro name until a match is found or the end of the list is reached. If a match is found,
 * the function returns a pointer to the content (string) of the macro. Otherwise, it returns NULL.
 *
 * @param macroTable: The macro table to perform the lookup in.
 * @param McroName: The macro name to search for.
 * @return: A pointer to the content (string) of the macro if found, NULL otherwise.
 */
char *mcroTableLockUp(mcro *macroTable,const char *McroName);

/**
 * This function inserts a macro name and its corresponding content into the macro table using
 * the provided hash function. It calculates the hash value of the macro name to determine the
 * index in the macro table. It then traverses the linked list of entries at that index to check
 * if the macro name already exists. If a matching macro name is found, the function updates its
 * content with the provided macro content. If no matching macro name is found, a new entry is
 * created with the macro name and content and added to the end of the linked list at the index.
 *
 * @param macroTable: The macro table to insert the macro into.
 * @param McroName: The name of the macro to be inserted.
 * @param McroContent: The content (string) of the macro.
 */
void mcroTableInsert(mcro *macroTable,const char *McroName,const char *McroContent);

/**
 * This function creates a new entry for the macro table by allocating memory and setting the
 * macro name, macro content, and next pointer. It first allocates memory for the mcroTable
 * structure and checks if the allocation was successful. Then, it allocates memory for the
 * macro name and macro content using the strDup function to duplicate the provided strings.
 * If any memory allocation fails, the previously allocated memory is freed and NULL is returned.
 * Otherwise, the newly created entry is returned.
 *
 * @param McroName: The name of the macro.
 * @param McroContent: The content (string) of the macro.
 * @return: A pointer to the newly created entry, or NULL if memory allocation fails.
 */
mcroTable *mcroTableInstall(const char *McroName,const char *McroContent);

/**
 * This function frees the memory allocated for the macro table and its entries. It iterates over
 * each entry in the macro table and frees the memory for the macro name, macro content, and the
 * entry itself. It then frees the memory allocated for the macro table structure. After calling
 * this function, the macro table and its entries are no longer valid.
 *
 * @param macro: The macro table to be freed.
 */
void freeMcroTable(mcro *macroTable);

/**
 * This function initializes a symbol table by allocating memory for the symbol table structure and
 * its entries. It first allocates memory for the symbol table structure itself and checks if the
 * allocation was successful. Then, it allocates memory for the entries using calloc to initialize
 * them to NULL. If the allocation of the entries fails, the previously allocated memory for the
 * symbol table is freed. The function returns a pointer to the initialized symbol table if successful,
 * or NULL if any allocation fails.
 *
 * @return: A pointer to the initialized symbol table, or NULL if memory allocation fails.
 */
symbolTable *initSymbolTable(void);

/**
 * Inserts a symbol into the symbol table. If a symbol with the same name and type already exists,
 * it updates its address. If the symbol doesn't exist, a new entry is created and added to the symbol table.
 *
 * @param symbolTable   Pointer to the symbol table object.
 * @param symbolName    Pointer to a string representing the name of the symbol.
 * @param addressSymbol Integer value representing the address of the symbol.
 * @param symbolType    Integer value representing the type of the symbol.
 */
void symbolTableInsert(symbolTable *symbolTable,char *symbolName,int addressSymbol,int symbolType);

/**
 * Creates a new symbol table entry and initializes it with the provided symbol information.
 *
 * @param symbolName    Pointer to a string representing the name of the symbol.
 * @param addressSymbol Integer value representing the address of the symbol.
 * @param symbolType    Integer value representing the type of the symbol.
 * @return Pointer to the newly created symbol table entry.
 */
symbolTableStart *symbolTableInsall(char *symbolName,int addressSymbol,int symbolType);

/**
 * Looks up a symbol in the symbol table and returns its symbol name if found.
 *
 * @param symbolTable Pointer to the symbol table object.
 * @param symbolName  Pointer to a string representing the name of the symbol to look up.
 * @return Pointer to the symbol name if found, otherwise NULL.
 */
char *symbolTableLockUp(symbolTable *symbolTable,char *symbolName);

/**
 * Looks up a symbol in the symbol table and returns its address if found.
 *
 * @param symbolTable Pointer to the symbol table object.
 * @param symbolName  Pointer to a string representing the name of the symbol to look up.
 * @return The address of the symbol if found and its symbol type is either sIC or sDC.
 *         If the symbol is not found or its type is not sIC or sDC, it returns -2.
 */
int symbolTableLockUpAddress(symbolTable *symbolTable,char *symbolName);

/**
 * Looks up a symbol in the symbol table and returns its symbol type if found.
 *
 * @param symbolTable Pointer to the symbol table object.
 * @param symbolName  Pointer to a string representing the name of the symbol to look up.
 * @return The symbol type of the symbol if found. Possible return values:
 *         - sEXETRN: If the symbol type is sEXETRN.
 *         - sENTRY: If the symbol type is sENTRY.
 *         - sDC or sIC: If the symbol type is either sDC or sIC.
 *         - ERROR: If the symbol is not found.
 */
int symbolTableLockUpType(symbolTable *symbolTable,char *symbolName);

/**
 * Looks up a symbol in the symbol table and returns its symbol type if found, considering only sDC and sIC types.
 *
 * @param symbolTable Pointer to the symbol table object.
 * @param symbolName  Pointer to a string representing the name of the symbol to look up.
 * @return The symbol type of the symbol if found and it is either sDC or sIC.
 *         If the symbol is not found or its type is not sDC or sIC, it returns ERROR.
 */
int symbolTableLockUpICDC(symbolTable *symbolTable,char *symbolName);

/**
 * Frees the memory allocated for the symbol table and its entries.
 *
 * @param symbolTable Pointer to the symbol table object to be freed.
 */
void freeSymbolTable(symbolTable *symbolTable);
