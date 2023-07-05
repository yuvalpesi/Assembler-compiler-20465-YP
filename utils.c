#include "data.h"

char* ignorSpace(char *str){
    int i=0;

    while((str[i]==' ' || str[i]=='\t') && str[i]!='\0'){
        i++;
    }

    memmove(str,str+i, strlen(str)+1);
    return str;
}

void RemoveAllSpaces(char *str){
    int i,count=0;

    for(i=0;str[i];i++){
        if(str[i]!=' ' && str[i]!='\t' && str[i]!='\n'){
            str[count++]=str[i];
        }
    }

    str[count]='\0';
}

void RemoveQuotationMarks(char *str){

    int i,count=0;

    for(i=0;str[i]!='\0';i++){
        if( str[i]!='"'){
            str[count++]=str[i];
        }
    }

    str[count]='\0';

}

char *strFirstArgu(char *str){
    int i=0;
    char *temp=NULL;

    temp=(char*) malloc(sizeof (char ) * strlen(str));

    if(temp==NULL){
        printf(" Failed to allocate memory.\n");
        exit(0);
    }

    while(str[i]!=',' && str[i]!='\0'){
        i++;
    }
    strncpy(temp,str,i);
    temp[i]='\0';
    return temp;
}


char *allocateStr(char *strFirst, char* strSec){
    char *str=NULL;
    str=(char *)malloc(strlen(strFirst)+strlen(strSec)+2);

    if(str==NULL){
        printf(" Failed to allocate memory.\n");
        exit(0);
    }

    strcpy(str, strFirst);
    strcat(str, strSec);
    return str;
}



int allDigits(char *str){
    int i=str[0]=='-'?1:0;

    while(str[i]!='\0'){
        if(!isdigit(str[i])){
            return False;
        }
        i++;
    }
    return True;
}

char* strDup(const char* str) {
    size_t len = strlen(str);
    char* duplicate = NULL;
    duplicate=(char*)malloc((len + 1) * sizeof(char));

    if(duplicate==NULL){
        printf(" Failed to allocate memory.\n");
        exit(0);
    }

    strcpy(duplicate, str);
    return duplicate;
}

int thereIsSpace(char *str){
    int i;

    for(i=0;i< strlen(str);i++){
        if(isspace(str[i])){
            return True;
        }
    }

    return False;
}

int getNumberFromData(char *str,int *index){
    int num,count=0;
    char *temp=NULL;

    while (str[count]!='\0'){
        if(str[count]=='-'){
            count++;
        }

        if(str[count]==','|| str[count+1]=='\0'){
            temp=(char*)malloc((count+2)*sizeof(char));

            if(temp==NULL){
                printf(" Failed to allocate memory.\n");
                exit(0);
            }

            if(str[count+1]=='\0'){
                strncpy(temp,str,count+1);
                temp[count+1]='\0';
                (*index)=count+1;
                break;
            } else{
                strncpy(temp,str,count);
                temp[count]='\0';
                (*index)=count+1;
                break;
            }
        }
        count++;

    }
    num=atoi(temp);
    free(temp);
    return num;
}

int checkString(const char *str){
    int i=0;
    char string[]=".string";

    while(i< strlen(string)){
         if(str[i]!=string[i]){
             return False;
         }
         i++;
    }

    return True;
}

int checkData(const char *str){
    int i=0;
    char data[]=".data";

    while(i< strlen(data)){
        if(str[i]!=data[i]){
            return False;
        }
        i++;
    }

    return True;
}

int checksExtern(const char *str){
    int i=0;
    char Extern[]=".extern";

    while(i< strlen(Extern)){
        if(str[i]!=Extern[i]){
            return False;
        }
        i++;
    }

    return True;
}

int checksEntry(const char *str){
    int i=0,j=0;
    char Entry[]=".entry";

    while (isspace(str[i])){
        i++;
    }

    while(j< strlen(Entry)){
        if(str[i]!=Entry[j]){
            return False;
        }
        i++;
        j++;
    }

    return True;
}

lineStr* createNodeLine(char *line){
    lineStr *node = (lineStr*)malloc(sizeof(lineStr)); /* allocates memory for the node */

    if (node==NULL){
        printf(" Error: unable to allocate memory for new node\n");
        return NULL;
    }

    /* Set the data and next pointer of the node */
    node->lineStr= strDup(line);
    node->next = NULL;
    return node;
}

void addNodeLine(lineStr **head, lineStr *nodeItem){
    if (*head==NULL){
        *head = nodeItem;
        nodeItem->next = NULL;
    }else{/* If the linked list already exists */
        lineStr *current = *head; /*Create a current pointer to traverse the list */

        /* Find the last node in the circular list */
        while(current->next!=NULL) {
            current = current->next;
        }
        current->next = nodeItem; /* Set the new node as the next node of the last node */
        nodeItem->next = NULL; /* Maintain the circular list by setting the next pointer of the new node to the head */
    }
}

void freeListNodeLine(lineStr *head){
    lineStr *temp=NULL;

    while(head!=NULL){
        temp=head->next;
        free(head->lineStr);
        free(head);
        head=temp;
    }
}
