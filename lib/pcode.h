//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or (at
// you option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <http://www.gnu.org/licenses/>
//

#ifndef LIB_PCODE_H
#define LIB_PCODE_H

enum pcode_t
{
    SLDC_0,     // 00 Short Load Constant
    SLDC_1,     // 01 Short Load Constant
    SLDC_2,     // 02 Short Load Constant
    SLDC_3,     // 03 Short Load Constant
    SLDC_4,     // 04 Short Load Constant
    SLDC_5,     // 05 Short Load Constant
    SLDC_6,     // 06 Short Load Constant
    SLDC_7,     // 07 Short Load Constant
    SLDC_8,     // 08 Short Load Constant
    SLDC_9,     // 09 Short Load Constant
    SLDC_10,    // 0A Short Load Constant
    SLDC_11,    // 0B Short Load Constant
    SLDC_12,    // 0C Short Load Constant
    SLDC_13,    // 0D Short Load Constant
    SLDC_14,    // 0E Short Load Constant
    SLDC_15,    // 0F Short Load Constant
    SLDC_16,    // 10 Short Load Constant
    SLDC_17,    // 11 Short Load Constant
    SLDC_18,    // 12 Short Load Constant
    SLDC_19,    // 13 Short Load Constant
    SLDC_20,    // 14 Short Load Constant
    SLDC_21,    // 15 Short Load Constant
    SLDC_22,    // 16 Short Load Constant
    SLDC_23,    // 17 Short Load Constant
    SLDC_24,    // 18 Short Load Constant
    SLDC_25,    // 19 Short Load Constant
    SLDC_26,    // 1A Short Load Constant
    SLDC_27,    // 1B Short Load Constant
    SLDC_28,    // 1C Short Load Constant
    SLDC_29,    // 1D Short Load Constant
    SLDC_30,    // 1E Short Load Constant
    SLDC_31,    // 1F Short Load Constant
    SLDC_32,    // 20 Short Load Constant
    SLDC_33,    // 21 Short Load Constant
    SLDC_34,    // 22 Short Load Constant
    SLDC_35,    // 23 Short Load Constant
    SLDC_36,    // 24 Short Load Constant
    SLDC_37,    // 25 Short Load Constant
    SLDC_38,    // 26 Short Load Constant
    SLDC_39,    // 27 Short Load Constant
    SLDC_40,    // 28 Short Load Constant
    SLDC_41,    // 29 Short Load Constant
    SLDC_42,    // 2A Short Load Constant
    SLDC_43,    // 2B Short Load Constant
    SLDC_44,    // 2C Short Load Constant
    SLDC_45,    // 2D Short Load Constant
    SLDC_46,    // 2E Short Load Constant
    SLDC_47,    // 2F Short Load Constant
    SLDC_48,    // 30 Short Load Constant
    SLDC_49,    // 31 Short Load Constant
    SLDC_50,    // 32 Short Load Constant
    SLDC_51,    // 33 Short Load Constant
    SLDC_52,    // 34 Short Load Constant
    SLDC_53,    // 35 Short Load Constant
    SLDC_54,    // 36 Short Load Constant
    SLDC_55,    // 37 Short Load Constant
    SLDC_56,    // 38 Short Load Constant
    SLDC_57,    // 39 Short Load Constant
    SLDC_58,    // 3A Short Load Constant
    SLDC_59,    // 3B Short Load Constant
    SLDC_60,    // 3C Short Load Constant
    SLDC_61,    // 3D Short Load Constant
    SLDC_62,    // 3E Short Load Constant
    SLDC_63,    // 3F Short Load Constant
    SLDC_64,    // 40 Short Load Constant
    SLDC_65,    // 41 Short Load Constant
    SLDC_66,    // 42 Short Load Constant
    SLDC_67,    // 43 Short Load Constant
    SLDC_68,    // 44 Short Load Constant
    SLDC_69,    // 45 Short Load Constant
    SLDC_70,    // 46 Short Load Constant
    SLDC_71,    // 47 Short Load Constant
    SLDC_72,    // 48 Short Load Constant
    SLDC_73,    // 49 Short Load Constant
    SLDC_74,    // 4A Short Load Constant
    SLDC_75,    // 4B Short Load Constant
    SLDC_76,    // 4C Short Load Constant
    SLDC_77,    // 4D Short Load Constant
    SLDC_78,    // 4E Short Load Constant
    SLDC_79,    // 4F Short Load Constant
    SLDC_80,    // 50 Short Load Constant
    SLDC_81,    // 51 Short Load Constant
    SLDC_82,    // 52 Short Load Constant
    SLDC_83,    // 53 Short Load Constant
    SLDC_84,    // 54 Short Load Constant
    SLDC_85,    // 55 Short Load Constant
    SLDC_86,    // 56 Short Load Constant
    SLDC_87,    // 57 Short Load Constant
    SLDC_88,    // 58 Short Load Constant
    SLDC_89,    // 59 Short Load Constant
    SLDC_90,    // 5A Short Load Constant
    SLDC_91,    // 5B Short Load Constant
    SLDC_92,    // 5C Short Load Constant
    SLDC_93,    // 5D Short Load Constant
    SLDC_94,    // 5E Short Load Constant
    SLDC_95,    // 5F Short Load Constant
    SLDC_96,    // 60 Short Load Constant
    SLDC_97,    // 61 Short Load Constant
    SLDC_98,    // 62 Short Load Constant
    SLDC_99,    // 63 Short Load Constant
    SLDC_100,   // 64 Short Load Constant
    SLDC_101,   // 65 Short Load Constant
    SLDC_102,   // 66 Short Load Constant
    SLDC_103,   // 67 Short Load Constant
    SLDC_104,   // 68 Short Load Constant
    SLDC_105,   // 69 Short Load Constant
    SLDC_106,   // 6A Short Load Constant
    SLDC_107,   // 6B Short Load Constant
    SLDC_108,   // 6C Short Load Constant
    SLDC_109,   // 6D Short Load Constant
    SLDC_110,   // 6E Short Load Constant
    SLDC_111,   // 6F Short Load Constant
    SLDC_112,   // 70 Short Load Constant
    SLDC_113,   // 71 Short Load Constant
    SLDC_114,   // 72 Short Load Constant
    SLDC_115,   // 73 Short Load Constant
    SLDC_116,   // 74 Short Load Constant
    SLDC_117,   // 75 Short Load Constant
    SLDC_118,   // 76 Short Load Constant
    SLDC_119,   // 77 Short Load Constant
    SLDC_120,   // 78 Short Load Constant
    SLDC_121,   // 79 Short Load Constant
    SLDC_122,   // 7A Short Load Constant
    SLDC_123,   // 7B Short Load Constant
    SLDC_124,   // 7C Short Load Constant
    SLDC_125,   // 7D Short Load Constant
    SLDC_126,   // 7E Short Load Constant
    SLDC_127,   // 7F Short Load Constant
    ABI,        // 80 Absolute value integer
    ABR,        // 81 Absolute value real
    ADI,        // 82 Add integers
    ADR,        // 83 Add reals
    LAND,       // 84 Logical AND
    DIF,        // 85 Set difference
    DVI,        // 86 Divide integers
    DVR,        // 87 Divide reals
    CHK,        // 88 Check subrange bounds
    FLO,        // 89 Float top-of-stack-1
    FLT,        // 8A Float top-of-stack
    INN,        // 8B Test set membership
    INT,        // 8C Set intersection
    LOR,        // 8D Logical OR
    MODI,       // 8E Modulo integers
    MPI,        // 8F Multiply integers
    MPR,        // 90 Multiply reals
    NGI,        // 91 Negate integer
    NGR,        // 92 Negate real
    LNOT,       // 93 Logical NOT
    SRS,        // 94 Build subrange set
    SBI,        // 95 Subtract integers
    SBR,        // 96 Subtract reals
    SGS,        // 97 Build one-member set
    SQI,        // 98 Square integer
    SQR,        // 99 Square real
    STO,        // 9A Store Indirect
    IXS,        // 9B Index String array
    UNI,        // 9C Set union

    LDE,        // 9D II.1: Load Extended (byte, big)
                // 9D II.0: not imp
    S2P = LDE,  // 9D I.3, I.5: string to packed convert

    CSP,        // 9E I.5, II.0, II.1: Call standard procedure (byte)
                // 9E I.3: not imp

    LDCN,       // 9F Load constant NIL
    ADJ,        // A0 Adjust set (byte)
    FJP,        // A1 False jump (byte)
    INC,        // A2 Incriment field pointer (big)
    IND,        // A3 Static index and load (big)
    IXA,        // A4 Index array (big)
    LAO,        // A5 Load Global Address (big)

    LSA,        // A6 II.0, II.1: Load constant String Address
    LCA = LSA,  // A6 I.3, I.5: Load Constant string Address

    LAE,        // A7 II.1: Load Extended Address (byte, big)
    LDO_i_5 = LAE, // A7 I.3, I.5: LoaD Global word
               // A7 II.0: not imp

    MOV,        // A8 Move words (big)

    LDO,        // A9 II.0, II.1: Load Global (big)
    MVB = LDO,  // A9 I.3, I.5: MoVe Bytes

    SAS,        // AA String Assign
    SRO,        // AB Store Global (big)
    XJP,        // AC Case jump (word, word, ..., word)

    RNP,        // AD I.5, II.0, II.1: Return from non-base procedure (byte)
                // AD 1.3: not imp

    CIP,        // AE I.5, II.0, II.1: Call intermediate procedure (byte)
                // AE I.3: not imp

    EQU,        // AF Test equality (byte)
    GEQ,        // B0 test greater than or equal (byte)
    GTR,        // B1 Test greater than (byte)
    LDA,        // B2 Load Intermediate Address (big)
    LDC,        // B3 Load Multi-word Constant (byte, data...)
    LEQ,        // B4 Test less than or equal (byte)
    LES,        // B5 Test less than (byte)
    LOD,        // B6 Load Intermediate (big)
    NEQ,        // B7 Test inequality (byte)
    STR,        // B8 Stote Intermediate (byte, big)
    UJP,        // B9 Unsigned jump (byte)
    LDP,        // BA Load packed field
    STP,        // BB Store packed field
    LDM,        // BC Load Multi-word (byte)
    STM,        // BD Store Multi-word (byte)
    LDB,        // BE Load Byte
    STB,        // BF Store Byte
    IXP,        // C0 Index packed array (bytes, byte)
    RBP,        // C1 Return from base procedure (byte)
    CBP,        // C2 Call base procedure (byte)
    EQUI,       // C3 Test integer equality
    GEQI,       // C4 Test integer greater than or equal
    GTRI,       // C5 Test integer greater than
    LLA,        // C6 Load Local Address (big)
    LDCI,       // C7 Load constant integer (word)
    LEQI,       // C8 Test integer less than or equal
    LESI,       // C9 Test integer less than
    LDL,        // CA Load Local (big)
    NEQI,       // CB Test integer inequality
    STL,        // CC Store Local (big)
    CXP,        // CD Call external procedure (byte, byte)
    CLP,        // CE Call local procedure (byte)
    CGP,        // CF Call global procedure (byte)

    LPA,        // D0 II.0, II.1: Load packed array (byte, data...)
    S1P = LPA,  // D0 I.3, I.5: string to packed convert

    STE,        // D1 II.1: Store Extended (byte, big)
    IXB = STE,  // D1 I.3, I.5: IndeX Byte
                // D1 II.0: not imp

    BYT,        // D2 I.3, I.5: convert word pointer to byte pointer
                // D2 II.0, II.1: not imp

    EFJ,        // D3 Equal False Jump (byte)     => "branch if not equal"
    NFJ,        // D4 Not equal False Jump (byte) => "branch if equal"
    BPT,        // D5 Breakpoint (byte)
    XIT,        // D6 Exit the operating system.
    NOP,        // D7 No-operation
    SLDL_1,     // D8 Short Load Local
    SLDL_2,     // D9 Short Load Local
    SLDL_3,     // DA Short Load Local
    SLDL_4,     // DB Short Load Local
    SLDL_5,     // DC Short Load Local
    SLDL_6,     // DD Short Load Local
    SLDL_7,     // DE Short Load Local
    SLDL_8,     // DF Short Load Local
    SLDL_9,     // E0 Short Load Local
    SLDL_10,    // E1 Short Load Local
    SLDL_11,    // E2 Short Load Local
    SLDL_12,    // E3 Short Load Local
    SLDL_13,    // E4 Short Load Local
    SLDL_14,    // E5 Short Load Local
    SLDL_15,    // E6 Short Load Local
    SLDL_16,    // E7 Short Load Local
    SLDO_1,     // E8 Short Load Global
    SLDO_2,     // E9 Short Load Global
    SLDO_3,     // EA Short Load Global
    SLDO_4,     // EB Short Load Global
    SLDO_5,     // EC Short Load Global
    SLDO_6,     // ED Short Load Global
    SLDO_7,     // EE Short Load Global
    SLDO_8,     // EF Short Load Global
    SLDO_9,     // F0 Short Load Global
    SLDO_10,    // F1 Short Load Global
    SLDO_11,    // F2 Short Load Global
    SLDO_12,    // F3 Short Load Global
    SLDO_13,    // F4 Short Load Global
    SLDO_14,    // F5 Short Load Global
    SLDO_15,    // F6 Short Load Global
    SLDO_16,    // F7 Short Load Global
    SIND_0,     // F8 Short Load Indirect
    SIND_1,     // F9 Short Load Indirect
    SIND_2,     // FA Short Load Indirect
    SIND_3,     // FB Short Load Indirect
    SIND_4,     // FC Short Load Indirect
    SIND_5,     // FD Short Load Indirect
    SIND_6,     // FE Short Load Indirect
    SIND_7      // FF Short Load Indirect
};

enum pcode_csp_t
{
    CSP_IOCHECK,    // IoCheck()
    CSP_NEW,        // new()
    CSP_MOVELEFT,   // moveleft()
    CSP_MOVERIGHT,  // moveright()
    CSP_EXIT,       // exit()
    CSP_UNITREAD,   // unitread()
    CSP_UNITWRITE,  // unitwrite()
    CSP_IDSEARCH,   // idsearch()
    CSP_TREESEARCH, // treesearch()
    CSP_TIME,       // time()
    CSP_FILLCHAR,   // fillchar()
    CSP_SCAN,       // scan()
    CSP_UNITSTATUS, // unitstatus()
    CSP_LOADSEGMENT = 21, // LoadSegment()
    CSP_UNLOADSEGMENT, // UnloadSegment()
    CSP_TRUNC,      // trunc()
    CSP_ROUND,      // round()
    CSP_SIN,        // sin()
    CSP_COS,        // cos()
    CSP_TAN,        // tan()
    CSP_ATAN,       // atan()
    CSP_LN,         // ln()
    CSP_EXP,        // exp()
    CSP_SQRT,       // sqrt()
    CSP_MARK,       // mark()
    CSP_RELEASE,    // release()
    CSP_IORESULT,   // ioresult()
    CSP_UNITBUSY,   // unitbusy()
    CSP_PWROFTEN,   // PwrOfTen()
    CSP_UNITWAIT,   // unitwait()
    CSP_UNITCLEAR,  // unitclear()
    CSP_HALT,       // halt()
    CSP_MEMAVAIL    // memavail()
};

enum
{
    CXP_0_EXECERROR = 2,
    CXP_0_FINIT,
    CXP_0_FRESET,
    CXP_0_FOPEN,
    CXP_0_FCLOSE,
    CXP_0_FGET,
    CXP_0_FPUT,
    CXP_0_XSEEK,
    CXP_0_FEOF,
    CXP_0_FEOLN,
    CXP_0_FREADINT,
    CXP_0_FWRITEINT,
    CXP_0_XREADREAL,
    CXP_0_XWRITEREAL,
    CXP_0_FREADCHAR,
    CXP_0_FWRITECHAR,
    CXP_0_FREADSTRING,
    CXP_0_FWRITESTRING,
    CXP_0_FWRITEBYTES,
    CXP_0_FREADLN,
    CXP_0_FWRITELN,
    CXP_0_SCONCAT,
    CXP_0_SINSERT,
    CXP_0_SCOPY,
    CXP_0_SDELETE,
    CXP_0_SPOS,
    CXP_0_FBLOCKIO,
    CXP_0_FGOTOXY
};

enum
{
    CXP_31_FSEEK = 2,
    CXP_31_FREADREAL,
    CXP_31_FWRITEREAL,
};

enum
{
    OP_CMP_REAL = 2,
    OP_CMP_STRING = 4,
    OP_CMP_BOOLEAN = 6,
    OP_CMP_SET = 8,
    OP_CMP_BYTE_ARRAY = 10,
    OP_CMP_MULTI_WORD = 12
};

// closetype
enum
{
    CNORMAL,
    CLOCK,
    CPURGE,
    CCRUNCH
};

//
// The DECOPS procedure (seg 30, proc 4) has a selector pushed onto the
// stack immediately before the call.
//
enum
{
    DECOPS_SEG_NUM = 30,
    DECOPS_PROC_NUM = 4,

    DECOPS_ADJUST = 0,
    DECOPS_ADD = 2,
    DECOPS_SUBTRACT = 4,
    DECOPS_NEGATE = 6,
    DECOPS_MULTIPLY = 8,
    DECOPS_DIVIDE = 10,
    DECOPS_STR = 12,
    DECOPS_CONVERT_TOS_M1 = 14,
    DECOPS_COMPARE = 16,
    DECOPS_CONVERT_TOS = 18,
    DECOPS_TRUNC = 20,

    // These selectors do not exist (yet).
    DECOPS_ABS = 22,
    DECOPS_SQR = 24,
    DECOPS_MOD = 26,

    DECOPS_COMPARE_LT = 8,
    DECOPS_COMPARE_LE = 9,
    DECOPS_COMPARE_GE = 10,
    DECOPS_COMPARE_GT = 11,
    DECOPS_COMPARE_NE = 12,
    DECOPS_COMPARE_EQ = 13,

    // procedure numbers
    DECOPS_FREADDEC = 2,
    DECOPS_FWRITEDEC = 3,
};

#endif // LIB_PCODE_H
// vim: set ts=8 sw=4 et :
