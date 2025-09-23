int reg = 0;
int getreg(){
    return reg++;
}
void freereg(){
     reg--;
}
int codeGen(struct tnode *t,FILE *target_file) {
    if (t->op == NULL) {
        int p = getreg();
        fprintf(target_file, "MOV R%d, %d\n", p, t->val);
        return p;
    }

    int leftReg = codeGen(t->left, target_file);
    int rightReg = codeGen(t->right, target_file);

    switch (*(t->op)) {
        case '+': fprintf(target_file, "ADD R%d, R%d\n", leftReg, rightReg); break;
        case '-': fprintf(target_file, "SUB R%d, R%d\n", leftReg, rightReg); break;
        case '*': fprintf(target_file, "MUL R%d, R%d\n", leftReg, rightReg); break;
        case '/': fprintf(target_file, "DIV R%d, R%d\n", leftReg, rightReg); break;
    }

    freereg(); // free rightReg
    return leftReg;
}
int execute(struct tnode * t){

    FILE *target_file = fopen("target_file.xsm", "w"); //task1
    				if (!target_file) {
        				perror("fopen");
        				return 1;
    				}
					fprintf(target_file, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
            				0,    
            				2056,
            				0,
            				0,
            				0,
            				0,
            				0,
            				0);
					
					int result = codeGen(t,target_file);//task2
    				fprintf(target_file, "MOV [4096], R%d\n", result);
                    fprintf(target_file, "MOV R%d, [4096]\n", result+1); 
					fprintf(target_file, "MOV SP, 4096\n");
                    fprintf(target_file, "MOV R5, \"Write\"\n");
					fprintf(target_file, "PUSH R5\n");
					fprintf(target_file, "MOV R5, -2\n");
					fprintf(target_file, "PUSH R5\n");
					fprintf(target_file, "PUSH R%d \n", result+1);
					fprintf(target_file, "PUSH R0\n");
					fprintf(target_file, "PUSH R0\n");
					fprintf(target_file, "CALL 0\n");
					fprintf(target_file, "POP R0\n");
					fprintf(target_file, "POP R1\n");
					fprintf(target_file, "POP R1\n");
					fprintf(target_file, "POP R1\n");
					fprintf(target_file, "MOV R5, \"Exit\"\n");
					fprintf(target_file, "PUSH R5\n");
					fprintf(target_file, "PUSH R1\n");
					fprintf(target_file, "PUSH R4\n");
					fprintf(target_file, "PUSH R0\n");
					fprintf(target_file, "PUSH R0\n");
					fprintf(target_file, "CALL 0\n");
					fprintf(target_file, "POP R0\n");
					fprintf(target_file, "POP R1\n");
					fprintf(target_file, "POP R1\n");
					fprintf(target_file, "POP R1\n");
					fprintf(target_file, "POP R1\n");

					fclose(target_file);
					return 1;

}
