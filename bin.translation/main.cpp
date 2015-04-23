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
#include <string.h>

int main(int argc, const char * argv[])
{
    if (argc < 2)
    {
        puts("no input\n");
        return 0;
    }
    lexem* source = getbuf(argv[1]);
    const char* output = nullptr;
    if (argc > 2) output = argv[2];
    translate(source, output);
    if (argc > 2)
    {
        char* cmd = (char*) calloc(strlen("chmod +x ") + strlen(argv[2]), sizeof(char));
        assert(cmd);
        memmove(cmd, "chmod +x ", strlen("chmod +x "));
        strcat(cmd, argv[2]);
        system(cmd);
        free(cmd);
    }
    else
        system("chmod +x a.out");
    free(source);
    return 0;
}
