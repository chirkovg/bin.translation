//
//  main.cpp
//  bin.translation
//
//  Created by Григорий Чирков on 10.04.15.
//  Copyright (c) 2015 Григорий Чирков. All rights reserved.
//

#include <iostream>
#include <stdio.h>

int main(int argc, const char * argv[])
{
    int x[] = {258};
    FILE* res = fopen("res", "wb");
    fwrite(x, 1, 4, res);
    return 0;
}
