#define tNUM 0
#define tVAR 1
#define tADD 2
#define tSUB 3
#define tMUL 4
#define tDIV 5
#define tREAD 6
#define tWRITE 7
#define tASSIGN 8
#define tCONNECT 9
#define tINT 10
#define tLT 11
#define tGT 12
#define tLE 13
#define tGE 14
#define tNE 15
#define tEQ 16
#define tBOOL 17
#define tWHILE 18
#define tIF 19
#define tBREAK 20
#define tCONTINUE 21
#define varLoc 4096
#define SPLoc 4121


typedef struct tnode { 
	int val;	// value of a number for NUM nodes.
	int type;	//type of variable
	char* symbol;	//name of a variable for ID nodes  
	int nodetype;  // information about non-leaf nodes - read/write/connector/+/* etc.  
	struct tnode *left,*right,*third;	//left and right branches   
}tnode;
	
void printExit(FILE* targetFile);
int getLabel();
int getReg();
void freeReg();
void printRead(FILE* targetFile, int varAddr);
void printWrite(FILE* targetFile, int regNum);
int getVarAddr(struct tnode*root);
int codeGen(FILE* fp, struct tnode* root);
void infixtoprefix(FILE* fp, struct tnode* root);


/*Create a node tnode*/
struct tnode* createTree(int val, int type, char* c, int nodeType, struct tnode *l, struct tnode *r,struct tnode *third);