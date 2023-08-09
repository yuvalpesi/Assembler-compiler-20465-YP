
/**
 * Structure defining a node in the linked list representing assembly addressing image lines.
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
 * Adds a new node to a addressing linked list.
 * If it is data image it will located in the end of the linked list
 * If it is code it will find the last code image and put it in the end of it
 * And the list will be at first the code image and right after it end
 * the data image
 *
 * @param head A pointer to the head of the addressing linked list.
 * @param nodeItem A pointer to the address node to be added.
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
 * Structure defining a node in the linked list representing .entry.
 */
typedef struct EnNode{
    char lable[MAX_LABLE];
    int address;
    struct EnNode *next;
}EnNode;

/**
 * Creates a new entry node for a lineNode given string name and address.
 *
 * @param line The string name to be stored in the node.
 * @param address The address number to be stored in the node.
 * @return A pointer to the newly created node, or NULL if memory allocation fails.
 */
EnNode* createNodeEn(const char *line,int address);

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
 * Structure defining a node in the linked list representing .extern.
 */
typedef struct ExNode{
    char lable[MAX_LABLE];
    int address;
    struct ExNode *next;
}ExNode;

/**
 * Creates a new extern node for a lineNode given string name and address.
 *
 * @param line The string name to be stored in the node.
 * @param address The address number to be stored in the node.
 * @return A pointer to the newly created node, or NULL if memory allocation fails.
 */
ExNode* createNodeEx(const char *line,int address);

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
