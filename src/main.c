#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "parser.h"
#include "regex.h"


void printTree(Node* root, int depth){
    int i;

    if(root == NULL){
        return;
    }

    printTree(root->r, depth + 1);

    for(i = 0; i < depth; i++){
        printf("          ");
    }

    switch(root->kind){
        case NODE_UNION:
            printf("UNION     \n");
            break;
        case NODE_ASTERISK:
            printf("ASTERISK  \n");
            break;
        case NODE_CONCAT:
            printf("CONCAT    \n");
            break;
        case NODE_CHARACTER:
            printf("CHAR \'%c\'  \n", root->c);
            break;
    }

    printTree(root->l, depth + 1);

    depth++;
}


int main(int argc, char** argv){
    if(argc != 3){
        fprintf(stderr, "wrong argument\n");
        exit(1);
    }

    Token head;
    Node* root;
    
    tokenize(argv[1], &head);

    root = expr(&head.next);

    //printTree(root, 0);

    Inst inst[REGEX_INST_MAX];
    int pos = 0;

    convert(root, inst, &pos);

    for(int i = 0; i <= pos; i++){
        switch(inst[i].opcode){
            case Char:
                printf("%04d Char  %c\n", i, inst[i].c);
                break;
            case Split:
                printf("%04d Split %ld, %ld\n", i, (inst[i].x - inst), (inst[i].y - inst));
                break;
            case Jmp:
                printf("%04d Jmp   %ld\n", i, (inst[i].x - inst));
                break;
            case Match:
                printf("%04d Match\n", i);
                break;
        }
    }


    if(recursive(inst, argv[2])){
        printf("text matched\n");
    }
    else{
        printf("text not matched\n");
    }




    freeNode(root);
    freeToken(&head);


    return 0;
}