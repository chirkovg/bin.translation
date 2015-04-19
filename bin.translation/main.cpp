//
//  main.cpp
//  bin.translation
//
//  Created by Григорий Чирков on 10.04.15.
//  Copyright (c) 2015 Григорий Чирков. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include "codification.h"
#include "reader.h"
#include "header.h"

int main(int argc, const char * argv[])
{
    lexem* source = getbuf("/Users/chirkovg/code");
    std::cout << doublesize(source);
    
    return 0;
}
