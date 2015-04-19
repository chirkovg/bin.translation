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
FILE* makeheader(const lexem*);

#endif /* defined(__bin_translation__header__) */