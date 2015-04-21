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

size_t textsize(const lexem*);
size_t marksize(const lexem*);
size_t doublesize(const lexem*);
FILE* translate(const lexem*);
struct mach_header_64* makeheader();
struct segment_command_64* makezeroseg();
struct segment_command_64* maketextseg(size_t size, uint64_t vmaddr);
struct section_64* maketextsect(uint64_t addr, size_t size);
struct segment_command_64* makedataseg(uint64_t addr, uint64_t offset, size_t doublesize, size_t marksize);
struct section_64* makedatasect(uint64_t addr, uint64_t size, uint32_t offset);

#endif /* defined(__bin_translation__header__) */