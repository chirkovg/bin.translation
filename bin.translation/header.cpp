//
//  header.cpp
//  bin.translation
//
//  Created by Григорий Чирков on 19.04.15.
//  Copyright (c) 2015 Григорий Чирков. All rights reserved.
//

#include "header.h"

size_t textsize(const lexem* source)
{
    assert(source);
    const lexem* ptr = source;
    size_t res = 0;
    while (ptr -> mark != EOC)
    {
        if (ptr -> mark <= MAXMARK) ptr++;
        else if (ptr -> op == PUSH)
        {
            operation curop(PUSH);
            res += curop.getsize();
            ptr += 2;
        }
        else if (ptr -> op >= JE && ptr -> op <= CALL)
        {
            operation curop(ptr -> op);
            res += curop.getsize();
            ptr += 2;
        }
        else if(ptr -> op >= END && ptr -> op <= SCAN)
        {
            operation curop(ptr -> op);
            res += curop.getsize();
            ptr++;
        }
        else
        {
            assert(WRONGOP);
        }
    }
    return res;
}

/*size_t marksize(const lexem* source)
{
    assert(source);
    const lexem* ptr = source;
    size_t res = 0;
    while (ptr -> mark != EOC)
    {
        if (0 <= ptr -> mark && ptr -> mark <= MAXMARK)
        {
            res++;
            ptr++;
        }
        else if (ptr -> op >= PUSH && ptr -> op <= CALL)
        {
            ptr += 2;
        }
        else if (ptr -> op < PUSH && ptr -> op >= END)
        {
            ptr++;
        }
        else
        {
            assert(WRONGOP);
        }
    }
    return res*8;
}*/

size_t doublesize(const lexem* source)
{
    assert(source);
    const lexem* ptr = source;
    size_t res = 1;
    while (ptr -> mark != EOC)
    {
        if (ptr -> op == PUSH)
        {
            res++;
            ptr += 2;
        }
        else if (ptr -> op >= JE) ptr += 2;
        else if (ptr -> op > MAXOP)
        {
            assert(WRONGOP);
        }
        else ptr++;
    }
    return res*8;
}


void translate(const lexem* source, const char* output)
{
    assert(source);
    FILE* res = nullptr;
    if (output == nullptr)
        res = fopen("a.out", "wb");
    else
        res = fopen(output, "wb");
    assert(res);
    
    struct mach_header_64* header = makeheader();
    assert(header);
    fwrite(header, 1, sizeof(*header), res);
    
    struct segment_command_64* zeroseg = makezeroseg();
    assert(zeroseg);
    fwrite(zeroseg, 1, sizeof(*zeroseg), res);
    
    size_t textsectsize = textsize(source);
    struct segment_command_64* textseg = maketextseg(textsectsize, zeroseg -> vmsize);
    assert(textseg);
    fwrite(textseg, 1, sizeof(*textseg), res);
    
    struct section_64* textsect = maketextsect(textsectsize, textseg -> filesize);
    assert(textsect);
    fwrite(textsect, 1, sizeof(*textsect), res);
    
    
    size_t datadoublesize = doublesize(source);
    size_t datamarksize = MARKDATASIZE;
    struct segment_command_64* dataseg = makedataseg(textseg -> vmsize + ALIGNNUM, textseg -> vmsize, datadoublesize + datamarksize);
    assert(dataseg);
    fwrite(dataseg, 1, sizeof(*dataseg), res);
    
    
    struct section_64* datasect = makedatasect(dataseg -> vmaddr,
                                               datadoublesize + datamarksize,
                                               dataseg -> fileoff);
    assert(datasect);
    fwrite(datasect, 1, sizeof(*datasect), res);
    
    void* thread = makethread(textsect -> addr, datasect -> addr + MARKDATASIZE,
                              datasect -> addr, datasect -> addr + MARKDATASIZE + datadoublesize);
    fwrite(thread, 1, sizeof(struct thread_command) + sizeof(struct x86_thread_state), res);
    
    filltextsect(source, res, textsectsize, textseg -> filesize);
    filldatasect(source, textsect -> addr, res, dataseg -> filesize, datasect -> size);
    
    fclose(res);
    free(header);
    free(zeroseg);
    free(textseg);
    free(textsect);
    free(dataseg);
    free(datasect);
    free(thread);
}

struct mach_header_64* makeheader()
{
    struct mach_header_64* header = (struct mach_header_64*) calloc (1, sizeof(*header));
    assert(header);
    header -> magic = MH_MAGIC_64;
    header -> cputype = CPU_TYPE_X86_64;
    header -> cpusubtype = CPU_SUBTYPE_X86_64_ALL;
    header -> filetype = MH_EXECUTE;
    header -> ncmds = 4;
    header -> sizeofcmds = 3*sizeof(struct segment_command_64) + 2*sizeof(struct section_64) + sizeof(struct thread_command) + sizeof(struct x86_thread_state);
    header -> flags = MH_NOUNDEFS;
    header -> reserved = 0;
    return header;
}


struct segment_command_64* makezeroseg()
{
    struct segment_command_64* zeroseg = (struct segment_command_64*) calloc(1, sizeof(*zeroseg));
    assert(zeroseg);
    zeroseg -> cmd = LC_SEGMENT_64;
    zeroseg -> cmdsize = sizeof(struct segment_command_64);
    memmove(zeroseg -> segname, SEG_PAGEZERO, sizeof(SEG_PAGEZERO));
    zeroseg -> vmaddr = 0;
    zeroseg -> vmsize = ALIGNNUM;
    zeroseg -> fileoff = 0;
    zeroseg -> filesize = 0;
    zeroseg -> maxprot = VM_PROT_NONE;
    zeroseg -> initprot = VM_PROT_NONE;
    zeroseg -> nsects = 0;
    zeroseg -> flags = 0;
    return zeroseg;
}

struct segment_command_64* maketextseg(size_t textsectsize, uint64_t vmaddr)
{
    
    struct segment_command_64* textseg = (struct segment_command_64*) calloc(1, sizeof(*textseg));
    assert(textseg);
    textseg -> cmd = LC_SEGMENT_64;
    textseg -> cmdsize = sizeof(struct segment_command_64) + sizeof(struct section_64);
    memmove(textseg -> segname, SEG_TEXT, sizeof(SEG_TEXT));
    textseg -> vmaddr = vmaddr;
    uint64_t vmsize = ALIGNNUM;
    
    size_t loadcmdsize = sizeof(struct mach_header_64) +
                         3*sizeof(struct segment_command_64) +
                         2*sizeof(struct section_64) + sizeof(thread_command) +
                         sizeof(x86_thread_state);
    
    while (vmsize < textsectsize + loadcmdsize)
    {
        vmsize += ALIGNNUM;
    }
    
    textseg -> vmsize = vmsize;
    textseg -> fileoff = 0;
    textseg -> filesize = vmsize;
    textseg -> maxprot = VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE;
    textseg -> initprot = VM_PROT_READ | VM_PROT_EXECUTE;
    textseg -> nsects = 1;
    textseg -> flags = 0;
    return textseg;
}

struct section_64* maketextsect(size_t size, size_t textsegsize)
{
    struct section_64* textsect = (struct section_64*) calloc(1, sizeof(*textsect));
    assert(textsect);
    memmove(textsect -> sectname, SECT_TEXT, sizeof(SECT_TEXT));
    memmove(textsect -> segname, SEG_TEXT, sizeof(SEG_TEXT));
    textsect -> addr = ALIGNNUM + (textsegsize - size);
    textsect -> size = size;
    textsect -> offset = (uint32_t)(textsegsize - size);
    textsect -> align = 0;
    textsect -> reloff = 0;
    textsect -> nreloc = 0;
    textsect -> flags = S_REGULAR | S_ATTR_SOME_INSTRUCTIONS | S_ATTR_PURE_INSTRUCTIONS;
    textsect -> reserved1 = 0;
    textsect -> reserved2 = 0;
    textsect -> reserved3 = 0;
    return textsect;
}


struct segment_command_64* makedataseg(uint64_t addr, uint64_t offset, size_t datasize)
{
    struct segment_command_64* dataseg = (struct segment_command_64*) calloc(1, sizeof(*dataseg));
    assert(dataseg);
    dataseg -> cmd = LC_SEGMENT_64;
    dataseg -> cmdsize = sizeof(struct segment_command_64) + sizeof(struct section_64);
    memmove(dataseg -> segname, SEG_DATA, sizeof(SEG_DATA));
    dataseg -> vmaddr = addr;
    uint64_t size = ALIGNNUM;
    while (size < datasize) size += ALIGNNUM;
    dataseg -> vmsize = size;
    dataseg -> fileoff = offset;
    dataseg -> filesize = size;
    dataseg -> maxprot = VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE;
    dataseg -> initprot = VM_PROT_READ | VM_PROT_WRITE;
    dataseg -> nsects = 1;
    dataseg -> flags = 0;
    return dataseg;
}

struct section_64* makedatasect(uint64_t addr, uint64_t size, uint64_t offset)
{
    struct section_64* datasect = (struct section_64*) calloc(1, sizeof(*datasect));
    assert(datasect);
    memmove(datasect -> segname, SEG_DATA, sizeof(SEG_DATA));
    memmove(datasect -> sectname, SECT_DATA, sizeof(SECT_DATA));
    datasect -> addr = addr;
    datasect -> size = size;
    datasect -> offset = (uint32_t) offset;
    datasect -> align = 0;
    datasect -> reloff = 0;
    datasect -> nreloc = 0;
    datasect -> flags = 0;
    datasect -> reserved1 = 0;
    datasect -> reserved2 = 0;
    datasect -> reserved3 = 0;
    return datasect;
}

void filldatasect(const lexem* source, uint64_t textsectaddr, FILE* dest,
                  size_t datasegsize, size_t datasectsize)
{
    assert(source);
    const lexem* ptr = source;
    
    /*marks*/
    bool ismark[101] = {false};
    while (ptr -> op != EOC)
    {
        if (ptr -> op >= JE)
        {
            ptr++;
            ismark[ptr -> mark] = true;
        }
        
        else if (ptr -> op == PUSH)
        {
            ptr++;
        }
        ptr++;
    }
    
    for (int i = 1; i <= 100; i++)
    {
        if (ismark[i])
        {
            uint64_t markaddr = getmarkaddr(i, source, textsectaddr);
            fwrite(&markaddr, 1, sizeof(markaddr), dest);
        }
        else
        {
            fwrite(&NOMARK, 1, sizeof(NOMARK), dest);
        }
    }

    /*double*/
    fwrite(&CENT, 1, sizeof(CENT), dest);
    ptr = source;
    while (ptr -> op != EOC)
    {
        if (ptr -> op == PUSH)
        {
            ptr++;
            fwrite(&(ptr -> num), 1, sizeof(ptr -> num), dest);
        }
        else if (ptr -> op >= JE) ptr++;
        ptr++;
    }
    
    /*filling with 0x00*/
    uint64_t nulcount = datasegsize - datasectsize;
    void* nulls = calloc(nulcount, sizeof(uint8_t));
    assert(nulls);
    fwrite(nulls, nulcount, sizeof(uint8_t), dest);
    free(nulls);
}


uint64_t getmarkaddr(uint32_t mark, const lexem* source, uint64_t textsectaddr)
{
    assert(source);
    uint64_t res = 0;
    if (mark > MAXMARK)
    {
        assert(WRONGMARK);
    }
    const lexem* ptr = source;
    while (ptr -> op != EOC)
    {
        if (ptr -> op <= CALL && ptr -> op >= END)
        {
            operation op(ptr -> op);
            res += op.getsize();
            if (ptr -> op >= PUSH)
            {
                ptr++;
            }
            ptr++;
        }
        if (ptr -> mark <= MAXMARK && ptr -> mark != mark) ptr++;
        if (ptr -> mark == mark) return res + textsectaddr;
    }
    return res;
}



void filltextsect(const lexem* source, FILE* dest, size_t textsectsize, size_t textsegsize)
{
    assert(source);
    uint64_t nulcount = (textsegsize - textsectsize) -
                        (sizeof(struct mach_header_64) +
                         3*sizeof(struct segment_command_64) +
                         2*sizeof(struct section_64) +
                         sizeof(thread_command) +
                         sizeof(x86_thread_state));
    //fwrite(&CENT, 1, sizeof(CENT), dest);
    void* nulls = calloc(nulcount, sizeof(uint8_t));
    fwrite(nulls, nulcount, sizeof(uint8_t), dest);
    free(nulls);
    
    uint32_t pushcount = 1;
    const lexem* ptr = source;
    while (ptr -> mark)
    {
        if (ptr -> mark <= MAXMARK) ptr++;
        else
        {
            operation op (ptr -> op);
            if (ptr -> op >= JE)
            {
                ptr++;
                uint32_t mark = ptr -> mark;
                op.printop(dest, mark);
            }
            else if (ptr -> op == PUSH)
            {
                op.printop(dest, pushcount);
                pushcount++;
                ptr++;
            }
            else
            {
                op.printop(dest, 0);
            }
            ptr++;
        }
    }
}

void* makethread(uint64_t start, uint64_t doubleaddr, uint64_t markaddr, uint64_t stackaddr)
{
    struct thread_command* res = (struct thread_command*)
                                 calloc(1, sizeof(struct thread_command) +
                                           sizeof(struct x86_thread_state));
    assert(res);
    
    res -> cmd = LC_UNIXTHREAD;
    res -> cmdsize = sizeof(struct thread_command) + sizeof(struct x86_thread_state);
    
    
    struct x86_thread_state* state = (struct x86_thread_state*) (res + 1);
    state -> tsh.flavor = x86_THREAD_STATE64;
    state -> tsh.count = 42;
    
    memset(&(state -> uts.ts64), 0, 21*sizeof(uint64_t));
    state -> uts.ts64.__rbp = stackaddr - 0x8;
    state -> uts.ts64.__r9 = doubleaddr;
    state -> uts.ts64.__r10 = markaddr - 0x8;
    state -> uts.ts64.__rip = start;
    return res;
}

struct uuid_command* makeuuid()
{
    struct uuid_command* uuid = (struct uuid_command*) calloc(1, sizeof(*uuid));
    assert(uuid);
    uuid -> cmd = LC_UUID;
    uuid -> cmdsize = sizeof(*uuid);
    memmove(uuid -> uuid, UUID, 16*sizeof(uint8_t));
    return uuid;
}

struct version_min_command* makeversion()
{
    struct version_min_command* version = (struct version_min_command*)
                                            calloc(1, sizeof(struct version_min_command));
    assert(version);
    version -> cmd = LC_VERSION_MIN_MACOSX;
    version -> cmdsize = sizeof(*version);
    version -> version = 0xA0600;
    return version;
}
