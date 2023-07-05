
int secPass(char *argv,LineHolder *head, symbolTable *symbol,int IC,int DC);

void replaceNodeItem(LineHolder * head, char *item, Operand *code);

int checkEntry(symbolTable *table,LineHolder **head);

int checkExtern(symbolTable *table,LineHolder **head,LineHolder *curr);

