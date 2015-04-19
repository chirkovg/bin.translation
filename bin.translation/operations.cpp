//
//  operations.cpp
//  bin.translation
//
//  Created by Григорий Чирков on 11.04.15.
//  Copyright (c) 2015 Григорий Чирков. All rights reserved.
//

#include <stdio.h>
#include "codification.h"
#include <iostream>
#include <cstdlib>


operation::operation(OP op)
{
    op = op;
    switch (op)
    {
        case WRONGOP:
        {
            puts("ERROR:WRONGOP");
            abort();
        }
        case END:
        {
            size = 13;
            instr = exitinst;
            break;
        }
        case ADD:
        {
            size = 19;
            instr = addinst;
            break;
        }
        case DIV:
        {
            size = 19;
            instr = divinst;
            break;
        }
        case MUL:
        {
            size = 19;
            instr = mulinst;
            break;
        }
        case SUB:
        {
            size = 19;
            instr = subinst;
            break;
        }
        case JE:
        {
            size = 23;
            instr = jeinst;
            break;
        }
        case JNE:
        {
            size = 23;
            instr = jneinst;
            break;
        }
        case JA:
        {
            size = 23;
            instr = jainst;
            break;
        }
        case JAE:
        {
            size = 23;
            instr = jaeinst;
            break;
        }
        case JB:
        {
            size = 23;
            instr = jbinst;
            break;
        }
        case JBE:
        {
            size = 23;
            instr = jbeinst;
            break;
        }
        case JMP:
        {
            size = 7;
            instr = jmpinst;
            break;
        }
        case PUSH:
        {
            size = 18;
            instr = pushinst;
            break;
        }
        case PUSHAX:
        {
            size = 9;
            instr = pushaxinst;
            break;
        }
        case PUSHBX:
        {
            size = 9;
            instr = pushbxinst;
            break;
        }
        case PUSHCX:
        {
            size = 9;
            instr = pushcxinst;
            break;
        }
        case PUSHDX:
        {
            size = 9;
            instr = pushdxinst;
            break;
        }
        case POPAX:
        {
            size = 9;
            instr = popaxinst;
            break;
        }
        case POPBX:
        {
            size = 9;
            instr = popbxinst;
            break;
        }
        case POPCX:
        {
            size = 9;
            instr = popcxinst;
            break;
        }
        case POPDX:
        {
            size = 9;
            instr = popdxinst;
            break;
        }
        case CALL:
        {
            size = 7;
            instr = callinst;
            break;
        }
        case RET:
        {
            size = 1;
            instr = retinst;
            break;
        }
        
    }
}

size_t operation::printop(FILE* dest, uint32_t mark)
{
    if (op <= 100) return 0;
    else if (op < 115)
    {
        fwrite(instr, size, sizeof(uint8_t), dest);
        return size;
    }
    else if (op <= 123)
    {
        fwrite(instr, size - 4, sizeof(uint8_t), dest);
        uint32_t addr = 0x8*mark;
        fwrite(&addr, 1, sizeof(addr), dest);
        return size;
    }
    else return 0;
}