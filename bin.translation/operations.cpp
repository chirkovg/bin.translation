//
//  operations.cpp
//  bin.translation
//
//  Created by Григорий Чирков on 11.04.15.
//  Copyright (c) 2015 Григорий Чирков. All rights reserved.
//

#include "codification.h"

operation::operation(OP op)
{
    this -> op = op;
#define MAKENOJMPOP(INST) \
size = sizeof(INST);\
instr = INST;\
break;
    
#define MAKEJMPOP(INST)\
size = sizeof(INST) + 4; \
instr = INST;\
break;
    
#define MAKEOP(INST) \
if (op < JE) \
    {\
    MAKENOJMPOP(INST)\
    }\
else\
{\
    MAKEJMPOP(INST)\
}\
break;


    switch (op)
    {
        case WRONGOP:
        {
            assert(WRONGOP);
            break;
        }
        case END:
        {
            MAKEOP(EXITINST);
        }
        case ADD:
        {
            MAKEOP(ADDINST);
        }
        case DIV:
        {
            MAKEOP(DIVINST);
        }
        case MUL:
        {
            MAKEOP(MULINST);
        }
        case SUB:
        {
            MAKEOP(SUBINST);
        }
        case JE:
        {
            MAKEOP(JEINST);
        }
        case JNE:
        {
            MAKEOP(JNEINST);
        }
        case JA:
        {
            MAKEOP(JAINST);
        }
        case JAE:
        {
            MAKEOP(JAEINST);
        }
        case JB:
        {
            MAKEOP(JBINST);
        }
        case JBE:
        {
            MAKEOP(JBEINST);
        }
        case JMP:
        {
            MAKEOP(JMPINST);
        }
        case PUSH:
        {
            MAKEOP(PUSHINST);
        }
        case PUSHAX:
        {
            MAKEOP(PUSHAXINST);
        }
        case PUSHBX:
        {
            MAKEOP(PUSHBXINST);
        }
        case PUSHCX:
        {
            MAKEOP(PUSHCXINST);
        }
        case PUSHDX:
        {
            MAKEOP(PUSHDXINST);
        }
        case POPAX:
        {
            MAKEOP(POPAXINST);
        }
        case POPBX:
        {
            MAKEOP(POPBXINST);
        }
        case POPCX:
        {
            MAKEOP(POPCXINST);
        }
        case POPDX:
        {
            MAKEOP(POPDXINST);
        }
        case CALL:
        {
            MAKEOP(CALLINST);
        }
        case RET:
        {
            MAKEOP(RETINST);
        }
        case PRINT:
        {
            MAKEOP(PRINTINST);
        }
        case SCAN:
        {
            MAKEOP(SCANINST);
        }
        default:
        {
            assert(WRONGOP);
            break;
        }
    }
#undef MAKEOP
#undef MAKEJMPOP
#undef MAKENOJMPOP
}

size_t operation::printop(FILE* dest, uint32_t mark)
{
    if (op < END) return 0;
    else if (op == PUSH)
    {
        fwrite(instr, 9, sizeof(uint8_t), dest);
        uint32_t addr = mark*8;
        fwrite(&addr, 1, sizeof(uint32_t), dest);
        fwrite(instr + 13, 5, sizeof(uint8_t), dest);
        return size;
    }
    else if (op < JE)
    {
        fwrite(instr, size, sizeof(uint8_t), dest);
        return size;
    }
    else if (op <= CALL)
    {
        fwrite(instr, size - 4, sizeof(uint8_t), dest);
        uint32_t addr = sizeof(double)*mark;
        fwrite(&addr, 1, sizeof(addr), dest);
        return size;
    }
    else return 0;
}


size_t operation::getsize()
{
    return size;
}