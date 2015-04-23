//
//  header.h
//  bin.translation
//
//  Created by Григорий Чирков on 19.04.15.
//  Copyright (c) 2015 Григорий Чирков. All rights reserved.
//

#ifndef __bin_translation__header__
#define __bin_translation__header__

#include <stdio.h>
#include "reader.h"
#include <mach/machine/thread_status.h>
#include "codification.h"
#include "reader.h"
#include <mach-o/loader.h>

size_t textsize(const lexem* source);
//size_t marksize(const lexem*);
size_t doublesize(const lexem* source);
void translate(const lexem* source, const char* output);
struct mach_header_64* makeheader();
struct segment_command_64* makezeroseg();
struct segment_command_64* maketextseg(size_t size, uint64_t vmaddr);
struct section_64* maketextsect(size_t size, size_t textsegsize);
struct segment_command_64* makedataseg(uint64_t addr, uint64_t offset, size_t datasize);
struct section_64* makedatasect(uint64_t addr, uint64_t size, uint64_t offset);
uint64_t getmarkaddr(uint32_t mark, const lexem* source, uint64_t textsectaddr);
void filldatasect(const lexem* source, uint64_t textsectaddr, FILE* dest, size_t datasegsize, size_t datasectsize);
void filltextsect(const lexem* source, FILE* dest, size_t textsectsize, size_t textsegsize);
void* makethread(uint64_t start, uint64_t doubleaddr, uint64_t markaddr, uint64_t stackaddr);
struct uuid_command* makeuuid();
struct version_min_command* makeversion();
struct segment_command_64* makelinkedit();
struct dyld_info_command* makedyldinfo(uint64_t offset, uint64_t size);

#define ALIGNNUM 4096
#define MARKDATASIZE 800

const uint8_t UUID[16] = {0x4C, 0x41, 0x5E, 0x5B, 0xE1, 0x6B, 0x34, 0xEA, 0x96, 0xd6, 0xdd, 0x0e, 0xe2, 0x72, 0x46, 0xc5};
#endif /* defined(__bin_translation__header__) */