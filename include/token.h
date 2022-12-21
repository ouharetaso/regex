#ifndef _TOKEN_H
#define _TOKEN_H

enum Kind{
    TOKEN_UNDEFINED = 0,
    TOKEN_EOF,
    TOKEN_CHARACTER,
    TOKEN_ASTERISK,
    TOKEN_LPAR,
    TOKEN_RPAR,
    TOKEN_BAR
};


extern const char* token_kind_string[];

typedef struct __token Token;

typedef struct __token{
    enum Kind kind;
    char* s;
    unsigned len;
    Token* next;
}_token;

void tokenize(char* s, Token* head);
void freeToken(Token* head);



#endif