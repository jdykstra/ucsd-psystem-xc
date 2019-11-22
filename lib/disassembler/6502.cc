//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
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

#include <lib/debug.h>
#include <lib/disassembler/6502.h>


disassembler_6502::~disassembler_6502()
{
}


disassembler_6502::disassembler_6502(
    dislabel &a_where,
    discolumns &a_report,
    int a_segnum,
    int a_proc_num,
    int a_proc_base,
    code_source_map *a_listing
) :
    disassembler(little_endian, a_where, a_report, a_segnum, a_proc_num,
        a_proc_base, a_listing),
    highwater(0),
    all_ascii(false)
{
}


disassembler_6502::pointer
disassembler_6502::create(dislabel &a_where, discolumns &a_report, int a_segnum,
    int a_proc_num, int a_proc_base, code_source_map *a_list)
{
    return
        pointer
        (
            new disassembler_6502
            (
                a_where,
                a_report,
                a_segnum,
                a_proc_num,
                a_proc_base,
                a_list
            )
        );
}


enum opcode_mode_t
{
    opcode_mode_absolute,
    opcode_mode_absolute_x,
    opcode_mode_abs_x_ind,
    opcode_mode_absolute_y,
    opcode_mode_accumulator,
    opcode_mode_immediate,
    opcode_mode_implicit,
    opcode_mode_indirect,
    opcode_mode_indirect_zp, // 65C02
    opcode_mode_zp_x_ind,
    opcode_mode_indirect_y,
    opcode_mode_undefined,
    opcode_mode_zero_page,
    opcode_mode_zero_page_x,
    opcode_mode_zero_page_y,
    opcode_mode_branch,
    opcode_mode_rockwell2, // 65C02s WDC
    opcode_mode_rockwell2z, // 65C02s WDC
    opcode_mode_rockwell3 // 65C02s WDC
};


struct table_t
{
    const char *name;
    opcode_mode_t mode;
    unsigned char length;
};


static const table_t table[] =
{
    { "BRK", opcode_mode_implicit,    1 }, // 00
    { "ORA", opcode_mode_zp_x_ind,    2 }, // 01
    { 0,     opcode_mode_undefined,   0 }, // 02
    { 0,     opcode_mode_undefined,   0 }, // 03
    { "TSB", opcode_mode_zero_page,   2 }, // 04 (65C02)
    { "ORA", opcode_mode_zero_page,   2 }, // 05
    { "ASL", opcode_mode_zero_page,   2 }, // 06
    { "RMB", opcode_mode_rockwell2z,  2 }, // 07 (65C02s)
    { "PHP", opcode_mode_implicit,    1 }, // 08
    { "ORA", opcode_mode_immediate,   2 }, // 09
    { "ASL", opcode_mode_accumulator, 1 }, // 0A
    { 0,     opcode_mode_undefined,   0 }, // 0B
    { "TSB", opcode_mode_absolute,    3 }, // 0C (65C02)
    { "ORA", opcode_mode_absolute,    3 }, // 0D
    { "ASL", opcode_mode_absolute,    3 }, // 0E
    { "BBR", opcode_mode_rockwell3,   3 }, // 0F (65C02s)
    { "BPL", opcode_mode_branch,      2 }, // 10
    { "ORA", opcode_mode_indirect_y,  2 }, // 11
    { "ORA", opcode_mode_indirect_zp, 2 }, // 12 (65C02)
    { 0,     opcode_mode_undefined,   0 }, // 13
    { "TRB", opcode_mode_zero_page,   2 }, // 14 (65C02)
    { "ORA", opcode_mode_zero_page_x, 2 }, // 15
    { "ASL", opcode_mode_zero_page_x, 2 }, // 16
    { "RMB", opcode_mode_rockwell2z,  2 }, // 17 (65C02s)
    { "CLC", opcode_mode_implicit,    1 }, // 18
    { "ORA", opcode_mode_absolute_y,  3 }, // 19
    { "INC", opcode_mode_accumulator, 1 }, // 1A (65C02)  aka "ina"
    { 0,     opcode_mode_undefined,   0 }, // 1B
    { "TRB", opcode_mode_absolute,    3 }, // 1C (65C02)
    { "ORA", opcode_mode_absolute_x,  3 }, // 1D
    { "ASL", opcode_mode_absolute_x,  3 }, // 1E
    { "BBR", opcode_mode_rockwell3,   3 }, // 1F (65C02s)
    { "JSR", opcode_mode_absolute,    3 }, // 20
    { "AND", opcode_mode_zp_x_ind,    2 }, // 21
    { 0,     opcode_mode_undefined,   0 }, // 22
    { 0,     opcode_mode_undefined,   0 }, // 23
    { "BIT", opcode_mode_zero_page,   2 }, // 24
    { "AND", opcode_mode_zero_page,   2 }, // 25
    { "ROL", opcode_mode_zero_page,   2 }, // 26
    { "RMB", opcode_mode_rockwell2z,  2 }, // 27 (65C02s)
    { "PLP", opcode_mode_implicit,    1 }, // 28
    { "AND", opcode_mode_immediate,   2 }, // 29
    { "ROL", opcode_mode_accumulator, 1 }, // 2A
    { 0,     opcode_mode_undefined,   0 }, // 2B
    { "BIT", opcode_mode_absolute,    3 }, // 2C
    { "AND", opcode_mode_absolute,    3 }, // 2D
    { "ROL", opcode_mode_absolute,    3 }, // 2E
    { "BBR", opcode_mode_rockwell3,   3 }, // 2F (65C02s)
    { "BMI", opcode_mode_branch,      2 }, // 30
    { "AND", opcode_mode_indirect_y,  2 }, // 31
    { "AND", opcode_mode_indirect_zp, 2 }, // 32 (65C02)
    { 0,     opcode_mode_undefined,   0 }, // 33
    { "BIT", opcode_mode_zero_page_x, 2 }, // 34 (65C02)
    { "AND", opcode_mode_zero_page_x, 2 }, // 35
    { "ROL", opcode_mode_zero_page_x, 2 }, // 36
    { "RMB", opcode_mode_rockwell2z,  2 }, // 37 (65C02s)
    { "SEC", opcode_mode_implicit,    1 }, // 38
    { "AND", opcode_mode_absolute_y,  3 }, // 39
    { "DEC", opcode_mode_accumulator, 1 }, // 3A (65C02)  aka "dea"
    { 0,     opcode_mode_undefined,   0 }, // 3B
    { "BIT", opcode_mode_absolute_x,  3 }, // 3C (65C02)
    { "AND", opcode_mode_absolute_x,  3 }, // 3D
    { "ROL", opcode_mode_absolute_x,  3 }, // 3E
    { "BBR", opcode_mode_rockwell3,   3 }, // 3F (65C02s)
    { "RTI", opcode_mode_implicit,    1 }, // 40
    { "EOR", opcode_mode_zp_x_ind,    2 }, // 41
    { 0,     opcode_mode_undefined,   0 }, // 42
    { 0,     opcode_mode_undefined,   0 }, // 43
    { 0,     opcode_mode_undefined,   0 }, // 44
    { "EOR", opcode_mode_zero_page,   2 }, // 45
    { "LSR", opcode_mode_zero_page,   2 }, // 46
    { "RMB", opcode_mode_rockwell2z,  2 }, // 47 (65C02s)
    { "PHA", opcode_mode_implicit,    1 }, // 48
    { "EOR", opcode_mode_immediate,   2 }, // 49
    { "LSR", opcode_mode_accumulator, 1 }, // 4A
    { 0,     opcode_mode_undefined,   0 }, // 4B
    { "JMP", opcode_mode_absolute,    3 }, // 4C
    { "EOR", opcode_mode_absolute,    3 }, // 4D
    { "LSR", opcode_mode_absolute,    3 }, // 4E
    { "BBR", opcode_mode_rockwell3,   3 }, // 4F (65C02s)
    { "BVC", opcode_mode_branch,      2 }, // 50
    { "EOR", opcode_mode_indirect_y,  2 }, // 51
    { "EOR", opcode_mode_indirect_zp, 2 }, // 52 (65C02)
    { 0,     opcode_mode_undefined,   0 }, // 53
    { 0,     opcode_mode_undefined,   0 }, // 54
    { "EOR", opcode_mode_zero_page_x, 2 }, // 55
    { "LSR", opcode_mode_zero_page_x, 2 }, // 56
    { "RMB", opcode_mode_rockwell2z,  2 }, // 57 (65C02s)
    { "CLI", opcode_mode_implicit,    1 }, // 58
    { "EOR", opcode_mode_absolute_y,  3 }, // 59
    { "PHY", opcode_mode_implicit,    1 }, // 5A (65C02)
    { 0,     opcode_mode_undefined,   0 }, // 5B
    { 0,     opcode_mode_undefined,   0 }, // 5C
    { "EOR", opcode_mode_absolute_x,  3 }, // 5D
    { "LSR", opcode_mode_absolute_x,  3 }, // 5E
    { "BBR", opcode_mode_rockwell3,   3 }, // 5F (65C02s)
    { "RTS", opcode_mode_implicit,    1 }, // 60
    { "ADC", opcode_mode_zp_x_ind,    2 }, // 61
    { 0,     opcode_mode_undefined,   0 }, // 62
    { 0,     opcode_mode_undefined,   0 }, // 63
    { "STZ", opcode_mode_zero_page,   2 }, // 64 (65C02)
    { "ADC", opcode_mode_zero_page,   2 }, // 65
    { "ROR", opcode_mode_zero_page,   2 }, // 66
    { "RMB", opcode_mode_rockwell2z,  2 }, // 67 (65C02s)
    { "PLA", opcode_mode_implicit,    1 }, // 68
    { "ADC", opcode_mode_immediate,   2 }, // 69
    { "ROR", opcode_mode_accumulator, 1 }, // 6A
    { 0,     opcode_mode_undefined,   0 }, // 6B
    { "JMP", opcode_mode_indirect,    3 }, // 6C
    { "ADC", opcode_mode_absolute,    3 }, // 6D
    { "ROR", opcode_mode_absolute,    3 }, // 6E
    { "BBR", opcode_mode_rockwell3,   3 }, // 6F (65C02s)
    { "BVS", opcode_mode_branch,      2 }, // 70
    { "ADC", opcode_mode_indirect_y,  2 }, // 71
    { "ADC", opcode_mode_indirect_zp, 2 }, // 72 (65C02)
    { 0,     opcode_mode_undefined,   0 }, // 73
    { "STZ", opcode_mode_zero_page_x, 2 }, // 74 (65C02)
    { "ADC", opcode_mode_zero_page_x, 2 }, // 75
    { "ROR", opcode_mode_zero_page_x, 2 }, // 76
    { "RMB", opcode_mode_rockwell2z,  2 }, // 77 (65C02s)
    { "SEI", opcode_mode_implicit,    1 }, // 78
    { "ADC", opcode_mode_absolute_y,  3 }, // 79
    { "PLY", opcode_mode_implicit,    1 }, // 7A (65C02)
    { 0,     opcode_mode_undefined,   0 }, // 7B
    { "JMP", opcode_mode_abs_x_ind,   3 }, // 7C (65C02)
    { "ADC", opcode_mode_absolute_x,  3 }, // 7D
    { "ROR", opcode_mode_absolute_x,  3 }, // 7E
    { "BBR", opcode_mode_rockwell3,   3 }, // 7F (65C02s)
    { "BRA", opcode_mode_branch,      2 }, // 80 (65C02)
    { "STA", opcode_mode_zp_x_ind,    2 }, // 81
    { 0,     opcode_mode_undefined,   0 }, // 82
    { 0,     opcode_mode_undefined,   0 }, // 83
    { "STY", opcode_mode_zero_page,   2 }, // 84
    { "STA", opcode_mode_zero_page,   2 }, // 85
    { "STX", opcode_mode_zero_page,   2 }, // 86
    { "SMB", opcode_mode_rockwell2z,  2 }, // 87 (65C02s)
    { "DEY", opcode_mode_implicit,    1 }, // 88
    { "BIT", opcode_mode_immediate,   2 }, // 89 (65C02)
    { "TXA", opcode_mode_implicit,    1 }, // 8A
    { 0,     opcode_mode_undefined,   0 }, // 8B
    { "STY", opcode_mode_absolute,    3 }, // 8C
    { "STA", opcode_mode_absolute,    3 }, // 8D
    { "STX", opcode_mode_absolute,    3 }, // 8E
    { "BBS", opcode_mode_rockwell3,   3 }, // 8F (65C02s)
    { "BCC", opcode_mode_branch,      2 }, // 90
    { "STA", opcode_mode_indirect_y,  2 }, // 91
    { "STA", opcode_mode_indirect_zp, 2 }, // 92 (65C02)
    { 0,     opcode_mode_undefined,   0 }, // 93
    { "STY", opcode_mode_zero_page_x, 2 }, // 94
    { "STA", opcode_mode_zero_page_x, 2 }, // 95
    { "STX", opcode_mode_zero_page_y, 2 }, // 96
    { "SMB", opcode_mode_rockwell2z,  2 }, // 97 (65C02s)
    { "TYA", opcode_mode_implicit,    1 }, // 98
    { "STA", opcode_mode_absolute_y,  3 }, // 99
    { "TXS", opcode_mode_implicit,    1 }, // 9A
    { 0,     opcode_mode_undefined,   0 }, // 9B
    { "STZ", opcode_mode_absolute,    3 }, // 9C (65C02)
    { "STA", opcode_mode_absolute_x,  3 }, // 9D
    { "STZ", opcode_mode_absolute_x,  3 }, // 9E (65C02)
    { "BBS", opcode_mode_rockwell3,   3 }, // 9F (65C02s)
    { "LDY", opcode_mode_immediate,   2 }, // A0
    { "LDA", opcode_mode_zp_x_ind,    2 }, // A1
    { "LDX", opcode_mode_immediate,   2 }, // A2
    { 0,     opcode_mode_undefined,   0 }, // A3
    { "LDY", opcode_mode_zero_page,   2 }, // A4
    { "LDA", opcode_mode_zero_page,   2 }, // A5
    { "LDX", opcode_mode_zero_page,   2 }, // A6
    { "SMB", opcode_mode_rockwell2z,  2 }, // A7 (65C02s)
    { "TAY", opcode_mode_implicit,    1 }, // A8
    { "LDA", opcode_mode_immediate,   2 }, // A9
    { "TAX", opcode_mode_implicit,    1 }, // AA
    { 0,     opcode_mode_undefined,   0 }, // AB
    { "LDY", opcode_mode_absolute,    3 }, // AC
    { "LDA", opcode_mode_absolute,    3 }, // AD
    { "LDX", opcode_mode_absolute,    3 }, // AE
    { "BBS", opcode_mode_rockwell3,   3 }, // AF (65C02s)
    { "BCS", opcode_mode_branch,      2 }, // B0
    { "LDA", opcode_mode_indirect_y,  2 }, // B1
    { "LDA", opcode_mode_indirect_zp, 2 }, // B2 (65C02)
    { 0,     opcode_mode_undefined,   0 }, // B3
    { "LDY", opcode_mode_zero_page_x, 2 }, // B4
    { "LDA", opcode_mode_zero_page_x, 2 }, // B5
    { "LDX", opcode_mode_zero_page_y, 2 }, // B6
    { "SMB", opcode_mode_rockwell2z,  2 }, // B7 (65C02s)
    { "CLV", opcode_mode_implicit,    1 }, // B8
    { "LDA", opcode_mode_absolute_y,  3 }, // B9
    { "TXS", opcode_mode_implicit,    1 }, // BA
    { 0,     opcode_mode_undefined,   0 }, // BB
    { "LDY", opcode_mode_absolute_x,  3 }, // BC
    { "LDA", opcode_mode_absolute_x,  3 }, // BD
    { "LDX", opcode_mode_absolute_y,  3 }, // BE
    { "BBS", opcode_mode_rockwell3,   3 }, // BF (65C02s)
    { "CPY", opcode_mode_immediate,   2 }, // C0
    { "CMP", opcode_mode_zp_x_ind,    2 }, // C1
    { 0,     opcode_mode_undefined,   0 }, // C2
    { 0,     opcode_mode_undefined,   0 }, // C3
    { "CPY", opcode_mode_zero_page,   2 }, // C4
    { "CMP", opcode_mode_zero_page,   2 }, // C5
    { "DEC", opcode_mode_zero_page,   2 }, // C6
    { "SMB", opcode_mode_rockwell2z,  2 }, // C7 (65C02s)
    { "INY", opcode_mode_implicit,    1 }, // C8
    { "CMP", opcode_mode_immediate,   2 }, // C9
    { "DEX", opcode_mode_implicit,    1 }, // CA
    { "WAI", opcode_mode_implicit,    1 }, // CB (65C02s WDC)
    { "CPY", opcode_mode_absolute,    3 }, // CC
    { "CMP", opcode_mode_absolute,    3 }, // CD
    { "DEC", opcode_mode_absolute,    3 }, // CE
    { "BBS", opcode_mode_rockwell3,   3 }, // CF (65C02s)
    { "BNE", opcode_mode_branch,      2 }, // D0
    { "CMP", opcode_mode_indirect_y,  2 }, // D1
    { "CMP", opcode_mode_indirect_zp, 2 }, // D2 (65C02)
    { 0,     opcode_mode_undefined,   0 }, // D3
    { 0,     opcode_mode_undefined,   0 }, // D4
    { "CMP", opcode_mode_zero_page_x, 2 }, // D5
    { "DEC", opcode_mode_zero_page_x, 2 }, // D6
    { "SMB", opcode_mode_rockwell2z,  2 }, // D7 (65C02s)
    { "CLD", opcode_mode_implicit,    1 }, // D8
    { "CMP", opcode_mode_absolute_y,  3 }, // D9
    { "PHX", opcode_mode_implicit,    1 }, // DA (65C02)
    { "STP", opcode_mode_implicit,    1 }, // DB (65C02s WDC)
    { 0,     opcode_mode_undefined,   0 }, // DC
    { "CMP", opcode_mode_absolute_x,  3 }, // DD
    { "DEC", opcode_mode_absolute_x,  3 }, // DE
    { "BBS", opcode_mode_rockwell3,   3 }, // DF (65C02s)
    { "CPX", opcode_mode_immediate,   2 }, // E0
    { "SBC", opcode_mode_zp_x_ind,    2 }, // E1
    { 0,     opcode_mode_undefined,   0 }, // E2
    { 0,     opcode_mode_undefined,   0 }, // E3
    { "CPX", opcode_mode_zero_page,   2 }, // E4
    { "SBC", opcode_mode_zero_page,   2 }, // E5
    { "INC", opcode_mode_zero_page,   2 }, // E6
    { "SMB", opcode_mode_rockwell2z,  2 }, // E7 (65C02s)
    { "INX", opcode_mode_implicit,    1 }, // E8
    { "SBC", opcode_mode_immediate,   2 }, // E9
    { "NOP", opcode_mode_implicit,    1 }, // EA
    { 0,     opcode_mode_undefined,   0 }, // EB
    { "CPX", opcode_mode_absolute,    3 }, // EC
    { "SBC", opcode_mode_absolute,    3 }, // ED
    { "INC", opcode_mode_absolute,    3 }, // EE
    { "BBS", opcode_mode_rockwell3,   3 }, // EF (65C02s)
    { "BEQ", opcode_mode_branch,      2 }, // F0
    { "SBC", opcode_mode_indirect_y,  2 }, // F1
    { "SBC", opcode_mode_indirect_zp, 2 }, // F2 (65C02)
    { 0,     opcode_mode_undefined,   0 }, // F3
    { 0,     opcode_mode_undefined,   0 }, // F4
    { "SBC", opcode_mode_zero_page_x, 2 }, // F5
    { "INC", opcode_mode_zero_page_x, 2 }, // F6
    { "SMB", opcode_mode_rockwell2z,  2 }, // F7 (65C02s)
    { "SED", opcode_mode_implicit,    1 }, // F8
    { "SBC", opcode_mode_absolute_y,  3 }, // F9
    { "PLX", opcode_mode_implicit,    1 }, // FA (65C02)
    { 0,     opcode_mode_undefined,   0 }, // FB
    { 0,     opcode_mode_undefined,   0 }, // FC
    { "SBC", opcode_mode_absolute_x,  3 }, // FD
    { "INC", opcode_mode_absolute_x,  3 }, // FE
    { "BBS", opcode_mode_rockwell3,   3 }, // FF (65C02s)
};


void
disassembler_6502::preprocess(const unsigned char *data, unsigned entry_ic,
    unsigned, size_t data_size)
{
    DEBUG(2, "data_size = %04X", (int)data_size);
    if (data_size < 6)
        highwater = 0;
    else
        highwater = data_size - 6;
    DEBUG(2, "highwater = %04X", (int)highwater);
    all_ascii = true;
    for (unsigned address = entry_ic; address < highwater; )
    {
        unsigned char c = data[address++];
        if (c == 0x60)
        {
            // The RTS opcode is ambiguous.  It could be an empty procedure,
            // or it could be a '`' character.  Look at the next one to decide.
            if (address >= highwater || !isgraph(data[address]))
            {
                all_ascii = false;
                break;
            }
        }
        else if (c != 0 && !isprint(c))
        {
            all_ascii = false;
            break;
        }
    }
    if (all_ascii)
        return;
    for (unsigned address = entry_ic; address < highwater; )
    {
        assert(address < data_size);
        unsigned char c = data[address++];
        const table_t *tp = &table[c];
        switch (tp->mode)
        {
        case opcode_mode_undefined:
            assert(tp->length == 0);
            break;

        case opcode_mode_implicit:
            assert(tp->length == 1);
            break;

        case opcode_mode_accumulator:
            assert(tp->length == 1);
            break;

        case opcode_mode_branch:
        case opcode_mode_rockwell2:
            {
               assert(tp->length == 2);
               int offset = data[address++];
               if (offset & 0x80)
                   offset -= 256;
                where.add(address + offset);
            }
            break;

        case opcode_mode_rockwell2z:
            ++address;
            break;

        case opcode_mode_immediate:
        case opcode_mode_zero_page:
        case opcode_mode_zero_page_x:
        case opcode_mode_zero_page_y:
        case opcode_mode_indirect_zp:
            assert(tp->length == 2);
            ++address;
            break;

        case opcode_mode_zp_x_ind:
        case opcode_mode_indirect_y:
            assert(tp->length == 2);
            ++address;
            break;

        case opcode_mode_abs_x_ind:
            assert(tp->length == 3);
            address += 2;
            break;

        case opcode_mode_absolute:
        case opcode_mode_indirect:
        case opcode_mode_absolute_x:
        case opcode_mode_absolute_y:
            assert(tp->length == 3);
            address += 2;
            break;

        case opcode_mode_rockwell3:
            {
               assert(tp->length == 3);
               ++address;
               int offset = data[address++];
               if (offset & 0x80)
                   offset -= 256;
                where.add(address + offset);
            }
            break;
        }
    }
}


unsigned
disassembler_6502::disassemble_one_instruction(const unsigned char *data,
    unsigned address, size_t data_size, int)
{
    assert(address < data_size);
    unsigned start_address = address;
    if (all_ascii)
    {
        unsigned char c = data[address++];
        if (!isprint(c))
        {
            report.data_column->fprintf("%02X", c);
            report.opcode_column->fputs(".byte");
            report.argument_column->fprintf("%d", c);
            return 1;
        }
        report.opcode_column->fputs(".ascii");
        report.argument_column->fputc('\'');
        for (;;)
        {
            report.data_column->fprintf("%02X", c);
            report.argument_column->fputc(c);
            if (address >= highwater)
                break;
            if (address >= start_address + 32)
                break;
            c = data[address];
            if (c == 0 || !isprint(c))
                break;
            ++address;
            report.data_column->fputc(' ');
        }
        report.argument_column->fputc('\'');
        return (address - start_address);
    }
    unsigned char c = data[address++];
    const table_t *tp = &table[c];
    if (!tp->name)
        return 0;
    report.data_column->fprintf("%02X", c);
    report.opcode_column->fputs(tp->name);
    switch (tp->mode)
    {
    case opcode_mode_undefined:
        assert(tp->length == 0);
        return 0;

    case opcode_mode_implicit:
        assert(tp->length == 1);
        break;

    case opcode_mode_branch:
        {
            assert(tp->length == 2);
            int offset = data[address++];
            report.data_column->fprintf(" %02X", offset);
            if (offset & 0x80)
                offset -= 256;
            rcstring name = where.get_name(address + offset);
            if (name.empty())
                report.argument_column->fprintf("$+%d", offset + 2);
            else
                report.argument_column->fputs(name);
        }
        break;

    case opcode_mode_accumulator:
        assert(tp->length == 1);
        report.argument_column->fputs("A");
        break;

    case opcode_mode_immediate:
        assert(tp->length == 2);
        c = data[address++];
        report.data_column->fprintf(" %02X", c);
        report.argument_column->fprintf("#%d", c);
        break;

    case opcode_mode_zero_page:
        assert(tp->length == 2);
        c = data[address++];
        report.data_column->fprintf(" %02X", c);
        report.argument_column->fprintf("%d", c);
        break;

    case opcode_mode_zero_page_x:
        assert(tp->length == 2);
        c = data[address++];
        report.data_column->fprintf(" %02X", c);
        report.argument_column->fprintf("%d, X", c);
        break;

    case opcode_mode_zero_page_y:
        assert(tp->length == 2);
        c = data[address++];
        report.data_column->fprintf(" %02X", c);
        report.argument_column->fprintf("%d, Y", c);
        break;

    case opcode_mode_absolute:
        {
            assert(tp->length == 3);
            unsigned n = data[address] | (data[address + 1] << 8);
            report.data_column->fprintf(" %02X", data[address]);
            report.data_column->fprintf(" %02X", data[address + 1]);
            address += 2;
            report.argument_column->fprintf("%u", n);
        }
        break;

    case opcode_mode_indirect_zp:
        {
            assert(tp->length == 2);
            unsigned char n = data[address++];
            report.data_column->fprintf(" %02X", n);
            report.argument_column->fprintf("(%u)", n);
        }
        break;

    case opcode_mode_indirect:
        {
            assert(tp->length == 3);
            unsigned n = data[address] | (data[address + 1] << 8);
            report.data_column->fprintf(" %02X", data[address]);
            report.data_column->fprintf(" %02X", data[address + 1]);
            address += 2;
            report.argument_column->fprintf("(%u)", n);
        }
        break;

    case opcode_mode_absolute_x:
        {
            assert(tp->length == 3);
            unsigned n = data[address] | (data[address + 1] << 8);
            report.data_column->fprintf(" %02X", data[address]);
            report.data_column->fprintf(" %02X", data[address + 1]);
            address += 2;
            report.argument_column->fprintf("%u, X", n);
        }
        break;

    case opcode_mode_abs_x_ind:
        {
            assert(tp->length == 3);
            unsigned n = data[address] | (data[address + 1] << 8);
            report.data_column->fprintf(" %02X", data[address]);
            report.data_column->fprintf(" %02X", data[address + 1]);
            address += 2;
            report.argument_column->fprintf("(%u, X)", n);
        }
        break;

    case opcode_mode_absolute_y:
        {
            assert(tp->length == 3);
            unsigned n = data[address] | (data[address + 1] << 8);
            report.data_column->fprintf(" %02X", data[address]);
            report.data_column->fprintf(" %02X", data[address + 1]);
            address += 2;
            report.argument_column->fprintf("%u, Y", n);
        }
        break;

    case opcode_mode_zp_x_ind:
        assert(tp->length == 2);
        c = data[address++];
        report.data_column->fprintf(" %02X", c);
        report.argument_column->fprintf("(%d, X)", c);
        break;

    case opcode_mode_indirect_y:
        assert(tp->length == 2);
        c = data[address++];
        report.data_column->fprintf(" %02X", c);
        report.argument_column->fprintf("(%d), Y", c);
        break;

    case opcode_mode_rockwell2:
        {
            assert(tp->length == 2);
            report.argument_column->fprintf("%d, ", (c >> 4) & 3);

            int offset = data[address++];
            report.data_column->fprintf(" %02X", offset);
            if (offset & 0x80)
                offset -= 256;
            rcstring name = where.get_name(address + offset);
            if (name.empty())
                report.argument_column->fprintf("$+%d", offset + 2);
            else
                report.argument_column->fputs(name);
        }
        break;

    case opcode_mode_rockwell2z:
        {
            assert(tp->length == 2);
            report.argument_column->fprintf("%d, ", (c >> 4) & 3);

            unsigned char offset = data[address++];
            report.data_column->fprintf(" %02X", offset);
            report.argument_column->fprintf("%d", offset);
        }
        break;

    case opcode_mode_rockwell3:
        {
            assert(tp->length == 3);
            report.argument_column->fprintf("%d, ", (c >> 4) & 3);

            unsigned char addr = data[address++];
            report.data_column->fprintf(" %02X", addr);
            report.argument_column->fprintf("%u, ", addr);

            int offset = data[address++];
            report.data_column->fprintf(" %02X", offset);
            if (offset & 0x80)
                offset -= 256;
            rcstring name = where.get_name(address + offset);
            if (name.empty())
                report.argument_column->fprintf("$+%d", offset + 2);
            else
                report.argument_column->fputs(name);
        }
        break;
    }
    return (address - start_address);
}


bool
disassembler_6502::is_native(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
