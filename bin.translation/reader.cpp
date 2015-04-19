//
//  reader.cpp
//  bin.translation
//
//  Created by Григорий Чирков on 10.04.15.
//  Copyright (c) 2015 Григорий Чирков. All rights reserved.
//

#include "reader.h"


lexem* getbuf(const char* filename)
{
    assert(filename);
    FILE* source = fopen(filename, "rb");
    assert(source);
    
    struct stat* filebuf = (struct stat*) calloc(1, sizeof(*filebuf));
    assert(filebuf);
    
    fstat(fileno(source), filebuf);
    assert(filebuf -> st_size);
    
    size_t length = filebuf -> st_size;
    free(filebuf);
    
    lexem* buf = (lexem*) calloc(length + 1, sizeof(char));
    assert(buf);
    
    fread(buf, sizeof(char), length, source);
    
    fclose(source);
    return buf;
}

