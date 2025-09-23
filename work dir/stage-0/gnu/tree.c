int reg=0;
int label_no=0;
int stacktop[20];	//for continue
int stackend[20];	//for break
int stack1=-1;		//for stacktop
int stack2=-1;		//for stackend
struct tnode* createTree(int val, int type, char* c, int nodeType, struct tnode *l, struct tnode *r, struct tnode* third){
    switch(nodeType){
    	case tADD:
    	case tSUB:
    	case tMUL:
    	case tDIV:
    		if(l->type!= r->type || l->type != tINT){
    			printf("\ntype mismatch error\n");
    			exit(0);
    		}
    		else{
    			type=tINT;
    		}
    		break;
    	case tLT:
    	case tGT:
    	case tLE:
    	case tGE:
    	case tNE:
    	case tEQ:
    		if(l->type!= r->type || l->type !=tINT){
    			printf("\ntype mismatch error\n");
    			exit(0);
    		}
    		else{
    			type=tBOOL;
    		
    		}break;
    	case tASSIGN:
    		if(r->type!=tINT || l->nodetype!=tVAR){
    			printf("\ntype mismatch error\n");
    			exit(0);
    		}
    		break;
    	case tWHILE:
    	case tIF:
    		if(l->type!=tBOOL){
    			printf("\ntype mismatch error\n");
    			exit(0);
    		}
    		break;
    }
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->val = val;
    temp->type = type;
    temp->symbol = c;
    temp->nodetype = nodeType;
    temp->left=l;
    temp->right = r;
    temp->third=third;
    return temp;
}



void printExit(FILE* targetFile){
	fprintf(targetFile,"INT 10\n");
}
int getLabel(){
	int curr_label=label_no;
	label_no++;
	return curr_label;
}

int getReg(){
	int r=reg;
	reg++;
	if(r>20){
		printf("out of registers\n");
		exit(1);
	}
	return r;	
}
void freeReg(){
	reg--;
	if(reg<0){
		reg=0;
	}	
}
void printRead(FILE* targetFile, int varAddr){
	int r=getReg();
	fprintf(targetFile,"MOV R%d, \"Read\"\nPUSH R%d\nMOV R%d, -1\nPUSH R%d\nMOV R%d,%d\nPUSH R%d\nPUSH R%d\nPUSH R%d\nCALL 0\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n",r,r,r,r,r,varAddr,r,r,r,r,r,r,r,r);
	freeReg();
}

void printWrite(FILE* targetFile, int regNum){
	int r=getReg();
	fprintf(targetFile,"MOV R%d, \"Write\"\nPUSH R%d\nMOV R%d, -2\nPUSH R%d\nMOV R%d,R%d\nPUSH R%d\nPUSH R%d\nPUSH R%d\nCALL 0\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nBRKP\n",r,r,r,r,r,regNum,r,r,r,r,r,r,r,r);
	freeReg();
}

int getVarAddr(struct tnode*root){
	char* symbol=root->symbol;
	int varAddr = 4096 + (symbol[0] - 'a');
	return varAddr;
}
void infixtoprefix(FILE* fp, struct tnode* root){
	if(root!=NULL){
		printf("%s ",root->symbol);
		fprintf(fp,"%s ",root->symbol);
		infixtoprefix(fp,root->left);
		infixtoprefix(fp,root->right);
	}
}
int codeGen(FILE* fp, struct tnode* root){
	int r,sourceReg,destReg,varAddr,label_1,label_2,label_3;
	char* symbol;
	if(root==NULL) return -1;
	switch(root->nodetype){
		case tCONNECT: 
				codeGen(fp,root->left);
				codeGen(fp,root->right);
				return -1;
		case tREAD:
				if(root->left==NULL) {printf("\ninvalid read stmt\n");exit(0);}
				symbol = (root->left)->symbol;
				varAddr= 4096 + (symbol[0] - 'a');
				//printf("var addr = %d\n",varAddr);
				printRead(fp,varAddr);
				return -1;
		case tNUM: 
				r=getReg();
				fprintf(fp,"MOV R%d, %d\n",r,root->val);
				return r;
		case tVAR:
				r=getReg();
				symbol=root->symbol;
				varAddr = 4096 + (symbol[0] - 'a');
				fprintf(fp,"MOV R%d,[%d]\n",r,varAddr);
				return r;
		case tWRITE:	
				r=codeGen(fp,root->left);
				printWrite(fp,r);
				return -1;
		case tASSIGN:
				sourceReg=getVarAddr(root->left);
				destReg=codeGen(fp,root->right);
				fprintf(fp,"MOV [%d],R%d\n",sourceReg,destReg);
				return -1;
		case tADD:
				sourceReg=codeGen(fp,root->left);
				destReg=codeGen(fp,root->right);
				fprintf(fp,"ADD R%d,R%d\n",sourceReg,destReg);
				freeReg();
				return sourceReg;
		case tSUB:
				sourceReg=codeGen(fp,root->left);
				destReg=codeGen(fp,root->right);
				fprintf(fp,"SUB R%d,R%d\n",sourceReg,destReg);
				freeReg();
				return sourceReg;
		case tMUL:
				sourceReg=codeGen(fp,root->left);
				destReg=codeGen(fp,root->right);
				fprintf(fp,"MUL R%d,R%d\n",sourceReg,destReg);
				freeReg();
				return sourceReg;
		case tDIV:
				sourceReg=codeGen(fp,root->left);
				destReg=codeGen(fp,root->right);
				fprintf(fp,"DIV R%d,R%d\n",sourceReg,destReg);
				freeReg();
				return sourceReg;
		case tLT:	
				sourceReg=codeGen(fp,root->left);
				destReg=codeGen(fp,root->right);
				fprintf(fp,"LT R%d, R%d\n",sourceReg,destReg);
				freeReg();
				return sourceReg;
		case tGT:	
				sourceReg=codeGen(fp,root->left);
				destReg=codeGen(fp,root->right);
				fprintf(fp,"GT R%d, R%d\n",sourceReg,destReg);
				freeReg();
				return sourceReg;
		case tLE:	
				sourceReg=codeGen(fp,root->left);
				destReg=codeGen(fp,root->right);
				fprintf(fp,"LE R%d, R%d\n",sourceReg,destReg);
				freeReg();
				return sourceReg;
		case tGE:	
				sourceReg=codeGen(fp,root->left);
				destReg=codeGen(fp,root->right);
				fprintf(fp,"GE R%d, R%d\n",sourceReg,destReg);
				freeReg();
				return sourceReg;
		case tNE:	
				sourceReg=codeGen(fp,root->left);
				destReg=codeGen(fp,root->right);
				fprintf(fp,"NE R%d, R%d\n",sourceReg,destReg);
				freeReg();
				return sourceReg;
		case tBREAK:
				if(stack2<0) {printf("\ninvalid Break statement\n");exit(0);}
				fprintf(fp,"JMP L%d\n",stackend[stack2]);
				return -1;
		case tCONTINUE:
				if(stack1<0) {printf("\ninvalid continue statement\n");exit(0);}
				fprintf(fp,"JMP L%d\n",stacktop[stack1]);
				return -1;
		case tEQ:	
				sourceReg=codeGen(fp,root->left);
				destReg=codeGen(fp,root->right);
				fprintf(fp,"EQ R%d, R%d\n",sourceReg,destReg);
				freeReg();
				return sourceReg;	
			
		case tWHILE:
				label_1 = getLabel();
				label_2 = getLabel();
				fprintf (fp, "L%d:\n", label_1); // Place the first label here.
				stack1++;
				stacktop[stack1]=label_1;
				stack2++;
				stackend[stack2]=label_2;
				sourceReg=codeGen(fp,root->left);
				fprintf(fp,"JZ R%d, L%d\n",sourceReg,label_2);
				codeGen(fp,root->right);
				fprintf(fp, "JMP L%d\n", label_1); // return to the beginning of the loop.
				fprintf(fp, "L%d:\n", label_2); // Place the second label here
				stack1--;
				stack2--;
				return -1;
		case tIF:
				//label_1 = getLabel();
				if(root->third!=NULL)
					label_2 = getLabel();
				label_3 = getLabel();
				//fprintf (fp, "L%d:\n", label_1); // Place the first label here.
				//if
				sourceReg=codeGen(fp,root->left);
				if(root->third!=NULL)
					fprintf(fp,"JZ R%d, L%d\n",sourceReg,label_2);
				else
					fprintf(fp,"JZ R%d, L%d\n",sourceReg,label_3);
				//then
				codeGen(fp,root->right);
				fprintf(fp, "JMP L%d\n", label_3); // jump to endif.
				//else
				if(root->third!=NULL){
					fprintf(fp, "L%d:\n", label_2); // Place the second label here
					//else
					codeGen(fp,root->third);
					fprintf(fp, "JMP L%d\n", label_3); // jump to endif.	
				}
				fprintf(fp, "L%d:\n", label_3); // Place the third label here
				return -1;
		defualt:
				printf("\nsome error in codegen fn : %d\n",root->nodetype);
				exit(0);		
	}
}