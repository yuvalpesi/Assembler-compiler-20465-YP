
typedef struct firstW{

}firstW;

typedef struct directImi{

}directImi;

typedef struct registerP{

}registerP;

typedef struct mechin{
    char *lable;
    union string
    {
       firstW *Fstr;
       directImi *strLoc;
       registerP *regLoc; 
    }string;
}mechin;

typedef struct data{
    unsigned int data: 14;
}data; 


int firstPass(char *argv,long int *IC,long int *DC);

int checkFristPass(symbolTable *s,long *IC,long *DC,ASTNode *temp,mechin *codeim,data *dataim,lineLoc *LineInf);
