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


FILE* translate(const lexem* source)
{
    FILE* res = fopen("/Users/chirkovg/res", "wb");
    
    struct mach_header_64* header = makeheader();
    fwrite(header, 1, sizeof(*header), res);
    
    struct segment_command_64* zeroseg = makezeroseg();
    fwrite(zeroseg, 1, sizeof(*zeroseg), res);
    
    size_t textsectsize = textsize(source);
    struct segment_command_64* textseg = maketextseg(textsectsize, zeroseg -> vmsize);
    fwrite(textseg, 1, sizeof(*textseg), res);
    
    struct section_64* textsect = maketextsect(zeroseg -> vmsize, textsectsize);
    fwrite(textsect, 1, sizeof(*textsect), res);
    
    
    
    
    
    
    size_t datadoublesize = doublesize(source);
    size_t datamarksize = marksize(source);
    struct segment_command_64* dataseg = makedataseg(textsect -> addr + textsect -> size,
                                                     textsect -> offset + textsect -> size,
                                                     datadoublesize, datamarksize);
    fwrite(dataseg, 1, sizeof(*dataseg), res);
    
    struct section_64* datasect = makedatasect(textsect -> addr + textsect -> size,
                                               datadoublesize + datamarksize,
                                               textsect -> offset + textsect -> size);
    fwrite(datasect, 1, sizeof(*datasect), res);


    
    const lexem* ptr = source;
    while (ptr -> mark)
    {
        if (ptr -> mark < END) ptr++;
        else
        {
            operation op (ptr -> op);
            if (ptr -> op >= JE)
            {
                ptr++;
                uint32_t mark = ptr -> mark;
                op.printop(res, mark);
            }
            else
            {
                op.printop(res, 0);
                if (ptr -> op == PUSH)
                    ptr++;
            }
            ptr++;
        }
    }
    
    return res;
}

struct mach_header_64* makeheader()
{
    struct mach_header_64* header = (struct mach_header_64*) calloc (1, sizeof(*header));
    header -> magic = MH_MAGIC_64;
    header -> cputype = CPU_TYPE_X86_64;
    header -> cpusubtype = CPU_SUBTYPE_X86_64_ALL;
    header -> filetype = MH_EXECUTE;
    header -> ncmds = 3;
    header -> sizeofcmds = 376;
    header -> flags = MH_NOUNDEFS;
    header -> reserved = 0;
    return header;
}


struct segment_command_64* makezeroseg()
{
    struct segment_command_64* zeroseg = (struct segment_command_64*) calloc(1, sizeof(*zeroseg));
    zeroseg -> cmd = LC_SEGMENT_64;
    zeroseg -> cmdsize = sizeof(struct segment_command_64);
    memmove(zeroseg -> segname, SEG_PAGEZERO, sizeof(SEG_PAGEZERO));
    zeroseg -> vmaddr = 0;
    zeroseg -> vmsize = 4096;
    zeroseg -> fileoff = 0;
    zeroseg -> filesize = 0;
    zeroseg -> maxprot = VM_PROT_NONE;
    zeroseg -> initprot = VM_PROT_NONE;
    zeroseg -> nsects = 0;
    zeroseg -> flags = 0;
    return zeroseg;
}

struct segment_command_64* maketextseg(size_t size, uint64_t vmaddr)
{
    
    struct segment_command_64* textseg = (struct segment_command_64*) calloc(1, sizeof(*textseg));
    textseg -> cmd = LC_SEGMENT_64;
    textseg -> cmdsize = sizeof(struct segment_command_64) + sizeof(struct section_64);
    memmove(textseg -> segname, SEG_TEXT, sizeof(SEG_TEXT));
    textseg -> vmaddr = vmaddr;
    textseg -> vmsize = size;
    textseg -> fileoff = sizeof(struct mach_header_64) +
                         3*sizeof(struct segment_command_64) +
                         2*sizeof(struct section_64);
    textseg -> filesize = size;
    textseg -> maxprot = VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE;
    textseg -> initprot = VM_PROT_READ | VM_PROT_EXECUTE;
    textseg -> nsects = 1;
    textseg -> flags = 0;
    return textseg;
}

struct section_64* maketextsect(uint64_t addr, size_t size)
{
    struct section_64* textsect = (struct section_64*) calloc(1, sizeof(*textsect));
    memmove(textsect -> sectname, SECT_TEXT, sizeof(SECT_TEXT));
    memmove(textsect -> segname, SEG_TEXT, sizeof(SEG_TEXT));
    textsect -> addr = addr;//zeroseg -> vmsize
    textsect -> size = size;
    textsect -> offset = sizeof(struct mach_header_64) +
                         3*sizeof(segment_command_64) +
                         2*sizeof(section_64);
    textsect -> align = 0;
    textsect -> reloff = 0;
    textsect -> nreloc = 0;
    textsect -> flags = S_REGULAR | S_ATTR_SOME_INSTRUCTIONS | S_ATTR_PURE_INSTRUCTIONS;
    textsect -> reserved1 = 0;
    textsect -> reserved2 = 0;
    textsect -> reserved3 = 0;
    return textsect;
}


struct segment_command_64* makedataseg(uint64_t addr, uint64_t offset,
                                       size_t doublesize, size_t marksize)
{
    struct segment_command_64* dataseg = (struct segment_command_64*) calloc(1, sizeof(*dataseg));
    dataseg -> cmd = LC_SEGMENT_64;
    dataseg -> cmdsize = sizeof(struct segment_command_64) + sizeof(struct section_64);
    memmove(dataseg -> segname, SEG_DATA, sizeof(SEG_DATA));
    dataseg -> vmaddr = addr;
    dataseg -> vmsize = doublesize + marksize;
    dataseg -> fileoff = offset;
    dataseg -> filesize = doublesize + marksize;
    dataseg -> maxprot = VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE;
    dataseg -> initprot = VM_PROT_READ | VM_PROT_WRITE;
    dataseg -> nsects = 1;
    dataseg -> flags = 0;
    return dataseg;
}

struct section_64* makedatasect(uint64_t addr, uint64_t size, uint32_t offset)
{
    struct section_64* datasect = (struct section_64*) calloc(1, sizeof(*datasect));
    memmove(datasect -> segname, SEG_DATA, sizeof(SEG_DATA));
    memmove(datasect -> sectname, SECT_DATA, sizeof(SECT_DATA));
    datasect -> addr = addr;
    datasect -> size = size;
    datasect -> offset = offset;
    datasect -> align = 0;
    datasect -> reloff = 0;
    datasect -> nreloc = 0;
    datasect -> flags = 0;
    datasect -> reserved1 = 0;
    datasect -> reserved2 = 0;
    datasect -> reserved3 = 0;
    return datasect;
}

















