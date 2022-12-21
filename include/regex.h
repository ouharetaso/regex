#ifndef _REGEX_H
#define _REGEX_H

#include "parser.h"


#define REGEX_INST_MAX 114514

enum VM_opcode{
    Char = 0,
    Match,
    Jmp,
    Split
};


typedef struct __Inst Inst;

struct __Inst{
    enum VM_opcode opcode;
    char c;
    struct __Inst* x;
    struct __Inst* y;
};



int convert(Node* node, Inst* inst, int* pos);
int recursive(Inst* pc, char* sp);


#endif