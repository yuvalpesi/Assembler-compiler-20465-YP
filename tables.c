#include "data.h"

unsigned int hash(const char *name){
    int length=strlen(name);
    unsigned long int hash_value=0;
    unsigned int i;

    /* Do several rounds of multiplication */
    for (i=0;i<length;i++){
        hash_value +=name[i];
        hash_value=(hash_value * 31 + name[i]) % TABLE_SIZE; /* Make sure value that is between 0 and TABLE_SIZE */
    }
    return hash_value;
}

mcro *initMcroTable(void){
    /* Allocate memory for table*/
    mcro *macroTable=malloc(sizeof(mcro));

    if (macroTable == NULL) {
        printf("Error: Failed to allocate memory for macro table.\n");
        exit(EXIT_FAILURE);
    }

    /* Allocate memory for the entry */
    macroTable->entry=calloc(sizeof(mcroTable),TABLE_SIZE);
    if (macroTable->entry == NULL) {
        printf("Error: Failed to allocate memory for macro table entries.\n");
        free(macroTable);
        exit(EXIT_FAILURE);
    }

    return macroTable;
}

char *mcroTableLockUp(mcro *macroTable,const char *McroName){
    unsigned int index = hash(McroName);

    /* try to find a valid data */
    mcroTable *temp = macroTable->entry[index];

    /* walk through each head in the data, which could just be a single thing  */
    while (temp != NULL && strcmp(temp->mcroName, McroName) != 0) {
        temp = temp->next; /* proceed to next key if available */
    }

    return temp != NULL ? temp->mcroContent : NULL;
}

void mcroTableInsert(mcro *macroTable,const char *McroName,const char *McroContent){
    unsigned int index=hash(McroName);
    mcroTable *current=macroTable->entry[index];
    mcroTable *prev=NULL,*newEntry=NULL;

    if (current == NULL){
        macroTable->entry[index]=mcroTableInstall(McroName,McroContent);
        return;
    }

    /* Iterate through each entry to until the end is reach or key match found*/
    while (current != NULL) {
        if(strcmp(current->mcroName, McroName) == 0) {
            free(current->mcroContent);
            current->mcroContent = strDup(McroContent);
            return;
        }
        /* Iterate to the next node */
        prev = current;
        current = prev->next;
    }
    /* Reached the end of the linked list, add another node with the pair */
    newEntry = mcroTableInstall(McroName,McroContent);

    if (prev == NULL) {
        macroTable->entry[index] = newEntry;
    } else {
        prev->next = newEntry;
    }
}

mcroTable *mcroTableInstall(const char *McroName,const char *McroContent){
    /* Allocate memory for mcroTable */
    mcroTable *newEntry=malloc(sizeof(mcroTable));

    if (newEntry == NULL) {
        printf("Error: Failed to allocate memory for macro table entry.\n");
        exit(EXIT_FAILURE);
    }

    newEntry->mcroName = strDup(McroName);
    newEntry->mcroContent = strDup(McroContent);
    newEntry->next = NULL;

    return newEntry;
}

void freeMcroTable(mcro *macro){
    int i;
    mcroTable *temp=NULL,*prev=NULL;

    for(i=0;i<TABLE_SIZE;i++){
        temp=macro->entry[i];

        while(temp!=NULL){
            prev=temp;
            temp=temp->next;
            free(prev->mcroName);
            free(prev->mcroContent);
            free(prev);
        }

    }

    free(macro->entry);
    free(macro);
}

symbolTable *initSymbolTable(void){
    /* Allocate memory for table*/
    symbolTable *symbolTable=malloc(sizeof(symbolTable));

    if (symbolTable == NULL) {
        printf("Error: Failed to allocate memory for symbol table.\n");
        exit(EXIT_FAILURE);
    }

    /* Allocate memory for the entries */
    symbolTable->entry=calloc(sizeof(symbolTableStart),TABLE_SIZE);

    if (symbolTable->entry == NULL) {
        printf("Error: Failed to allocate memory for symbol table entries.\n");
        free(symbolTable);
        exit(EXIT_FAILURE);
    }

    return symbolTable;
}

void symbolTableInsert(symbolTable *symbolTable,char *symbolName,int addressSymbol,int symbolType){
    unsigned int index=hash(symbolName);
    int item;
    symbolTableStart *current=symbolTable->entry[index];
    symbolTableStart *prev=NULL,*newEntry=NULL;


    /* Iterate through each entry to until the end is reach or key match found*/
    while (current != NULL) {
        item=current->symbolType;
        if(item==symbolType && strcmp(current->symbolName, symbolName) == 0 ) {
            /*Match found - Will replace the existing key with new one*/
            free(current->symbolName);

            current->symbolName = strDup(symbolName);
            current->symbolLoc = addressSymbol;
            current->symbolType = symbolType;
            return;
        }
        /* Iterate to the next node */
        prev = current;
        current = prev->next;
    }
    /* Reached the end of the linked list, add another node with the pair */
    newEntry = symbolTableInsall(symbolName,addressSymbol,symbolType);
    if (prev == NULL) {
        symbolTable->entry[index] = newEntry;
    } else {
        prev->next = newEntry;
    }
}

symbolTableStart *symbolTableInsall(char *symbolName,int addressSymbol,int symbolType){
    /* Allocate memory for the symbolTable */
    symbolTableStart *newEntry=malloc(sizeof(symbolTableStart));

    if (newEntry == NULL) {
        printf("Error: Failed to allocate memory for symbol table.\n");
        exit(EXIT_FAILURE);
    }

    /* Get the value of the key & the data. */
    newEntry->symbolName = strDup(symbolName);
    newEntry->symbolLoc = addressSymbol;
    newEntry->symbolType = symbolType;
    newEntry->next = NULL;

    return newEntry;
}

char *symbolTableLockUp(symbolTable *symbolTable,char *symbolName){
    unsigned int index = hash(symbolName);

    symbolTableStart *temp = symbolTable->entry[index];

    while (temp != NULL) {
        /* return value if found */
        if (strcmp(temp->symbolName, symbolName) == 0) {
            return temp->symbolName;
        }
        temp = temp->next;
    }

    /* reaching here means there were >= 1 entries but no key match */
    return NULL;
}

int symbolTableLockUpAddress(symbolTable *symbolTable,char *symbolName){
    unsigned int index = hash(symbolName);
    int tmp;

    symbolTableStart *temp = symbolTable->entry[index];

    while (temp != NULL) {
        /* return value if found */
        if (strcmp(temp->symbolName, symbolName) == 0 && (temp->symbolType==sIC || temp->symbolType==sDC)) {
            tmp=temp->symbolLoc;
            return tmp;
        }
        temp = temp->next;
    }

    /* reaching here means there were >= 1 entries but no key match */
    return ERROR;
}

int symbolTableLockUpType(symbolTable *symbolTable,char *symbolName){
    unsigned int index = hash(symbolName);


    symbolTableStart *temp = symbolTable->entry[index];

    while (temp != NULL) {
        /* return value if found */
        if(strcmp(temp->symbolName, symbolName) == 0 && temp->symbolType ==sEXETRN){
            return sEXETRN;

        } else if(strcmp(temp->symbolName, symbolName) == 0 && temp->symbolType==sENTRY){
            return sENTRY;
        } else if( strcmp(temp->symbolName, symbolName) == 0 && (temp->symbolType==sDC || temp->symbolType==sIC)){
            return temp->symbolType;
        }
        temp = temp->next;
    }

    /* reaching here means there were >= 1 entries but no key match */
    return ERROR;

}

int symbolTableLockUpICDC(symbolTable *symbolTable,char *symbolName){
    unsigned int index = hash(symbolName);


    symbolTableStart *temp = symbolTable->entry[index];

    while (temp != NULL) {
        /* return value if found */
        if( strcmp(temp->symbolName, symbolName) == 0 && (temp->symbolType==sDC || temp->symbolType==sIC)){
            return temp->symbolType;
        }
        temp = temp->next;
    }

    /* reaching here means there were >= 1 entries but no key match */
    return ERROR;

}

void freeSymbolTable(symbolTable *symbolTable){
    int i=0;
    symbolTableStart *temp=NULL,*prev=NULL;

    for (i=0;i<TABLE_SIZE;i++){
        temp=symbolTable->entry[i];
        while (temp!=NULL){
            prev=temp;
            temp = temp->next;
            free(prev->symbolName);
            free(prev);
        }
    }

    free(symbolTable->entry);
    free(symbolTable);
}
