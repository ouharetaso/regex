#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "parser.h"

#define MAX_OPCODES  114514


int match(Token** ptok, enum Kind k){
    if ((*ptok)->kind == k){
        *ptok = (*ptok)->next;
        return 1;
    }
    fprintf(stderr, "syntax error\n");
    exit(1);
}


Node* Character(const char c){
    Node* node = malloc(sizeof(Node));

    node->kind = NODE_CHARACTER;
    node->c = c;
    node->l = NULL;
    node->r = NULL;

    return node;
}


Node* Concat(Node* node1, Node* node2){
    Node* node = malloc(sizeof(Node));

    node->kind = NODE_CONCAT;
    node->l = node1;
    node->r = node2;

    return node;
}


Node* Asterisk(Node* node1){
    Node* node = malloc(sizeof(Node));

    node->kind = NODE_ASTERISK;
    node->l = node1;
    node->r = NULL;

    return node;
}


Node* Union(Node* node1, Node* node2){
    Node* node = malloc(sizeof(Node));

    node->kind = NODE_UNION;
    node->l = node1;
    node->r = node2;

    return node;
}


/*
expr    := subexpr EOF
subexpr := seq '|' subexpr | seq
seq     := subseq | ''
subseq  := star subseq | star
star    := factor '*' | factor
factor  := '(' subexpr ')' | CHARACTER
*/


Node* factor(Token** ptok){
    // factor := '(' subexpr ')'
    if((*ptok)->kind == TOKEN_LPAR){
        match(ptok, TOKEN_LPAR);
        Node* node = subexpr(ptok);

        match(ptok, TOKEN_RPAR);

        return node;
    }

    // factor := CHARACTER
    else {
        Node* node = Character( *(*ptok)->s);

        match( ptok, TOKEN_CHARACTER);

        return node;
    }
}


Node* star(Token** ptok){
    // star    := factor '*' | factor
    Node* node = factor(ptok);
    if((*ptok)->kind == TOKEN_ASTERISK){
        match(ptok, TOKEN_ASTERISK);
        node = Asterisk(node);
    }

    return node;
}



Node* seq(Token** ptok){
    // seq     := subseq | ''
    if((*ptok)->kind == TOKEN_LPAR ||
       (*ptok)->kind == TOKEN_CHARACTER){
        // seq := subseq
        return subseq(ptok);
    }
    else{
        // seq := ''
        return Character('\0');
    }
}


Node* subseq(Token** ptok){
    Node* node1 = star(ptok);
    if((*ptok)->kind == TOKEN_LPAR ||
       (*ptok)->kind == TOKEN_CHARACTER){
        // subseq := star subseq
        Node* node2 = subseq(ptok);
        Node* node = Concat(node1, node2);
        return node;
    }
    else{
        // subseq := star
        return node1;
    }
}


Node* subexpr(Token** ptok){
    // subexpr := seq '|' subexpr | seq
    Node* node = seq(ptok);

    if((*ptok)->kind == TOKEN_BAR){
        match(ptok, TOKEN_BAR);
        Node* node2 = subexpr(ptok);
        node = Union(node, node2);
    }

    return node;
}


Node* expr(Token** ptok){
    // expr := subexpr EOF
    Node* node = subexpr(ptok);
    match(ptok, TOKEN_EOF);

    return node;
}


void freeNode(Node* node){
    if( node->l != NULL ){
        freeNode(node->l);
    }
    if( node->r != NULL ){
        freeNode(node->r);
    }

    free(node);
}