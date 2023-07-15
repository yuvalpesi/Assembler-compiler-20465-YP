#include "data.h"

void printObjFileBase64(char *argv,LineHolder *head,int IC,int DC){
    FILE *fd=NULL;
    char *sourceFile=NULL,*nameTemp=NULL,*base64=NULL;
    LineHolder *current=head;
    int i,fileSize=strlen(argv);
    char *base64Data=NULL,temp[2];

    sourceFile=(char*)malloc(fileSize+4 * sizeof(char));
    base64Data=malloc(sizeof (char)*(WORD_SIZE+1));
    if(sourceFile == NULL || base64Data==NULL){
        printf(" Failed to allocate memory.\n");
        return;
    }

    strncpy(sourceFile,argv,strlen(argv));
    sourceFile[strlen(argv)]='\0';
    strcat(sourceFile,".ob");
    sourceFile[strlen(sourceFile)]='\0';

    if((fd=fopen(sourceFile,"w+"))==NULL){
        fprintf(stderr, "\n Error: Failed to create the expanded source code file %s. \n", sourceFile);
        free(sourceFile);
        return;
    }

    fprintf(fd,"       %d %d   ",IC,DC);

    while (current!=NULL){
        if(base64Data!=NULL){
            free(base64Data);
            base64Data=NULL;
            base64Data= malloc(WORD_SIZE+1*sizeof(char));
            base64Data[0]='\0';
        }

        fprintf(fd,"\n       ");
        nameTemp= strDup(current->Binary->lableName);
        if(checkString(nameTemp)==True || checkData(nameTemp)==True){

            for (i = 11; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.directiveSentence->directive >> i) & 1;
                sprintf(temp,"%u",bit);
                strcat(base64Data,temp);
            }
            base64= binaryToBase64(base64Data);
            fprintf(fd,"%s", base64);
        } else if(checkRegister(nameTemp)!=ERROR){

            for (i = 4; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.registerAddress->SecRed >> i) & 1;
                sprintf(temp,"%u",bit);
                strcat(base64Data,temp);
            }
            for (i = 4; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.registerAddress->FirstRed >> i) & 1;
                sprintf(temp,"%u",bit);
                strcat(base64Data,temp);
            }

            for (i = 1; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.registerAddress->ARE >> i) & 1;
                sprintf(temp,"%u",bit);
                strcat(base64Data,temp);
            }

            base64= binaryToBase64(base64Data);
            fprintf(fd,"%s", base64);

        } else if(commandType(nameTemp)!=ERROR){

            for (i = 2; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.first->sourceOperand >> i) & 1;
                sprintf(temp,"%u",bit);
                strcat(base64Data,temp);
            }
            for (i = 3; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.first->opcode >> i) & 1;
                sprintf(temp,"%u",bit);
                strcat(base64Data,temp);
            }

            for (i = 2; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.first->targetOperand >> i) & 1;
                sprintf(temp,"%u",bit);
                strcat(base64Data,temp);
            }

            for (i = 1; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.first->ARE >> i) & 1;
                sprintf(temp,"%u",bit);
                strcat(base64Data,temp);
            }

            base64= binaryToBase64(base64Data);
            fprintf(fd,"%s", base64);

        } else if(allDigits(nameTemp)==True){

            for(i = 9; i >= 0; i--){
                unsigned int bit = (current->Binary->operandStucter.immediate->Operand >> i) & 1;
                sprintf(temp,"%u",bit);
                strcat(base64Data,temp);
            }
            for(i = 1; i >= 0; i--){

                unsigned int bit = (current->Binary->operandStucter.immediate->ARE >> i) & 1;
                sprintf(temp,"%u",bit);
                strcat(base64Data,temp);
            }

            base64= binaryToBase64(base64Data);
            fprintf(fd,"%s", base64);

        } else {

            for(i = 9; i >= 0; i--){
                unsigned int bit = (current->Binary->operandStucter.address->Operand >> i) & 1;
                sprintf(temp,"%u",bit);
                strcat(base64Data,temp);
            }
            for(i = 1; i >= 0; i--){
                unsigned int bit = (current->Binary->operandStucter.address->ARE >> i) & 1;
                sprintf(temp,"%u",bit);
                strcat(base64Data,temp);
            }

            base64= binaryToBase64(base64Data);
            fprintf(fd,"%s", base64);
        }

        current=current->next;
        free(nameTemp);
        free(base64);
    }


    fclose(fd);
    free(sourceFile);
    free(base64Data);
}

void printObjFile(char *argv,LineHolder *head,int IC,int DC){
    FILE *fd=NULL;
    char *sourceFile=NULL;
    LineHolder *current=head;
    int i,fileSize=strlen(argv);

    sourceFile=(char*)malloc(fileSize+4 * sizeof(char));

    if(sourceFile == NULL){
        printf(" Failed to allocate memory.\n");
        return;
    }

    strncpy(sourceFile,argv,strlen(argv));
    sourceFile[strlen(argv)]='\0';
    strcat(sourceFile,".ob");
    sourceFile[strlen(sourceFile)]='\0';

    if((fd=fopen(sourceFile,"w+"))==NULL){
        fprintf(stderr, "\n Error: Failed to create the expanded source code file %s. \n", sourceFile);
        free(sourceFile);
        return ;
    }

    fprintf(fd,"           IC=%d , DC=%d   ",IC,DC);

    while (current!=NULL){
        fprintf(fd,"\n0%d       ",current->address);

        if(strcmp(current->Binary->lableName,".string")==0 || strcmp(current->Binary->lableName,".data")==0){
            for (i = 11; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.directiveSentence->directive >> i) & 1;
                fprintf(fd,"%u", bit);
            }
        } else if(checkRegister(current->Binary->lableName)!=ERROR){
            for (i = 4; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.registerAddress->SecRed >> i) & 1;
                fprintf(fd,"%u", bit);
            }
            for (i = 4; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.registerAddress->FirstRed >> i) & 1;
                fprintf(fd,"%u", bit);
            }

            for (i = 1; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.registerAddress->ARE >> i) & 1;
                fprintf(fd,"%u", bit);
            }

        } else if(commandType(current->Binary->lableName)!=ERROR){
            for (i = 2; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.first->sourceOperand >> i) & 1;
                fprintf(fd,"%u", bit);
            }
            for (i = 3; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.first->opcode >> i) & 1;
                fprintf(fd,"%u", bit);
            }

            for (i = 2; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.first->targetOperand >> i) & 1;
                fprintf(fd,"%u", bit);
            }

            for (i = 1; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.first->ARE >> i) & 1;
                fprintf(fd,"%u", bit);
            }
        } else if(allDigits(current->Binary->lableName)==True){
            for(i = 9; i >= 0; i--){
                unsigned int bit = (current->Binary->operandStucter.immediate->Operand >> i) & 1;
                fprintf(fd,"%u", bit);
            }
            for(i = 1; i >= 0; i--){

                unsigned int bit = (current->Binary->operandStucter.immediate->ARE >> i) & 1;
                fprintf(fd,"%u", bit);
            }
        } else {
            for(i = 9; i >= 0; i--){
                unsigned int bit = (current->Binary->operandStucter.address->Operand >> i) & 1;
                fprintf(fd,"%u", bit);
            }
            for(i = 1; i >= 0; i--){

                unsigned int bit = (current->Binary->operandStucter.address->ARE >> i) & 1;
                fprintf(fd,"%u", bit);
            }

        }

        current=current->next;
    }

    fclose(fd);
    free(sourceFile);
}

void printEntFile(char *argv,LineHolder *head){
    FILE *fd=NULL;
    char *sourceFile=NULL;
    LineHolder *current=head;

    sourceFile=(char*)malloc((strlen(argv)+4) * sizeof(char));

    if(sourceFile == NULL){
        printf(" Failed to allocate memory.\n");
        return;
    }

    strncpy(sourceFile,argv,strlen(argv));
    sourceFile[strlen(argv)]='\0';
    strcat(sourceFile,".ent");
    sourceFile[strlen(sourceFile)]='\0';

    if((fd=fopen(sourceFile,"w+"))==NULL){
        fprintf(stderr, "\n Error: Failed to create the expanded source code file %s. \n", sourceFile);
        free(sourceFile);
        return ;
    }

    while (current!=NULL){
        fprintf(fd,"%s       %d\n",current->Binary->lableName,current->address);
        current=current->next;
    }

    fclose(fd);
    free(sourceFile);
}

void printExtFile(char *argv,LineHolder *head){
    FILE *fd=NULL;
    char *sourceFile=NULL;
    LineHolder *current=head;

    sourceFile=(char*)malloc((strlen(argv)+4) * sizeof(char));

    if(sourceFile == NULL){
        printf(" Failed to allocate memory.\n");
        return;
    }

    strncpy(sourceFile,argv,strlen(argv));
    sourceFile[strlen(argv)]='\0';
    strcat(sourceFile,".ext");
    sourceFile[strlen(sourceFile)]='\0';

    if((fd=fopen(sourceFile,"w+"))==NULL){
        fprintf(stderr, "\n Error: Failed to create the expanded source code file %s. \n", sourceFile);
        free(sourceFile);
        return ;
    }

    while (current!=NULL){
        fprintf(fd,"%s       %d\n",current->Binary->lableName,current->address);
        current=current->next;
    }

    fclose(fd);
    free(sourceFile);
}

char *binaryToBase64(const char *binary){
    const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char *first=NULL,*sec=NULL,*base64=NULL;
    int i,resultFirst=0,resultSec=0,sizeOfBase64=3,halfWord=(WORD_SIZE/2);

    first = malloc(sizeof(char)*((WORD_SIZE/2)+1));
    sec = malloc(sizeof(char)*((WORD_SIZE/2)+1));
    base64= malloc(sizeof(char)*sizeOfBase64);

    if(first==NULL || sec==NULL || base64==NULL){
        printf(" Failed to allocate memory.\n");
        exit(0);
    }

    strncpy(first,binary,halfWord);
    first[strlen(first)+1]='\0';
    strcpy(sec,binary+halfWord);
    sec[strlen(sec)+1]='\0';

    for (i = 0; i < halfWord; i++) {
        resultFirst <<= 1;
        resultFirst |= (first[i] - '0');
        resultSec <<= 1;
        resultSec |= (sec[i] - '0');
    }

    base64[0]=base64_table[resultFirst];
    base64[1]=base64_table[resultSec];
    base64[2]='\0';

    free(first);
    free(sec);
    return base64;
}

