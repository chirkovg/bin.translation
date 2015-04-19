//
//  reader.h
//  bin.translation
//
//  Created by Григорий Чирков on 10.04.15.
//  Copyright (c) 2015 Григорий Чирков. All rights reserved.
//

#ifndef __bin_translation__reader__
#define __bin_translation__reader__

#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <iostream>
#include "codification.h"

union lexem
{
    OP op;
    double num;
    int mark;
};

lexem* getbuf(const char*);




#endif /* defined(__bin_translation__reader__) */
