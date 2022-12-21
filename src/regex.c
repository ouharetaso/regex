#include "regex.h"
#include "token.h"
#include "parser.h"



int convert(Node* node, Inst* inst, int* pos){
    switch(node->kind){
        case NODE_CHARACTER:
            (inst + *pos)->opcode = Char;
            (inst + *pos)->c      = node->c;
            (*pos)++;
            break;
        case NODE_CONCAT:
            convert(node->l, inst, pos);
            convert(node->r, inst, pos);
            break;
        /*
        UNION (e1 | e2)

            Split L1, L2
        L1: e1
            Jmp L3
        L2: e2
        L3:

        */
        case NODE_UNION:{
            int L1, L2, L3, L_Split, L_Jmp;

            (inst + *pos)->opcode = Split;
            L_Split = *pos;
            (*pos)++;
            L1 = *pos;

            convert(node->l, inst, pos);
            (inst + *pos)->opcode = Jmp;
            L_Jmp = *pos;
            (*pos)++;
            L2 = *pos;

            convert(node->r, inst, pos);
            L3 = *pos;
            (inst + L_Jmp)->x  = inst + L3;
            (inst + L_Split)->x = inst + L1;
            (inst + L_Split)->y = inst + L2;
            }
            break;
        case NODE_ASTERISK:{
            int L1, L2, L3;

            L1 = *pos;
            (inst + *pos)->opcode = Split;
            (*pos)++;
            L2 = *pos;

            convert(node->l, inst, pos);

            (inst + *pos)->opcode = Jmp;
            (inst + *pos)->x = inst + L1;
            (*pos)++;
            L3 = *pos;

            (inst + L1)->x = inst + L2;
            (inst + L1)->y = inst + L3;
            }
            break;
    }

    (inst + *pos)->opcode = Match;

    return 0;
}


int recursive(Inst* pc, char* sp){
    switch(pc->opcode){
        case Char:
            if(*sp != pc->c) return 0;
            return recursive(pc+1, sp+1);
            break;
        case Match:
            return 1;
            break;
        case Jmp:
            return recursive(pc->x, sp);
            break;
        case Split:
            if(recursive(pc->x, sp)) return 1;
            return recursive(pc->y, sp);
            break;
    }
    return 0;
}