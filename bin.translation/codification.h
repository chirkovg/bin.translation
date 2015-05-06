//
//  codification.h
//  Stack_proc
//
//  Created by Григорий Чирков on 26.03.15.
//  Copyright (c) 2015 Григорий Чирков. All rights reserved.
//

#ifndef Stack_proc_codification_h
#define Stack_proc_codification_h

#include <cstdlib>
#include <cstdio>
#include <stdint.h>
#include <iostream>
#include <assert.h>
#include "x86_cmds.h"


enum OP {EOC = 0, WRONGOP = 0, END = 101, ADD, SUB,
    DIV, MUL, RET, PUSHAX, PUSHBX,
    PUSHCX, PUSHDX, POPAX, POPBX, POPCX,
    POPDX, PRINT, SCAN, PUSH, JE, JNE, JA, JAE, JB, JBE, JMP, CALL = 125};
#define MAXMARK END - 1
#define MAXOP CALL
const uint64_t NOMARK = 0;
#define WRONGMARK 0

class operation
{
public:
    operation(OP op);
    size_t printop(FILE* dest, uint32_t mark);
    size_t getsize();
private:
    size_t size;
    OP op;
    const uint8_t* instr;
};


#endif

