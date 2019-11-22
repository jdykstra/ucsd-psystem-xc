//
// UCSD p-System cross compiler
// Copyright (C) 2011, 2012 Peter Miller
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

#include <lib/ac/ctype.h>

#include <lib/code_source_map.h>
#include <lib/debug.h>
#include <lib/disassembler/pcode/ii_1.h>
#include <lib/pcode.h>
#include <lib/sizeof.h>


disassembler_pcode_ii_1::~disassembler_pcode_ii_1()
{
}


disassembler_pcode_ii_1::disassembler_pcode_ii_1(
    byte_sex_t a_byte_sex,
    dislabel &a_where,
    discolumns &a_report,
    int a_seg_num,
    int a_proc_num,
    int a_proc_base,
    code_source_map *a_listing
) :
    disassembler_pcode(a_byte_sex, a_where, a_report, a_seg_num, a_proc_num,
        a_proc_base, a_listing),
    entry_ic(a_proc_base),
    exit_ic(0),
    jump_table_ic(0),
    proc_attr_table_offset(0)
{
}


disassembler_pcode_ii_1::pointer
disassembler_pcode_ii_1::create(byte_sex_t a_byte_sex, dislabel &a_where,
    discolumns &a_report, int a_seg_num, int a_proc_num, int a_proc_base,
    code_source_map *a_listing)
{
    return
        pointer
        (
            new disassembler_pcode_ii_1
            (
                a_byte_sex,
                a_where,
                a_report,
                a_seg_num,
                a_proc_num,
                a_proc_base,
                a_listing
            )
        );
}


void
disassembler_pcode_ii_1::preprocess(const unsigned char *data,
    unsigned a_entry_ic, unsigned a_exit_ic, size_t data_size)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    entry_ic = a_entry_ic;
    DEBUG(2, "entry_ic = 0x%04X", entry_ic);
    exit_ic = a_exit_ic;
    DEBUG(2, "exit_ic = 0x%04X", exit_ic);
    proc_attr_table_offset = data_size - 2;
    for (unsigned m = proc_attr_table_offset - 10; m > exit_ic; m -= 2)
    {
        unsigned short dstadr = m - get_word(data + m);
        where.add(dstadr);
    }
    jump_table_ic = proc_attr_table_offset - 8;

    // Take a pass through the code, looking for XJP opcodes.  The case
    // statement code does not use the jump table to reach each branch,
    // but rather self-relative pointers.
    unsigned address = entry_ic;
    while (address < jump_table_ic)
    {
        unsigned char c = data[address++];
        switch (c)
        {
        case SLDC_0:
        case SLDC_1:
        case SLDC_2:
        case SLDC_3:
        case SLDC_4:
        case SLDC_5:
        case SLDC_6:
        case SLDC_7:
        case SLDC_8:
        case SLDC_9:
        case SLDC_10:
        case SLDC_11:
        case SLDC_12:
        case SLDC_13:
        case SLDC_14:
        case SLDC_15:
        case SLDC_16:
        case SLDC_17:
        case SLDC_18:
        case SLDC_19:
        case SLDC_20:
        case SLDC_21:
        case SLDC_22:
        case SLDC_23:
        case SLDC_24:
        case SLDC_25:
        case SLDC_26:
        case SLDC_27:
        case SLDC_28:
        case SLDC_29:
        case SLDC_30:
        case SLDC_31:
        case SLDC_32:
        case SLDC_33:
        case SLDC_34:
        case SLDC_35:
        case SLDC_36:
        case SLDC_37:
        case SLDC_38:
        case SLDC_39:
        case SLDC_40:
        case SLDC_41:
        case SLDC_42:
        case SLDC_43:
        case SLDC_44:
        case SLDC_45:
        case SLDC_46:
        case SLDC_47:
        case SLDC_48:
        case SLDC_49:
        case SLDC_50:
        case SLDC_51:
        case SLDC_52:
        case SLDC_53:
        case SLDC_54:
        case SLDC_55:
        case SLDC_56:
        case SLDC_57:
        case SLDC_58:
        case SLDC_59:
        case SLDC_60:
        case SLDC_61:
        case SLDC_62:
        case SLDC_63:
        case SLDC_64:
        case SLDC_65:
        case SLDC_66:
        case SLDC_67:
        case SLDC_68:
        case SLDC_69:
        case SLDC_70:
        case SLDC_71:
        case SLDC_72:
        case SLDC_73:
        case SLDC_74:
        case SLDC_75:
        case SLDC_76:
        case SLDC_77:
        case SLDC_78:
        case SLDC_79:
        case SLDC_80:
        case SLDC_81:
        case SLDC_82:
        case SLDC_83:
        case SLDC_84:
        case SLDC_85:
        case SLDC_86:
        case SLDC_87:
        case SLDC_88:
        case SLDC_89:
        case SLDC_90:
        case SLDC_91:
        case SLDC_92:
        case SLDC_93:
        case SLDC_94:
        case SLDC_95:
        case SLDC_96:
        case SLDC_97:
        case SLDC_98:
        case SLDC_99:
        case SLDC_100:
        case SLDC_101:
        case SLDC_102:
        case SLDC_103:
        case SLDC_104:
        case SLDC_105:
        case SLDC_106:
        case SLDC_107:
        case SLDC_108:
        case SLDC_109:
        case SLDC_110:
        case SLDC_111:
        case SLDC_112:
        case SLDC_113:
        case SLDC_114:
        case SLDC_115:
        case SLDC_116:
        case SLDC_117:
        case SLDC_118:
        case SLDC_119:
        case SLDC_120:
        case SLDC_121:
        case SLDC_122:
        case SLDC_123:
        case SLDC_124:
        case SLDC_125:
        case SLDC_126:
        case SLDC_127:
            break;

        case ABI:
        case ABR:
        case ADI:
        case ADR:
        case LAND:
        case DIF:
        case DVI:
        case DVR:
        case CHK:
        case FLO:
        case FLT:
        case INN:
        case INT:
        case LOR:
        case MODI:
        case MPI:
        case MPR:
        case NGI:
        case NGR:
        case LNOT:
        case SRS:
        case SBI:
        case SBR:
        case SGS:
        case SQI:
        case SQR:
        case STO:
        case IXS:
        case UNI:
            break;

        case LDE:
            {
                args_db_b:
                // DB (unsigned byte)
                ++address;

                // B (big)
                unsigned value = data[address];
                ++address;
                if (value & 0x80)
                    ++address;
            }
            break;

        case CSP:
            ++address;
            break;

        case LDCN:
            break;

        case ADJ:
            goto args_ub;

        case FJP:
            goto args_jump;

        case INC:
            goto args_b;

        case IND:
            {
                args_b:
                // B (big)
                unsigned value = data[address];
                ++address;
                if (value & 0x80)
                    ++address;
            }
            break;

        case IXA:
            goto args_b;

        case LAO:
            goto args_b;

        case LSA:
            {
                unsigned len = data[address++];
                address += len;
            }
            break;

        case LAE:
            goto args_db_b;

        case MOV:
        case LDO:
            goto args_b;

        case SAS:
            goto args_ub;

        case SRO:
            goto args_b;

        case XJP:
            {
                // Case jump.
                if (address & 1)
                    ++address;

                int w1 = get_word(data + address);
                int w2 = get_word(data + address + 2);
                if (w2 < w1)
                    w1 -= (1 << 16);
                address += 4;

                if (data[address] == UJP)
                {
                    int n = data[address + 1];
                    if (n & 0x80)
                    {
                        n -= 256;
                        unsigned short dstadr = proc_attr_table_offset + n;
                        if (dstadr < jump_table_ic)
                        {
                            jump_table_ic = dstadr;
                            dstadr -= get_word(data + dstadr);
                            where.add(dstadr);
                        }
                    }
                }
                address += 2;

                for (int n = w1; n <= w2 && address < exit_ic; ++n)
                {
                    unsigned short dstadr = address - get_word(data + address);
                    where.add(dstadr);
                    address += 2;
                }
            }
            break;

        case RNP:
        case CIP:
            goto args_ub;

        case EQU:
        case GEQ:
        case GTR:
            goto args_type;

        case LDA:
            goto args_db_b;

        case LDC:
            {
                unsigned len = data[address++];
                if (address & 1)
                    ++address;
                address += 2 * len;
            }
            break;

        case LEQ:
        case LES:
            goto args_type;

        case LOD:
            goto args_db_b;

        case NEQ:
            args_type:
            c = data[address++];
            switch (c)
            {
            case 2:
            case 4:
            case 6:
            case 8:
                break;

            case 10:
            case 12:
                goto args_b;

            default:
                break;
            }
            break;

        case STR:
            goto args_db_b;

        case UJP:
            {
                args_jump:
                // SB
                int n = data[address];
                ++address;
                if (n & 0x80)
                {
                    n -= 256;
                    unsigned short dstadr = proc_attr_table_offset + n;
                    if (dstadr < jump_table_ic)
                        jump_table_ic = dstadr;
                    dstadr -= get_word(data + dstadr);
                    where.add(dstadr);
                }
                else
                {
                    where.add(address + n);
                }
            }
            break;

        case LDP:
        case STP:
            break;

        case LDM:
        case STM:
            args_ub:
            // UB
            ++address;
            break;

        case LDB:
        case STB:
            break;

        case IXP:
            args_ub_ub:
            // UB, UB
            ++address;
            ++address;
            break;

        case RBP:
            // goto args_db;
            goto args_ub;

        case CBP:
            goto args_ub;

        case EQUI:
        case GEQI:
        case GTRI:
            break;

        case LLA:
            goto args_b;

        case LDCI:
            address += 2;
            break;

        case LEQI:
        case LESI:
            break;

        case LDL:
            goto args_b;

        case NEQI:
            break;

        case STL:
            goto args_b;

        case CXP:
            goto args_ub_ub;

        case CLP:
        case CGP:
            goto args_ub;

        case LPA:
            {
                unsigned len = data[address++];
                address += len;
            }
            break;

        case STE:
            goto args_db_b;

        case EFJ:
        case NFJ:
            goto args_jump;

        case BPT:
            goto args_b;

        case XIT:
        case NOP:
            break;

        case SLDL_1:
        case SLDL_2:
        case SLDL_3:
        case SLDL_4:
        case SLDL_5:
        case SLDL_6:
        case SLDL_7:
        case SLDL_8:
        case SLDL_9:
        case SLDL_10:
        case SLDL_11:
        case SLDL_12:
        case SLDL_13:
        case SLDL_14:
        case SLDL_15:
        case SLDL_16:
            break;

        case SLDO_1:
        case SLDO_2:
        case SLDO_3:
        case SLDO_4:
        case SLDO_5:
        case SLDO_6:
        case SLDO_7:
        case SLDO_8:
        case SLDO_9:
        case SLDO_10:
        case SLDO_11:
        case SLDO_12:
        case SLDO_13:
        case SLDO_14:
        case SLDO_15:
        case SLDO_16:
            break;

        case SIND_0:
        case SIND_1:
        case SIND_2:
        case SIND_3:
        case SIND_4:
        case SIND_5:
        case SIND_6:
        case SIND_7:
            break;
        }
    }
    DEBUG(1, "}");
}


struct opcode_info_t
{
    const char *name;
    const char *description;
};


static const opcode_info_t opcode_info[256] =
{
    { "SLDC", "Short Load Constant"    }, //   0
    { "SLDC", "Short Load Constant"    }, //   1
    { "SLDC", "Short Load Constant"    }, //   2
    { "SLDC", "Short Load Constant"    }, //   3
    { "SLDC", "Short Load Constant"    }, //   4
    { "SLDC", "Short Load Constant"    }, //   5
    { "SLDC", "Short Load Constant"    }, //   6
    { "SLDC", "Short Load Constant"    }, //   7
    { "SLDC", "Short Load Constant"    }, //   8
    { "SLDC", "Short Load Constant"    }, //   9
    { "SLDC", "Short Load Constant"    }, //  10
    { "SLDC", "Short Load Constant"    }, //  11
    { "SLDC", "Short Load Constant"    }, //  12
    { "SLDC", "Short Load Constant"    }, //  13
    { "SLDC", "Short Load Constant"    }, //  14
    { "SLDC", "Short Load Constant"    }, //  15
    { "SLDC", "Short Load Constant"    }, //  16
    { "SLDC", "Short Load Constant"    }, //  17
    { "SLDC", "Short Load Constant"    }, //  18
    { "SLDC", "Short Load Constant"    }, //  19
    { "SLDC", "Short Load Constant"    }, //  20
    { "SLDC", "Short Load Constant"    }, //  21
    { "SLDC", "Short Load Constant"    }, //  22
    { "SLDC", "Short Load Constant"    }, //  23
    { "SLDC", "Short Load Constant"    }, //  24
    { "SLDC", "Short Load Constant"    }, //  25
    { "SLDC", "Short Load Constant"    }, //  26
    { "SLDC", "Short Load Constant"    }, //  27
    { "SLDC", "Short Load Constant"    }, //  28
    { "SLDC", "Short Load Constant"    }, //  29
    { "SLDC", "Short Load Constant"    }, //  30
    { "SLDC", "Short Load Constant"    }, //  31
    { "SLDC", "Short Load Constant"    }, //  32
    { "SLDC", "Short Load Constant"    }, //  33
    { "SLDC", "Short Load Constant"    }, //  34
    { "SLDC", "Short Load Constant"    }, //  35
    { "SLDC", "Short Load Constant"    }, //  36
    { "SLDC", "Short Load Constant"    }, //  37
    { "SLDC", "Short Load Constant"    }, //  38
    { "SLDC", "Short Load Constant"    }, //  39
    { "SLDC", "Short Load Constant"    }, //  40
    { "SLDC", "Short Load Constant"    }, //  41
    { "SLDC", "Short Load Constant"    }, //  42
    { "SLDC", "Short Load Constant"    }, //  43
    { "SLDC", "Short Load Constant"    }, //  44
    { "SLDC", "Short Load Constant"    }, //  45
    { "SLDC", "Short Load Constant"    }, //  46
    { "SLDC", "Short Load Constant"    }, //  47
    { "SLDC", "Short Load Constant"    }, //  48
    { "SLDC", "Short Load Constant"    }, //  49
    { "SLDC", "Short Load Constant"    }, //  50
    { "SLDC", "Short Load Constant"    }, //  51
    { "SLDC", "Short Load Constant"    }, //  52
    { "SLDC", "Short Load Constant"    }, //  53
    { "SLDC", "Short Load Constant"    }, //  54
    { "SLDC", "Short Load Constant"    }, //  55
    { "SLDC", "Short Load Constant"    }, //  56
    { "SLDC", "Short Load Constant"    }, //  57
    { "SLDC", "Short Load Constant"    }, //  58
    { "SLDC", "Short Load Constant"    }, //  59
    { "SLDC", "Short Load Constant"    }, //  60
    { "SLDC", "Short Load Constant"    }, //  61
    { "SLDC", "Short Load Constant"    }, //  62
    { "SLDC", "Short Load Constant"    }, //  63
    { "SLDC", "Short Load Constant"    }, //  64
    { "SLDC", "Short Load Constant"    }, //  65
    { "SLDC", "Short Load Constant"    }, //  66
    { "SLDC", "Short Load Constant"    }, //  67
    { "SLDC", "Short Load Constant"    }, //  68
    { "SLDC", "Short Load Constant"    }, //  69
    { "SLDC", "Short Load Constant"    }, //  70
    { "SLDC", "Short Load Constant"    }, //  71
    { "SLDC", "Short Load Constant"    }, //  72
    { "SLDC", "Short Load Constant"    }, //  73
    { "SLDC", "Short Load Constant"    }, //  74
    { "SLDC", "Short Load Constant"    }, //  75
    { "SLDC", "Short Load Constant"    }, //  76
    { "SLDC", "Short Load Constant"    }, //  77
    { "SLDC", "Short Load Constant"    }, //  78
    { "SLDC", "Short Load Constant"    }, //  79
    { "SLDC", "Short Load Constant"    }, //  80
    { "SLDC", "Short Load Constant"    }, //  81
    { "SLDC", "Short Load Constant"    }, //  82
    { "SLDC", "Short Load Constant"    }, //  83
    { "SLDC", "Short Load Constant"    }, //  84
    { "SLDC", "Short Load Constant"    }, //  85
    { "SLDC", "Short Load Constant"    }, //  86
    { "SLDC", "Short Load Constant"    }, //  87
    { "SLDC", "Short Load Constant"    }, //  88
    { "SLDC", "Short Load Constant"    }, //  89
    { "SLDC", "Short Load Constant"    }, //  90
    { "SLDC", "Short Load Constant"    }, //  91
    { "SLDC", "Short Load Constant"    }, //  92
    { "SLDC", "Short Load Constant"    }, //  93
    { "SLDC", "Short Load Constant"    }, //  94
    { "SLDC", "Short Load Constant"    }, //  95
    { "SLDC", "Short Load Constant"    }, //  96
    { "SLDC", "Short Load Constant"    }, //  97
    { "SLDC", "Short Load Constant"    }, //  98
    { "SLDC", "Short Load Constant"    }, //  99
    { "SLDC", "Short Load Constant"    }, // 100
    { "SLDC", "Short Load Constant"    }, // 101
    { "SLDC", "Short Load Constant"    }, // 102
    { "SLDC", "Short Load Constant"    }, // 103
    { "SLDC", "Short Load Constant"    }, // 104
    { "SLDC", "Short Load Constant"    }, // 105
    { "SLDC", "Short Load Constant"    }, // 106
    { "SLDC", "Short Load Constant"    }, // 107
    { "SLDC", "Short Load Constant"    }, // 108
    { "SLDC", "Short Load Constant"    }, // 109
    { "SLDC", "Short Load Constant"    }, // 110
    { "SLDC", "Short Load Constant"    }, // 111
    { "SLDC", "Short Load Constant"    }, // 112
    { "SLDC", "Short Load Constant"    }, // 113
    { "SLDC", "Short Load Constant"    }, // 114
    { "SLDC", "Short Load Constant"    }, // 115
    { "SLDC", "Short Load Constant"    }, // 116
    { "SLDC", "Short Load Constant"    }, // 117
    { "SLDC", "Short Load Constant"    }, // 118
    { "SLDC", "Short Load Constant"    }, // 119
    { "SLDC", "Short Load Constant"    }, // 120
    { "SLDC", "Short Load Constant"    }, // 121
    { "SLDC", "Short Load Constant"    }, // 122
    { "SLDC", "Short Load Constant"    }, // 123
    { "SLDC", "Short Load Constant"    }, // 124
    { "SLDC", "Short Load Constant"    }, // 125
    { "SLDC", "Short Load Constant"    }, // 126
    { "SLDC", "Short Load Constant"    }, // 127
    { "ABI",  "Absolute value integer" }, // 128
    { "ABR",  "Absolute value real"    }, // 129
    { "ADI",  "Add integers"           }, // 130
    { "ADR",  "Add reals"              }, // 131
    { "LAND", "Logical AND"            }, // 132
    { "DIF",  "Set difference"         }, // 133
    { "DVI",  "Divide integers"        }, // 134
    { "DVR",  "Divide reals"           }, // 135
    { "CHK",  "Check subrange bounds"  }, // 136
    { "FLO",  "Float top-of-stack - 1" }, // 137
    { "FLT",  "Float top-of-stack"     }, // 138
    { "INN",  "Test set membership"    }, // 139
    { "INT",  "Set intersection"       }, // 140
    { "LOR",  "Logical OR"             }, // 141
    { "MODI", "Modulo integers"        }, // 142
    { "MPI",  "Multiply integers"      }, // 143
    { "MPR",  "Multiply reals"         }, // 144
    { "NGI",  "Negate integer"         }, // 145
    { "NGR",  "Negate real"            }, // 146
    { "LNOT", "Logical NOT"            }, // 147
    { "SRS",  "Build subrange set"     }, // 148
    { "SBI",  "Subtract integers"      }, // 149
    { "SBR",  "Subtract reals"         }, // 150
    { "SGS",  "Build one-member set"   }, // 151
    { "SQI",  "Square integer"         }, // 152
    { "SQR",  "Square real"            }, // 153
    { "STO",  "Store Indirect"         }, // 154
    { "IXS",  "Index String array"     }, // 155
    { "UNI",  "Set union"              }, // 156
    { "LDE",  "Load Extended"          }, // 157
    { "CSP",  "Call Standard Proc"     }, // 158
    { "LDCN", "Load Constant NIL"      }, // 159
    { "ADJ",  "Adjust set"             }, // 160
    { "FJP",  "False jump"             }, // 161
    { "INC",  "Incriment field ptr"    }, // 162
    { "IND",  "Static index and load"  }, // 163
    { "IXA",  "Index array"            }, // 164
    { "LAO",  "Load Global Address"    }, // 165
    { "LSA",  "Load const Strng Addr"  }, // 166
    { "LAE",  "Load Extended Address"  }, // 167
    { "MOV",  "Move words"             }, // 168
    { "LDO",  "Load Global"            }, // 169
    { "SAS",  "String Assign"          }, // 170
    { "SRO",  "Store Global"           }, // 171
    { "XJP",  "Case jump"              }, // 172
    { "RNP",  "Return Non-base Proc"   }, // 173
    { "CIP",  "Call Intermediate Proc" }, // 174
    { "EQU",  "Test ="                 }, // 175
    { "GEQ",  "test >="                }, // 176
    { "GTR",  "Test >"                 }, // 177
    { "LDA",  "Load intermediate Addr" }, // 178
    { "LDC",  "Load Multi-word Const"  }, // 179
    { "LEQ",  "Test <="                }, // 180
    { "LES",  "Test <"                 }, // 181
    { "LOD",  "Load Intermediate"      }, // 182
    { "NEQ",  "Test <>"                }, // 183
    { "STR",  "Stote Intermediate"     }, // 184
    { "UJP",  "Unconditional Jump"     }, // 185
    { "LDP",  "Load packed field"      }, // 186
    { "STP",  "Store packed field"     }, // 187
    { "LDM",  "Load Multi-word"        }, // 188
    { "STM",  "Store Multi-word"       }, // 189
    { "LDB",  "Load Byte"              }, // 190
    { "STB",  "Store Byte"             }, // 191
    { "IXP",  "Index packed array"     }, // 192
    { "RBP",  "Return from base proc"  }, // 193
    { "CBP",  "Call base procedure"    }, // 194
    { "EQUI", "Test integer ="         }, // 195
    { "GEQI", "Test integer >="        }, // 196
    { "GTRI", "Test integer >"         }, // 197
    { "LLA",  "Load Local Address"     }, // 198
    { "LDCI", "Load constant integer"  }, // 199
    { "LEQI", "Test integer <="        }, // 200
    { "LESI", "Test integer <"         }, // 201
    { "LDL",  "Load Local"             }, // 202
    { "NEQI", "Test integer <>"        }, // 203
    { "STL",  "Store Local"            }, // 204
    { "CXP",  "Call external proc"     }, // 205
    { "CLP",  "Call local procedure"   }, // 206
    { "CGP",  "Call global procedure"  }, // 207
    { "LPA",  "Load packed array"      }, // 208
    { "STE",  "Store Extended"         }, // 209
    { "???",  0                        }, // 210
    { "EFJ",  "Equal False Jump"       }, // 211
    { "NFJ",  "Not equal False Jump"   }, // 212
    { "BPT",  "Breakpoint"             }, // 213
    { "XIT",  "Exit the p-system"      }, // 214
    { "NOP",  "No operation"           }, // 215
    { "SLDL", "Short Load Local"       }, // 216
    { "SLDL", "Short Load Local"       }, // 217
    { "SLDL", "Short Load Local"       }, // 218
    { "SLDL", "Short Load Local"       }, // 219
    { "SLDL", "Short Load Local"       }, // 220
    { "SLDL", "Short Load Local"       }, // 221
    { "SLDL", "Short Load Local"       }, // 222
    { "SLDL", "Short Load Local"       }, // 223
    { "SLDL", "Short Load Local"       }, // 224
    { "SLDL", "Short Load Local"       }, // 225
    { "SLDL", "Short Load Local"       }, // 226
    { "SLDL", "Short Load Local"       }, // 227
    { "SLDL", "Short Load Local"       }, // 228
    { "SLDL", "Short Load Local"       }, // 229
    { "SLDL", "Short Load Local"       }, // 230
    { "SLDL", "Short Load Local"       }, // 231
    { "SLDO", "Short Load Global"      }, // 232
    { "SLDO", "Short Load Global"      }, // 233
    { "SLDO", "Short Load Global"      }, // 234
    { "SLDO", "Short Load Global"      }, // 235
    { "SLDO", "Short Load Global"      }, // 236
    { "SLDO", "Short Load Global"      }, // 237
    { "SLDO", "Short Load Global"      }, // 238
    { "SLDO", "Short Load Global"      }, // 239
    { "SLDO", "Short Load Global"      }, // 240
    { "SLDO", "Short Load Global"      }, // 241
    { "SLDO", "Short Load Global"      }, // 242
    { "SLDO", "Short Load Global"      }, // 243
    { "SLDO", "Short Load Global"      }, // 244
    { "SLDO", "Short Load Global"      }, // 245
    { "SLDO", "Short Load Global"      }, // 246
    { "SLDO", "Short Load Global"      }, // 247
    { "SIND", "Short Load Indirect"    }, // 248
    { "SIND", "Short Load Indirect"    }, // 249
    { "SIND", "Short Load Indirect"    }, // 250
    { "SIND", "Short Load Indirect"    }, // 251
    { "SIND", "Short Load Indirect"    }, // 252
    { "SIND", "Short Load Indirect"    }, // 253
    { "SIND", "Short Load Indirect"    }, // 254
    { "SIND", "Short Load Indirect"    }, // 255
};


static bool
all_printable(const unsigned char *data, size_t data_size)
{
    while (data_size)
    {
        if (!isprint(*data))
            return false;
        ++data;
        --data_size;
    }
    return true;
}


unsigned
disassembler_pcode_ii_1::disassemble_one_instruction(const unsigned char *data,
    unsigned address, size_t, int proc_number)
{
    if (address < entry_ic)
        return 0;

    unsigned start_address = address;
    if (address < jump_table_ic)
    {
        unsigned char c = data[address++];
        report.data_column->fprintf("%02X", c);
        report.opcode_column->fputs(opcode_info[c].name);
        switch (c)
        {
        case SLDC_0:
        case SLDC_1:
        case SLDC_2:
        case SLDC_3:
        case SLDC_4:
        case SLDC_5:
        case SLDC_6:
        case SLDC_7:
        case SLDC_8:
        case SLDC_9:
        case SLDC_10:
        case SLDC_11:
        case SLDC_12:
        case SLDC_13:
        case SLDC_14:
        case SLDC_15:
        case SLDC_16:
        case SLDC_17:
        case SLDC_18:
        case SLDC_19:
        case SLDC_20:
        case SLDC_21:
        case SLDC_22:
        case SLDC_23:
        case SLDC_24:
        case SLDC_25:
        case SLDC_26:
        case SLDC_27:
        case SLDC_28:
        case SLDC_29:
        case SLDC_30:
        case SLDC_31:
        case SLDC_32:
        case SLDC_33:
        case SLDC_34:
        case SLDC_35:
        case SLDC_36:
        case SLDC_37:
        case SLDC_38:
        case SLDC_39:
        case SLDC_40:
        case SLDC_41:
        case SLDC_42:
        case SLDC_43:
        case SLDC_44:
        case SLDC_45:
        case SLDC_46:
        case SLDC_47:
        case SLDC_48:
        case SLDC_49:
        case SLDC_50:
        case SLDC_51:
        case SLDC_52:
        case SLDC_53:
        case SLDC_54:
        case SLDC_55:
        case SLDC_56:
        case SLDC_57:
        case SLDC_58:
        case SLDC_59:
        case SLDC_60:
        case SLDC_61:
        case SLDC_62:
        case SLDC_63:
        case SLDC_64:
        case SLDC_65:
        case SLDC_66:
        case SLDC_67:
        case SLDC_68:
        case SLDC_69:
        case SLDC_70:
        case SLDC_71:
        case SLDC_72:
        case SLDC_73:
        case SLDC_74:
        case SLDC_75:
        case SLDC_76:
        case SLDC_77:
        case SLDC_78:
        case SLDC_79:
        case SLDC_80:
        case SLDC_81:
        case SLDC_82:
        case SLDC_83:
        case SLDC_84:
        case SLDC_85:
        case SLDC_86:
        case SLDC_87:
        case SLDC_88:
        case SLDC_89:
        case SLDC_90:
        case SLDC_91:
        case SLDC_92:
        case SLDC_93:
        case SLDC_94:
        case SLDC_95:
        case SLDC_96:
        case SLDC_97:
        case SLDC_98:
        case SLDC_99:
        case SLDC_100:
        case SLDC_101:
        case SLDC_102:
        case SLDC_103:
        case SLDC_104:
        case SLDC_105:
        case SLDC_106:
        case SLDC_107:
        case SLDC_108:
        case SLDC_109:
        case SLDC_110:
        case SLDC_111:
        case SLDC_112:
        case SLDC_113:
        case SLDC_114:
        case SLDC_115:
        case SLDC_116:
        case SLDC_117:
        case SLDC_118:
        case SLDC_119:
        case SLDC_120:
        case SLDC_121:
        case SLDC_122:
        case SLDC_123:
        case SLDC_124:
        case SLDC_125:
        case SLDC_126:
        case SLDC_127:
            report.argument_column->fprintf("%d", c);
            if (isprint(c))
            {
                if (c == '\'')
                    report.comment_column->fputs("; ''''");
                else
                    report.comment_column->fprintf("; '%c'", c);
            }
            break;

        case ABI:
        case ABR:
        case ADI:
        case ADR:
        case LAND:
        case DIF:
        case DVI:
        case DVR:
        case CHK:
        case FLO:
        case FLT:
        case INN:
        case INT:
        case LOR:
        case MODI:
        case MPI:
        case MPR:
        case NGI:
        case NGR:
        case LNOT:
        case SRS:
        case SBI:
        case SBR:
        case SGS:
        case SQI:
        case SQR:
        case STO:
        case IXS:
        case UNI:
            break;

        case LDE:
            {
                args_db_b:
                // DB (unsigned byte)
                report.data_column->fprintf(" %02X", data[address]);
                report.argument_column->fprintf("%d", data[address]);
                ++address;

                // B (big)
                report.data_column->fprintf(" %02X", data[address]);
                unsigned value = data[address];
                ++address;
                if (value & 0x80)
                {
                    report.data_column->fprintf(" %02X", data[address]);
                    value = ((value & 0x7F) << 8) + data[address];
                    ++address;
                }
                report.argument_column->fprintf(", %d", value);
            }
            break;

        case CSP:
            {
                static const char *const csp[256] =
                {
                    "IOCHECK",       //   0
                    "NEW",           //   1
                    "MOVELEFT",      //   2
                    "MOVERIGHT",     //   3
                    "EXIT",          //   4
                    "UNITREAD",      //   5
                    "UNITWRITE",     //   6
                    "IDSEARCH",      //   7
                    "TREESEARCH",    //   8
                    "TIME",          //   9
                    "FILLCHAR",      //  10
                    "SCAN",          //  11
                    "UNITSTATUS",    //  12
                    0,               //  13
                    0,               //  14
                    0,               //  15
                    0,               //  16
                    0,               //  17
                    0,               //  18
                    0,               //  19
                    0,               //  20
                    "LOADSEGMENT",   //  21
                    "UNLOADSEGMENT", //  22
                    "TRUNC",         //  23
                    "ROUND",         //  24
                    "SIN",           //  25
                    "COS",           //  26
                    "LOG",           //  27
                    "ATAN",          //  28
                    "LN",            //  29
                    "EXP",           //  30
                    "SQRT",          //  31
                    "MARK",          //  32
                    "RELEASE",       //  33
                    "IORESULT",      //  34
                    "UNITBUSY",      //  35
                    "PWROFTEN",      //  36
                    "UNITWAIT",      //  37
                    "UNITCLEAR",     //  38
                    "HALT",          //  39
                    "MEMAVAIL",      //  40
                };

                c = data[address++];
                report.data_column->fprintf(" %02X", c);
                const char *n2 = csp[c];
                if (n2 && *n2)
                    report.argument_column->fputs(n2);
                else
                {
                    report.argument_column->fprintf("%d", c);
                    report.comment_column->fputs("; unknown");
                }
            }
            break;

        case LDCN:
            break;

        case ADJ:
            goto args_ub;

        case FJP:
            goto args_jump;

        case INC:
            goto args_b;

        case IND:
            {
                args_b:
                // B (big)
                report.data_column->fprintf(" %02X", data[address]);
                unsigned value = data[address];
                ++address;
                if (value & 0x80)
                {
                    report.data_column->fprintf(" %02X", data[address]);
                    value = ((value & 0x7F) << 8) + data[address];
                    ++address;
                }
                report.argument_column->fprintf("%d", value);

                if (c == STL && value == 0)
                {
                    report.comment_column->fprintf("invalid");
                }
            }
            break;

        case IXA:
            goto args_b;

        case LAO:
            goto args_b;

        case LSA:
            {
                unsigned len = data[address];
                report.data_column->fprintf(" %02X", len);
                report.argument_column->fprintf("%d, ", len);
                ++address;

                rcstring s((char *)(data + address), len);
                report.argument_column->fputs(s.quote_c());

                while (len > 0)
                {
                    report.data_column->fprintf(" %02X", data[address]);
                    ++address;
                    --len;
                }
            }
            break;

        case LAE:
            goto args_db_b;

        case MOV:
        case LDO:
            goto args_b;

        case SAS:
            goto args_ub;

        case SRO:
            goto args_b;

        case XJP:
            {
                if (address & 1)
                {
                    // word alignment padding
                    report.data_column->fprintf(" %02X", data[address]);
                    ++address;
                }

                // Case jump.
                report.data_column->fprintf(" %02X", data[address]);
                report.data_column->fprintf(" %02X", data[address + 1]);
                int w1 = get_word(data + address);
                int w2 = get_word(data + address + 2);
                if (w2 < w1)
                    w1 -= (1 << 16);
                report.argument_column->fprintf("%d", w1);
                address += 2;

                report.data_column->fprintf(" %02X", data[address]);
                report.data_column->fprintf(" %02X", data[address + 1]);
                report.argument_column->fprintf(", %d", w2);
                report.comment_column->fputs("; switch");
                address += 2;
                report.eoln();

                // Wow this is f... this is fun.
                // The otherwise clause is actually a UJP opcode.
                if (data[address] == UJP)
                {
                    {
                        rcstring lbl = where.get_name(address);
                        if (!lbl.empty())
                        {
                            report.label_column->fputs(lbl);
                            report.label_column->fputc(':');
                        }
                    }

                    report.data_column->fprintf("%02X", data[address++]);
                    report.data_column->fprintf(" %02X", data[address++]);
                    int n = data[address - 1];
                    report.opcode_column->fputs("UJP");

                    if (n & 0x80)
                    {
                        n -= 256;
                        unsigned short dstadr = proc_attr_table_offset + n;
                        if (dstadr < jump_table_ic)
                            jump_table_ic = dstadr;
                        dstadr -= get_word(data + dstadr);
                        rcstring dst = where.get_name(dstadr);
                        if (dst.empty())
                            report.argument_column->fprintf("JTAB[%d]", n);
                        else
                            report.argument_column->fputs(dst);
                    }
                    else
                    {
                        where.add(address + n);
                        rcstring dst = where.get_name(address + n);
                        assert(!dst.empty());
                        report.argument_column->fputs(dst);
                    }
                }
                else
                {
                    report.data_column->fprintf("%02X", data[address]);
                    report.data_column->fprintf(" %02X", data[address + 1]);
                    int w3 = get_word(data + address);
                    address += 2;
                    report.opcode_column->fputs(".word");
                    report.argument_column->fprintf("%d", w3);
                }
                report.comment_column->fputs("; otherwise");

                for (int n = w1; n <= w2 && address < exit_ic; ++n)
                {
                    report.eoln();
                    report.data_column->fprintf("%02X", data[address]);
                    report.data_column->fprintf(" %02X", data[address + 1]);
                    int w = get_word(data + address);
                    unsigned short dstadr = address - w;
                    address += 2;
                    report.opcode_column->fputs(".word");
                    report.argument_column->fputs("$ - ");
                    rcstring dst = where.get_name(dstadr);
                    if (dst.empty())
                        report.argument_column->fprintf("0x%04X", dstadr);
                    else
                        report.argument_column->fputs(dst);
                    report.comment_column->fprintf("; case %d", n);
                    if (n >= ' ' && n <= '~' && c != '\'' && c != '\\')
                        report.comment_column->fprintf(" '%c'", n);
                }
            }
            break;

        case RNP:
        case CIP:
            goto args_ub;

        case EQU:
        case GEQ:
        case GTR:
            goto args_type;

        case LDA:
            goto args_db_b;

        case LDC:
            {
                unsigned len = data[address];
                report.data_column->fprintf(" %02X", len);
                report.argument_column->fprintf("%d", len);
                ++address;

                // The data is to be word aligned.
                if (address & 1)
                {
                    report.data_column->fprintf(" %02X", data[address]);
                    ++address;
                }

                if (len == 2)
                {
                    float x;
                    unsigned char *xp = (unsigned char *)&x;
                    xp[0] = data[address + 2];
                    xp[1] = data[address + 3];
                    xp[2] = data[address + 0];
                    xp[3] = data[address + 1];
                    report.comment_column->fprintf("; %g", x);
                }
                while (len > 0)
                {
                    report.data_column->fprintf
                    (
                        " %02X %02X",
                        data[address],
                        data[address + 1]
                    );
                    unsigned value = get_word(data + address);
                    address += 2;
                    report.argument_column->fprintf(", %d", value);
                    --len;
                }
            }
            break;

        case LEQ:
        case LES:
            goto args_type;

        case LOD:
            goto args_db_b;

        case NEQ:
            args_type:
            c = data[address++];
            report.data_column->fprintf(" %02X", c);
            switch (c)
            {
            case 2:
                report.argument_column->fputs("REAL");
                break;

            case 4:
                report.argument_column->fputs("STR");
                break;

            case 6:
                report.argument_column->fputs("BOOL");
                break;

            case 8:
                report.argument_column->fputs("POWR");
                break;

            case 10:
                report.argument_column->fputs("BYT, ");

                cmp_big:
                // B (big)
                {
                        report.data_column->fprintf(" %02X", data[address]);
                        unsigned value = data[address];
                        ++address;
                        if (value & 0x80)
                        {
                            report.data_column->fprintf(" %02X", data[address]);
                            value = ((value & 0x7F) << 8) + data[address];
                            ++address;
                        }
                        report.argument_column->fprintf("%d", value);
                }
                break;

            case 12:
                report.argument_column->fputs("WORD, ");
                goto cmp_big;

            default:
                report.argument_column->fprintf("%d", c);
                report.comment_column->fputs("; unknown");
                break;
            }
            break;

        case STR:
            goto args_db_b;

        case UJP:
            {
                args_jump:
                // SB
                int n = data[address];
                report.data_column->fprintf(" %02X", n);
                ++address;
                if (n & 0x80)
                {
                    n -= 256;
                    unsigned short dstadr = proc_attr_table_offset + n;
                    if (dstadr < jump_table_ic)
                        jump_table_ic = dstadr;
                    dstadr -= get_word(data + dstadr);
                    rcstring dst = where.get_name(dstadr);
                    if (dst.empty())
                        report.argument_column->fprintf("JTAB[%d]", n);
                    else
                        report.argument_column->fputs(dst);
                }
                else
                {
                    where.add(address + n);
                    rcstring dst = where.get_name(address + n);
                    assert(!dst.empty());
                    report.argument_column->fputs(dst);
                }
            }
            break;

        case LDP:
        case STP:
            break;

        case LDM:
        case STM:
            args_ub:
            // UB
            report.data_column->fprintf(" %02X", data[address]);
            report.argument_column->fprintf("%d", data[address]);
            ++address;
            break;

        case LDB:
        case STB:
            break;

        case IXP:
            args_ub_ub:
            // UB, UB
            report.data_column->fprintf(" %02X", data[address]);
            report.argument_column->fprintf("%d", data[address]);
            ++address;
            report.data_column->fprintf(" %02X", data[address]);
            report.argument_column->fprintf(", %d", data[address]);
            ++address;

            if (c == CXP)
            {
                switch (data[address - 2])
                {
                case 0:
                    {
                        unsigned n = data[address - 1];
                        static const char *const syscall[] =
                        {
                            0,              // proc 0 does not exist
                            0,              // proc 1 is the system program
                            "execerror",
                            "finit",
                            "freset",
                            "fopen",
                            "fclose",
                            "fget",
                            "fput",
                            "xseek",
                            "feof",
                            "feoln",
                            "freadint",
                            "fwriteint",
                            "xreadreal",
                            "xwritereal",
                            "freadchar",
                            "fwritechar",
                            "freadstring",
                            "fwritestring",
                            "fwritebytes",
                            "freadln",
                            "fwriteln",
                            "sconcat",
                            "sinsert",
                            "scopy",
                            "sdelete",
                            "spos",
                            "fblockio",
                            "fgotoxy",
                        };
                        if (n < SIZEOF(syscall) && syscall[n])
                            report.comment_column->fprintf("; %s", syscall[n]);
                    }
                    break;

                case 20:
                    {
                        unsigned n = data[address - 1];
                        static const char *const syscall[] =
                        {
                            0,
                            0,
                            "initturtle",
                            "turn",
                            "turnto",
                            "move",
                            "moveto",
                            "pencolor",
                            "textmode",
                            "grafmode",
                            "fillscreen",
                            "viewport",
                            "turtlex",
                            "turtley",
                            "turtleang",
                            "screenbit",
                            "drawblock",
                            "wchar",
                            "wstring",
                            "chartype",
                        };
                        if (n < SIZEOF(syscall) && syscall[n])
                        {
                            report.comment_column->fprintf
                            (
                                "; %s (TURTLEGR)",
                                syscall[n]
                            );
                        }
                    }
                    break;

                case 22:
                    {
                        unsigned n = data[address - 1];
                        static const char *const syscall[] =
                        {
                            0,
                            0,
                            "paddle",
                            "button",
                            "ttlout",
                            "keypress",
                            "random",
                            "randomize",
                            "note",
                        };
                        if (n < SIZEOF(syscall) && syscall[n])
                        {
                            report.comment_column->fprintf
                            (
                                "; %s (APPLESTU)",
                                syscall[n]
                            );
                        }
                    }
                    break;

                case 28:
                    {
                        unsigned n = data[address - 1];
                        static const char *const syscall[] =
                        {
                            0,
                            0,
                            "setchain",
                            "setcval",
                            "getcval",
                            "swapon",
                            "swapoff",
                        };
                        if (n < SIZEOF(syscall) && syscall[n])
                        {
                            report.comment_column->fprintf
                            (
                                "; %s (CHAINSTU)",
                                syscall[n]
                            );
                        }
                    }
                    break;

                case 29:
                    {
                        unsigned n = data[address - 1];
                        static const char *const syscall[] =
                        {
                            0,
                            0,
                            "sin",
                            "cos",
                            "exp",
                            "atan",
                            "ln",
                            "log",
                            "sqrt",
                        };
                        if (n < SIZEOF(syscall) && syscall[n])
                        {
                            report.comment_column->fprintf
                            (
                                "; %s (TRANSCEN)",
                                syscall[n]
                            );
                        }
                    }
                    break;

                case 30:
                    {
                        unsigned n = data[address - 1];
                        static const char *const syscall[] =
                        {
                            0,
                            0,
                            "freaddec",
                            "fwritedec",
                            "decops",
                        };
                        if (n < SIZEOF(syscall) && syscall[n])
                        {
                            report.comment_column->fprintf
                            (
                                "; %s",
                                syscall[n]
                            );
                        }
                        if (n == DECOPS_PROC_NUM)
                        {
                            // The compiler always emits a SLDC opcode
                            // to select the operation to be performed.
                            n = data[address - 4];

                            static const char *names[] =
                            {
                                "adjust",
                                0,
                                "add",
                                0,
                                "subtract",
                                0,
                                "negate",
                                0,
                                "multiply",
                                0,
                                "divide",
                                0,
                                "str()",
                                0,
                                "convert tos-1",
                                0,
                                "compare",
                                0,
                                "convert",
                                0,
                                "trunc",
                                0,
                                "abs *",
                                0,
                                "sqr *",
                                0,
                                "mod *",
                            };
                            if (n < SIZEOF(names) && names[n])
                            {
                                report.comment_column->fprintf
                                (
                                    " %s",
                                    names[n]
                                );
                            }
                            if (n == DECOPS_COMPARE)
                            {
                                // The compiler always emits a SLDC opcode
                                // to select the operation to be performed.
                                unsigned n2 = data[address - 5];

                                static const char *names2[] =
                                {
                                    0, // 0
                                    0, // 1
                                    0, // 2
                                    0, // 3
                                    0, // 4
                                    0, // 5
                                    0, // 6
                                    0, // 7
                                    "lt",
                                    "le",
                                    "ge",
                                    "gt",
                                    "ne",
                                    "eq",
                                };
                                if (n2 < SIZEOF(names2) && names2[n2])
                                {
                                    report.comment_column->fprintf
                                    (
                                        " %s",
                                        names2[n2]
                                    );
                                }
                            }
                        }
                    }
                    break;

                case 31:
                    {
                        unsigned n = data[address - 1];
                        static const char *const syscall[] =
                        {
                            0,
                            0,
                            "fseek",
                            "freadreal",
                            "fwritereal",
                        };
                        if (n < SIZEOF(syscall) && syscall[n])
                        {
                            report.comment_column->fprintf
                            (
                                "; %s (PASCALIO)",
                                syscall[n]
                            );
                        }
                    }
                    break;
                }
            }
            break;

        case RBP:
        case CBP:
            goto args_ub;

        case EQUI:
        case GEQI:
        case GTRI:
            break;

        case LLA:
            goto args_b;

        case LDCI:
            {
                // W (word)
                report.data_column->fprintf
                (
                    " %02X %02X",
                    data[address],
                    data[address + 1]
                );
                int arg = get_word(data + address);
                if (arg & 0x8000)
                    arg -= (1 << 16);
                report.argument_column->fprintf("%d", arg);
                address+= 2;
            }
            break;

        case LEQI:
        case LESI:
            break;

        case LDL:
            goto args_b;

        case NEQI:
            break;

        case STL:
            goto args_b;

        case CXP:
            goto args_ub_ub;

        case CLP:
        case CGP:
            goto args_ub;

        case LPA:
            {
                unsigned len = data[address];
                report.data_column->fprintf(" %02X", len);
                report.argument_column->fprintf("%d", len);
                ++address;

                // If all of the bytes are printable characters, print a
                // string constant instead of a bunch of bytes.
                if (all_printable(data + address, len))
                {
                    rcstring text((char *)(data + address), len);
                    report.argument_column->fputs(", ");
                    report.argument_column->fputs(text.quote_c());

                    while (len > 0)
                    {
                        unsigned char n = data[address];
                        ++address;
                        report.data_column->fprintf(" %02X", n);
                        --len;
                    }
                }
                else
                {
                    while (len > 0)
                    {
                        unsigned char n = data[address];
                        ++address;
                        report.data_column->fprintf(" %02X", n);
                        report.argument_column->fprintf(", %d", n);
                        --len;
                    }
                }
            }
            break;

        case STE:
            goto args_db_b;

        case EFJ:
        case NFJ:
            goto args_jump;

        case BPT:
            goto args_b;

        case XIT:
        case NOP:
            break;

        case SLDL_1:
        case SLDL_2:
        case SLDL_3:
        case SLDL_4:
        case SLDL_5:
        case SLDL_6:
        case SLDL_7:
        case SLDL_8:
        case SLDL_9:
        case SLDL_10:
        case SLDL_11:
        case SLDL_12:
        case SLDL_13:
        case SLDL_14:
        case SLDL_15:
        case SLDL_16:
            report.argument_column->fprintf("%d", c - SLDL_1 + 1);
            break;

        case SLDO_1:
        case SLDO_2:
        case SLDO_3:
        case SLDO_4:
        case SLDO_5:
        case SLDO_6:
        case SLDO_7:
        case SLDO_8:
        case SLDO_9:
        case SLDO_10:
        case SLDO_11:
        case SLDO_12:
        case SLDO_13:
        case SLDO_14:
        case SLDO_15:
        case SLDO_16:
            report.argument_column->fprintf("%d", c - SLDO_1 + 1);
            break;

        case SIND_0:
        case SIND_1:
        case SIND_2:
        case SIND_3:
        case SIND_4:
        case SIND_5:
        case SIND_6:
        case SIND_7:
            report.argument_column->fprintf("%d", c - SIND_0);
            break;
        }
        if
        (
            address == jump_table_ic
        &&
            jump_table_ic < proc_attr_table_offset - 8
        )
        {
            report.eoln();
            report.full->fprintf("; Procedure %d Jump Table:", proc_number);
        }
    }
    else
    {
        unsigned n = get_word(data + address);
        unsigned short dstadr = address - n;
        report.data_column->fprintf("%02X ", data[address++]);
        report.data_column->fprintf("%02X", data[address++]);
        report.opcode_column->fputs(".word");
        rcstring dst = where.get_name(dstadr);
        if (dst.empty())
        {
            report.argument_column->fprintf("%u", n);
        }
        else
        {
            report.argument_column->fputs("$ - ");
            report.argument_column->fputs(dst);
        }
    }
    return (address - start_address);
}


// vim: set ts=8 sw=4 et :
