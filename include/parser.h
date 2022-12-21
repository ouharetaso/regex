#ifndef _PARSER_H
#define _PARSER_H

#include "token.h"

typedef struct __node Node;

enum NodeKind{
    NODE_UNION,
    NODE_ASTERISK,
    NODE_CONCAT,
    NODE_CHARACTER
};


struct __node{
    enum NodeKind kind;
    char c;
    Node* l;
    Node* r;
};


/*
expr    := subexpr EOF
subexpr := seq '|' subexpr | seq
seq     := subseq | ''
subseq  := star subseq | star
star    := factor '*' | factor
factor  := '(' subexpr ')' | CHARACTER
*/



Node* expr(Token** ptok);
Node* subexpr(Token** ptok);
Node* seq(Token** ptok);
Node* subseq(Token** ptok);
Node* star(Token** ptok);
Node* factor(Token** ptok);

void freeNode(Node* node);
#endif