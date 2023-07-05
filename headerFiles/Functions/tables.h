
#define TABLE_SIZE 64

typedef struct mcroTable{
    struct mcroTable *next;
    char *mcroName;
    char *mcroContent;

}mcroTable;

typedef struct{
    mcroTable **entry;
}mcro;

enum enrtyExtern{
    NoEntryOrExtern,
    sENTRY=2,
    sEXETRN=3
};

typedef struct symbolTableStart{
    struct symbolTableStart *next;
    int  symbolLoc;
    char *symbolName;
    int symbolType;

}symbolTableStart;

typedef struct{
    symbolTableStart **entry;
}symbolTable;


unsigned int hash(const char *name);

/* Creates an empty Macros Table and return A pointer to the table */
mcro *initMcroTable(void);

char *mcroTableLockUp(mcro *macroTable,const char *McroName);

void mcroTableInsert(mcro *macroTable,const char *McroName,const char *McroContent);

mcroTable *mcroTableInstall(const char *McroName,const char *McroContent);

void freeMcroTable(mcro *macroTable);

/* Creates an empty symbol Table and return a pointer to the table */
symbolTable *initSymbolTable(void);

/* add a entry */
void symbolTableInsert(symbolTable *symbolTable,char *symbolName,int addressSymbol,int symbolType);

/* Alloc memory */
symbolTableStart *symbolTableInsall(char *symbolName,int addressSymbol,int symbolType);

char *symbolTableLockUp(symbolTable *symbolTable,char *symbolName);

int symbolTableLockUpAddress(symbolTable *symbolTable,char *symbolName);

int symbolTableLockUpType(symbolTable *symbolTable,char *symbolName);

void freeSymbolTable(symbolTable *symbolTable);
