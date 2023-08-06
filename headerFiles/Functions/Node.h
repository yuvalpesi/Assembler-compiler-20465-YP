
/**
 * Structure defining a node in the linked list representing assembly lines.
 */
typedef struct Line{
    int address;
    Operand *Binary;
    struct Line *next;
}LineHolder;

/**
 * Creates a new node for a lineNodeStruct given string line.
 *
 * @param address The number address to be stored in the node.
 * @param code A pointer too the binary code struct.
 * @return A pointer to the newly created node, or NULL if memory allocation fails.
 */
LineHolder* createNodeItem(int address,struct Operand *code);

/**
 * Adds a new node to a linked list.
 *
 * @param head A pointer to the head of the linked list.
 * @param nodeItem A pointer to the node to be added.
 */
void addNode(LineHolder **head, LineHolder *nodeItem);

/**
 * This function replaces the binary representation of a LineHolder node with a new code.
 * It searches for a specific item (label name) in the LineHolder linked list and replaces
 * the binary representation with the provided code. If the item is found, the memory allocated
 * for the previous binary representation is freed.
 *
 * @param head: The head of the LineHolder linked list.
 * @param item: The item (label name) to search for.
 * @param code: The new binary representation to replace with.
 */
void replaceNodeItem(LineHolder * head, char *item, Operand *code);

/**
 * Frees a linked list, deallocating the memory occupied by each node.
 *
 * @param head A pointer to the head node of the linked list.
 */
void freeListNode(LineHolder *head);

/**
 * Structure defining a node in the linked list representing lines of text.
 */
typedef struct lineStr{
    char *lineStr;
    struct lineStr *next;
}lineStr;

/**
 * Creates a new node for a lineNode given string line.
 *
 * @param line The string line to be stored in the node.
 * @return A pointer to the newly created node, or NULL if memory allocation fails.
 */
lineStr* createNodeLine(char *line);

/**
 * Adds a new node to a linked list.
 *
 * @param head A pointer to the head of the linked list.
 * @param nodeItem A pointer to the node to be added.
 */
void addNodeLine(lineStr **head, lineStr *nodeItem);

/**
 * Frees a linked list, deallocating the memory occupied by each node.
 *
 * @param head A pointer to the head node of the linked list.
 */
void freeListNodeLine(lineStr *head);

/**
 * Structure defining a node in the linked list representing .entry.
 */
typedef struct EnNode{
    char lineStr[MAX_LABLE];
    int address;
    struct EnNode *next;
}EnNode;

/**
 * Structure defining a node in the linked list representing .extern.
 */
typedef struct ExNode{
    char lineStr[MAX_LABLE];
    int address;
    struct ExNode *next;
}ExNode;


/**
 * Creates a new entry node for a lineNode given string name and address.
 *
 * @param line The string name to be stored in the node.
 * @param address The address number to be stored in the node.
 * @return A pointer to the newly created node, or NULL if memory allocation fails.
 */
EnNode* createNodeEn(char *line,int address);

/**
 * Adds a new entry node to a linked list.
 *
 * @param head A pointer to the head of the linked list.
 * @param nodeItem A pointer to the node to be added.
 */
void addNodeEn(EnNode **head, EnNode *nodeItem);

/**
 * Frees a entry linked list, deallocating the memory occupied by each node.
 *
 * @param head A pointer to the head node of the entry linked list.
 */
void freeListNodeEn(EnNode *head);

/**
 * Creates a new extern node for a lineNode given string name and address.
 *
 * @param line The string name to be stored in the node.
 * @param address The address number to be stored in the node.
 * @return A pointer to the newly created node, or NULL if memory allocation fails.
 */
ExNode* createNodeEx(char *line,int address);

/**
 * Adds a new extern node to a linked list.
 *
 * @param head A pointer to the head of the linked list.
 * @param nodeItem A pointer to the node to be added.
 */
void addNodeEx(ExNode **head, ExNode *nodeItem);

/**
 * Frees a extern linked list, deallocating the memory occupied by each node.
 *
 * @param head A pointer to the head node of the extern linked list.
 */
void freeListNodeEx(ExNode *head);
