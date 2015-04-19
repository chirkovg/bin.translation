//
//  header.cpp
//  bin.translation
//
//  Created by Григорий Чирков on 19.04.15.
//  Copyright (c) 2015 Григорий Чирков. All rights reserved.
//

#include "header.h"
#include "codification.h"
#include "reader.h"

size_t textsize(const lexem* source)
{
    const lexem* ptr = source;
    size_t res = 0;
    while (ptr -> mark != WRONGOP)
    {
        if (ptr -> op <= 100) ptr++;
        else if (ptr -> op == PUSH)
        {
            operation pushop(PUSH);
            res += pushop.getsize();
            ptr += 2;
        }
        else if (ptr -> op >= 116 && ptr -> op <= 123)
        {
            operation curop(ptr -> op);
            res += curop.getsize();
            ptr += 2;
        }
        else if(ptr -> op >= 101 && ptr -> op <= 115)
        {
            operation curop(ptr -> op);
            res += curop.getsize();
            ptr++;
        }
        else break;
    }
    return res;
}

size_t marksize(const lexem* source)
{
    const lexem* ptr = source;
    size_t res = 0;
    while (ptr -> mark != WRONGOP)
    {
        if (0 <= ptr -> mark && ptr -> mark <= 100)
        {
            res++;
            ptr++;
        }
        else if (ptr -> op == PUSH)
        {
            ptr += 2;
        }
        else ptr++;
    }
    return res*8;
}

size_t doublesize(const lexem* source)
{
    const lexem* ptr = source;
    size_t res = 0;
    while (ptr -> mark != WRONGOP)
    {
        if (ptr -> op == PUSH)
        {
            res++;
            ptr += 2;
        }
        else if (ptr -> op >= 116) ptr += 2;
        else ptr++;
    }
    return res*8;
}

