#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "token.h"



const char* token_kind_string[] = {
    "TOKEN_UNDEFINED",
    "TOKEN_EOF",
    "TOKEN_CHARACTER",
    "TOKEN_ASTERISK",
    "TOKEN_LPAR",
    "TOKEN_RPAR",
    "TOKEN_BAR"
};




void tokenize(char* s, Token* head){
    Token* cur;
    Token* tok;

    cur = head;
    while( *s != '\0'){
        if( isalnum(*s) ){
            tok = (Token*)malloc(sizeof(Token));
            tok->kind = TOKEN_CHARACTER;

            tok->s    = s;
            s++;

            tok->len = 1;
            tok->next = NULL;

            cur->next = tok;
            cur = tok;
        }

        else if(*s == '(' || *s == ')' || *s == '|' || *s == '*'){
            tok = (Token*)malloc(sizeof(Token));
            switch(*s){
                case '(':
                    tok->kind = TOKEN_LPAR;
                    break;
                case ')':
                    tok->kind = TOKEN_RPAR;
                    break;
                case '|':
                    tok->kind = TOKEN_BAR;
                    break;
                case '*':
                    tok->kind = TOKEN_ASTERISK;
                    break;                
            }
            tok->s    = s;
            s++;

            tok->len = 1;
            tok->next = NULL;

            cur->next = tok;
            cur = tok;
        }

        else if(*s == '\\'){
            s++;
            tok = (Token*)malloc(sizeof(Token));
            tok->kind = TOKEN_CHARACTER;
            tok->s    = s;
            s++;
            tok->len = 1;
            tok->next = NULL;

            cur->next = tok;
            cur = tok;
        }

        else{
            s++;
        }
    }
    tok = (Token*)malloc(sizeof(Token));
    tok->kind = TOKEN_EOF;
    tok->next = NULL;

    cur->next = tok;
}


void freeToken(Token* head){
    Token* tok;
    Token* next;

    tok = head->next;

    while(tok != NULL){
        next = tok->next;
        free(tok);
        tok = next;
    }
}