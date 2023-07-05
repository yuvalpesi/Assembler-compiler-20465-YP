#include "data.h"

void printObjFileBase64(char *argv,LineHolder *head,int IC,int DC){
    FILE *fd=NULL;
    char *sourceFile=NULL,*nameTemp=NULL;
    LineHolder *current=head;
    int i,fileSize=strlen(argv);
    char base64Data[5];

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

    fprintf(fd,"         %d %d   ",IC,DC);

    while (current!=NULL){
        fprintf(fd,"\n0%d       ",current->address);
        nameTemp= strDup(current->Binary->lableName);
        if(checkString(nameTemp)==True || checkData(nameTemp)==True){
            for (i = 11; i >= 0; i--) {
                unsigned int bit = (current->Binary->operandStucter.directiveSentence->directive >> i) & 1;
                fprintf(fd,"%u", bit);
            }
        } else if(checkRegister(nameTemp)!=ERROR){
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

        } else if(commandType(nameTemp)!=ERROR){
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
        } else if(allDigits(nameTemp)==True){
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



        /*fprintf(fd,"%u",binaryHolder);*/
        current=current->next;
        free(nameTemp);
    }


    fclose(fd);
    free(sourceFile);
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
