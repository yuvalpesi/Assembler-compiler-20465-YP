
typedef struct Line{
    int address;
    Operand *Binary;
    struct Line *next;
}LineHolder;

LineHolder* createNodeItem(int address,struct Operand *code);

void addNode(LineHolder **head, LineHolder *nodeItem);

void freeListNode(LineHolder *head);

