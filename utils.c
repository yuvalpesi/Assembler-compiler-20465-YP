#include "data.h"

char* ignorSpace(char *str){
    int i=0;

    if(str==NULL){
        return str;
    }

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
        printf("Error: Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    while(str[i]!=',' && str[i]!='\0' &&str[i]!=' '){
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
        printf("Error: Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
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

int allDigitsForData(char *str){
    int i=str[0]=='-'?1:0;

    if(!isdigit(str[i])){
        return i;
    }

    while(str[i]!='\0' && str[i]!=','){
        if(!isdigit(str[i])){
            return i+1;
        }
        i++;
    }
    return ERROR;
}

char* strDup(const char* str) {
    int len = strlen(str);
    char *duplicate = NULL;
    len+=2;
    duplicate=malloc( sizeof(char)*len);

    if(duplicate==NULL){
        printf("Error: Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
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
                printf("Error: Failed to allocate memory.\n");
                exit(EXIT_FAILURE);
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
    int i=0,j=0;
    char string[]=".string";
    /* skip leading whitespace characters*/
    while (isspace(str[i])){
        i++;
    }

    while(j< strlen(string)){
        if(str[i]!=string[j]){
            return False;
        }
        i++;
        j++;
    }

    return True;
}

int checkData(const char *str){
    int i=0,j=0;
    char data[]=".data";

    if(str==NULL){
        return False;
    }
    /* skip leading whitespace characters*/
    while (isspace(str[i])){
        i++;
    }

    while(j< strlen(data)){
        if(str[i]!=data[j]){
            return False;
        }
        i++;
        j++;
    }

    return True;
}

int checksExtern(const char *str){
    int i=0,j=0;
    char Extern[]=".extern";
    /* skip leading whitespace characters*/
    while (isspace(str[i])){
        i++;
    }

    while(j< strlen(Extern)){
        if(str[i]!=Extern[j]){
            return False;
        }
        i++;
        j++;
    }

    return True;
}

int checksEntry(const char *str){
    int i=0,j=0;
    char Entry[]=".entry";
    /* skip leading whitespace characters*/
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

int findComma(char *str){
    int i=0;

    while (i< strlen(str)){
        if(isdigit(str[i])){
            i++;

            if(isdigit(str[i]) || isalpha(str[i])){
                continue;
            }

            while (i<strlen(str)){
                if(str[i]==',' || str[i]=='\n' || str[i]=='\0'){
                    break;
                } else if((isdigit(str[i]) || isalpha(str[i]))){
                    return False;
                }
                i++;
            }

        }

        if(str[i]=='-' && str[i+1]==' '){
            return False;
        }
        i++;
    }
    return True;
}
