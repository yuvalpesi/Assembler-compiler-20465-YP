
typedef struct lineLoc{
    char *filename;
    char *str;
    int lineLoction;
}lineLoc;

typedef struct lineStr{
    char *lineStr;
    struct lineStr *next;
}lineStr;

char* ignorSpace(char *str);

void RemoveQuotationMarks(char *str);

void RemoveAllSpaces(char *str);

char *strFirstArgu(char *str);

char *allocateStr(char *strFirst, char* strSec);

int allDigits(char *str);

char* strDup(const char* str);

int thereIsSpace(char *str);

int getNumberFromData(char *str,int *index);

int checkString(const char *str);

int checkData(const char *str);

int checksEntry(const char *str);

int checksExtern(const char *str);

lineStr* createNodeLine(char *line);

void addNodeLine(lineStr **head, lineStr *nodeItem);

void freeListNodeLine(lineStr *head);
