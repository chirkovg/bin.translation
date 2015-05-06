//
//  x86_cmds.h
//  bin.translation
//
//  Created by Григорий Чирков on 27.04.15.
//  Copyright (c) 2015 Григорий Чирков. All rights reserved.
//

#ifndef bin_translation_x86_cmds_h
#define bin_translation_x86_cmds_h

#include "codification.h"


#define x86_MAKESOUND          0x6a, 0x07, \
                               0x48, 0x89, 0xe6, \
                               0xbf, 0x01, 0x00, 0x00, 0x00, \
                               0xba, 0x01, 0x00, 0x00, 0x00, \
                               0xb8, 0x04, 0x00, 0x00, 0x02, \
                               0x0f, 0x05
                                                             // makes beep sound before exit

#define x86_EXITPROGRAM        0xb8, 0x01, 0x00, 0x00, 0x02, \
                               0xbf, 0x00, 0x00, 0x00, 0x00, \
                               0x0f, 0x05                    
                                                             // exits program

#define MEMRBP_TO_XMM0         0xf2, 0x0f, 0x10, 0x45, 0x00  // movsd (%rbp), %xmm0
#define ADD_8_TO_RBP           0x48, 0x83, 0xc5, 0x08        // addq $0x8, %rbp
#define ADD_MEMRBP_TO_XMM0     0xf2, 0x0f, 0x58, 0x45, 0x00  // addsd (%rbp), %xmm0
#define XMM0_TO_MEMRBP         0xf2, 0x0f, 0x11, 0x45, 0x00  // movsd %xmm0, (%rbp)
#define SUB_MEMRBP_FROM_XMM0   0xf2, 0x0f, 0x5c, 0x45, 0x00  // subsd (%rbp), %xmm0
#define DIV_MEMRBP_ON_XMM0     0xf2, 0x0f, 0x5e, 0x45, 0x00  // divsd (%rbp), %xmm0
#define MUL_MEMRBP_ON_XMM0     0xf2, 0x0f, 0x59, 0x45, 0x00  // mulsd (%rbp), %xmm0
#define x86_RET                0xc3                          // ret
#define XMM1_TO_MEMRBP         0xf2, 0x0f, 0x11, 0x4d, 0x00  // movsd %xmm1, (%rbp)
#define XMM2_TO_MEMRBP         0xf2, 0x0f, 0x11,0x55, 0x00   // movsd %xmm2, (%rbp)
#define XMM3_TO_MEMRBP         0xf2, 0x0f, 0x11, 0x5d, 0x00  // movsd %xmm3, (%rbp)
#define XMM4_TO_MEMRBP         0xf2, 0x0f, 0x11, 0x65, 0x00  // movsd %xmm4, (%rbp)
#define SUB_8_FROM_RBP         0x48, 0x83, 0xed, 0x08        // subq $0x8, %rbp
#define MEMRBP_TO_XMM1         0xf2, 0x0f, 0x10, 0x4d, 0x00  // movsd (%rbp), %xmm1
#define MEMRBP_TO_XMM2         0xf2, 0x0f, 0x10, 0x55, 0x00  // movsd (%rbp), %xmm2
#define MEMRBP_TO_XMM3         0xf2, 0x0f, 0x10, 0x5d, 0x00  // movsd (%rbp), %xmm3
#define MEMRBP_TO_XMM4         0xf2, 0x0f, 0x10, 0x65, 0x00  // movsd (%rbp), %xmm4
#define MEMOFFR9_TO_XMM0       0xf2, 0x41, 0x0f, 0x10, 0x81  // movsd (%r9), %xmm0
#define ADD_8_TO_R9            0x49, 0x83, 0xc1, 0x08        // addq $0x8, %r9
#define SUB_16_FROM_RBP        0x48, 0x83, 0xed, 0x10        // subq $0x16, %rbp
#define CMP_MEM8RBP_WITH_XMM0  0x66, 0x0f, 0x2f, 0x45, 0x08  // comisd 0x8(%rbp), %xmm0
#define JNE_7                  0x75, 0x07
#define JMPMARK                0x41, 0xff, 0xa2
#define JE_7                   0x74, 0x07
#define JA_7                   0x77, 0x07
#define JAE_7                  0x73, 0x07
#define JB_7                   0x72, 0x07
#define JBE_7                  0x76, 0x07
#define CALL_MARK              0x41, 0xff, 0x92

#define TH_TO_XMM5             0xf2, 0x0f, 0x10, 0x2c, 0x25, \
                               0x20, 0x23, 0x00, 0x00

#define MUL_XMM0_ON_XMM5       0xf2, 0x0f, 0x59, 0xc5
#define TEN_TO_RCX             0xb9, 0x0a, 0x00, 0x00, 0x00
#define PUSHMIN1               0x6a, 0xff
#define NULLRDX                0x48, 0x31, 0xd2
#define DIVRCX                 0x48, 0xf7, 0xf1
#define PUSHRDX                0x52
#define POPRDX                 0x5a
#define CMPRDXMIN1             0x48, 0x83, 0xfa, 0xff
#define CMPRAX0                0x48, 0x83, 0xf8, 0x00
#define JMPDIV                 0x75, 0xf3

#define CMPMIN1WITH18MEMRBP    0x48, 0x83, 0x7C, 0x24, 0x18, \
                               0xFF

#define JNENODOT               0x75, 0x17
#define PUSHDOT                0x6a, 0x2e
#define MOVRSPTORSI            0x48, 0x89, 0xe6

#define PRINTCHAR              0xbf, 0x01, 0x00, 0x00, 0x00, \
                               0xba, 0x01, 0x00, 0x00, 0x00, \
                               0xb8, 0x04, 0x00, 0x00, 0x02, \
                               0x0f, 0x05, \
                               0x5a

#define JERET                  0x74, 0x1c
#define ADD0RDX                0x48, 0x83, 0xc2, 0x30
#define JMPPRINT               0xeb, 0xbe

#define PRINTRETURN            0x6a, 0x0a, \
                               MOVRSPTORSI, \
                               PRINTCHAR

#define TOINT                  0xf2, 0x48, 0x0f, 0x2c, 0xc0
#define NULLRAX                0x48, 0x31, 0xc0
#define PUSHRAX                0x50
#define PUSHSMTH               0x6a, 0x41

#define GETCHAR                MOVRSPTORSI, \
                               0xbf, 0x00, 0x00, 0x00, 0x00, \
                               0xba, 0x01, 0x00, 0x00, 0x00, \
                               0xb8, 0x03, 0x00, 0x00, 0x02, \
                               0x0f, 0x05

#define POPRCX                 0x59
#define POPRAX                 0x58
#define CMPRCXNL               0x48, 0x83, 0xf9, 0x0a
#define JEEXIT                 0x74, 0x11
#define SUB0RCX                0x48, 0x83, 0xe9, 0x30
#define MOV10RDX               0xba, 0x0a, 0x00, 0x00, 0x00
#define MULRDX                 0x48, 0xf7, 0xe2
#define ADDRCXTORAX            0x48, 0x01, 0xc8
#define JMPNUMMAKING           0xEB, 0xD0
#define TODOUBLE               0xf2, 0x48, 0x0f, 0x2a, 0xc0

const uint8_t SCANINST[] = {NULLRAX, PUSHRAX, PUSHSMTH, GETCHAR, POPRCX, POPRAX, CMPRCXNL, JEEXIT, SUB0RCX, MOV10RDX, MULRDX, ADDRCXTORAX, JMPNUMMAKING, TODOUBLE, ADD_8_TO_RBP, XMM0_TO_MEMRBP};

const uint8_t EXITINST[] = {x86_MAKESOUND, x86_EXITPROGRAM};

const uint8_t ADDINST[] = {MEMRBP_TO_XMM0, SUB_8_FROM_RBP, ADD_MEMRBP_TO_XMM0, XMM0_TO_MEMRBP};

const uint8_t DIVINST[] = {MEMRBP_TO_XMM0, SUB_8_FROM_RBP, DIV_MEMRBP_ON_XMM0, XMM0_TO_MEMRBP};

const uint8_t SUBINST[] = {MEMRBP_TO_XMM0, SUB_8_FROM_RBP, SUB_MEMRBP_FROM_XMM0, XMM0_TO_MEMRBP};

const uint8_t MULINST[] = {MEMRBP_TO_XMM0, SUB_8_FROM_RBP, MUL_MEMRBP_ON_XMM0, XMM0_TO_MEMRBP};

const uint8_t PUSHAXINST[] = {ADD_8_TO_RBP, XMM1_TO_MEMRBP};

const uint8_t PUSHBXINST[] = {ADD_8_TO_RBP, XMM2_TO_MEMRBP};

const uint8_t PUSHCXINST[] = {ADD_8_TO_RBP, XMM3_TO_MEMRBP};

const uint8_t PUSHDXINST[] = {ADD_8_TO_RBP, XMM4_TO_MEMRBP};

const uint8_t POPAXINST[] = {MEMRBP_TO_XMM1, SUB_8_FROM_RBP};

const uint8_t POPBXINST[] = {MEMRBP_TO_XMM2, SUB_8_FROM_RBP};

const uint8_t POPCXINST[] = {MEMRBP_TO_XMM3, SUB_8_FROM_RBP};

const uint8_t POPDXINST[] = {MEMRBP_TO_XMM4, SUB_8_FROM_RBP};

const uint8_t RETINST[] = {x86_RET};

const uint8_t PUSHINST[] = {ADD_8_TO_RBP, MEMOFFR9_TO_XMM0, 0x00, 0x00, 0x00, 0x00,  XMM0_TO_MEMRBP};

//next instructions are not full
//in the end - offset of jump instruction in instr segment

const uint8_t CALLINST[] = {CALL_MARK};

const uint8_t JMPINST[] = {JMPMARK};

const uint8_t JEINST[] = {MEMRBP_TO_XMM0, SUB_16_FROM_RBP, CMP_MEM8RBP_WITH_XMM0, JNE_7, JMPMARK};

const uint8_t JNEINST[] = {MEMRBP_TO_XMM0, SUB_16_FROM_RBP, CMP_MEM8RBP_WITH_XMM0, JE_7, JMPMARK};

const uint8_t JAINST[] = {MEMRBP_TO_XMM0, SUB_16_FROM_RBP, CMP_MEM8RBP_WITH_XMM0, JBE_7, JMPMARK};

const uint8_t JAEINST[] = {MEMRBP_TO_XMM0, SUB_16_FROM_RBP, CMP_MEM8RBP_WITH_XMM0, JB_7, JMPMARK};

const uint8_t JBINST[] = {MEMRBP_TO_XMM0, SUB_16_FROM_RBP, CMP_MEM8RBP_WITH_XMM0, JAE_7, JMPMARK};

const uint8_t JBEINST[] = {MEMRBP_TO_XMM0, SUB_16_FROM_RBP, CMP_MEM8RBP_WITH_XMM0, JA_7, JMPMARK};

const uint8_t PRINTINST[] = {MEMRBP_TO_XMM0, TH_TO_XMM5, MUL_XMM0_ON_XMM5, SUB_8_FROM_RBP, TOINT, TEN_TO_RCX, PUSHMIN1, NULLRDX, DIVRCX, PUSHRDX, CMPRAX0, JMPDIV, CMPMIN1WITH18MEMRBP, JNENODOT, PUSHDOT, MOVRSPTORSI, PRINTCHAR, POPRDX, CMPRDXMIN1, JERET, ADD0RDX, PUSHRDX, MOVRSPTORSI, PRINTCHAR, JMPPRINT, PRINTRETURN};



#endif
