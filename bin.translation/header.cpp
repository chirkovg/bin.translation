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
#include <mach-o/loader.h>

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


FILE* makeheader(const lexem* source)
{
    FILE* res = fopen("/Users/chirkovg/res", "wb");
    //fwrite(&MH_MAGIC_64, 1, sizeof(MH_MAGIC_64), res);
    struct mach_header_64* header = (struct mach_header_64*) calloc (1, sizeof(*header));
    header -> magic = MH_MAGIC_64;
    header -> cputype = CPU_TYPE_X86_64;
    header -> cpusubtype = CPU_SUBTYPE_X86_64_ALL;
    header -> filetype = MH_EXECUTE;
    header -> ncmds = 1;
    header -> sizeofcmds = 72;
    header -> flags = MH_NOUNDEFS;
    header -> reserved = 0;
    fwrite(header, 1, sizeof(*header), res);
    
    struct segment_command_64* zeroseg = (struct segment_command_64*) calloc(1, sizeof(zeroseg));
    zeroseg -> cmd = LC_SEGMENT_64;
    zeroseg -> cmdsize = 72;
    //memcpy(zeroseg -> segname, SEG_PAGEZERO, 10);
    memmove(zeroseg -> segname, SEG_PAGEZERO, strlen(SEG_PAGEZERO));
    zeroseg -> vmaddr = 0;
    zeroseg -> vmsize = 4096;
    zeroseg -> fileoff = 0;
    zeroseg -> filesize = 0;
    zeroseg -> maxprot = VM_PROT_NONE;
    zeroseg -> initprot = VM_PROT_NONE;
    zeroseg -> nsects = 0;
    zeroseg -> flags = 0;
    fwrite(zeroseg, 1, sizeof(*zeroseg), res);
    
    size_t textsegsize = textsize(source);
    struct segment_command_64* textseg = (struct segment_command_64*) calloc(1, sizeof(textseg));
    textseg -> cmd = LC_SEGMENT_64;
    textseg -> cmdsize = 152;
    memmove(textseg -> segname, SEG_TEXT, strlen(SEG_TEXT));
    zeroseg -> vmaddr = 4096;
    uint64_t vmsize = 4096;
    
    while (vmsize < textsegsize)
        vmsize *= 2;
    zeroseg -> vmsize = vmsize;
    zeroseg -> fileoff = 0;
    zeroseg -> filesize = 
    
    
    return res;
}

