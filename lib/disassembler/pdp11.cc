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

#include <lib/disassembler/pdp11.h>
#include <lib/sizeof.h>


disassembler_pdp11::~disassembler_pdp11()
{
}


disassembler_pdp11::disassembler_pdp11(
    dislabel &a_where,
    discolumns &a_report,
    int a_segnum,
    int a_proc_num,
    int a_proc_base,
    code_source_map *a_listing
) :
    disassembler(little_endian, a_where, a_report, a_segnum, a_proc_num,
        a_proc_base, a_listing),
    highwater(0)
{
}


disassembler_pdp11::pointer
disassembler_pdp11::create(dislabel &a_where, discolumns &a_report,
    int a_segnum, int a_proc_num, int a_proc_base, code_source_map *a_list)
{
    return
        pointer
        (
            new disassembler_pdp11
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
    mode_branch,
    chgflg,
    decbra,
    fop1,
    fop2a,
    fop2b,
    fop2c,
    fop2d,
    mode_undefined,
    muldiv,
    op0,     // no registers
    op0_5,   // half operand (one register)
    op1,     // single operand
    op1_5,   // register and operand
    op2,     // two operands
    setpri,
    trpemt,
};

struct table_t
{
    unsigned short mask;
    unsigned short opcode;
    const char *name;
    bool bflag :1;
    opcode_mode_t mode :7;
    unsigned short model;
};

enum
{
    model_falcon =       0x0001,
    model_lsi11 =        0x0002,
    model_lsi11_2 =      0x0004,
    model_lsi11_23 =     0x0008,
    model_micro_j11 =    0x0010,
    model_micro_pdp11 =  0x0020,
    model_micro_t11 =    0x0040,
    model_pdp11_23plus = 0x0080,
    model_pdp11_24 =     0x0100,
    model_pdp11_35 =     0x0200,
    model_pdp11_40 =     0x0400,
    model_pdp11_44 =     0x0800,
    model_pdp11_45 =     0x1000,
    model_pdp11_70 =     0x2000,
    model_vax11 =        0x8000,
    model_all =          0xFFFF,
    model_all_except_vax11 = model_all - model_vax11,
};


static const table_t table[] =
{
    // mask,   opcode,  name,     bflag, mode,   model
    // ------  -------  --------  -----  ------  -------
    { 0000000, 0000000, "halt",   false, op0,    model_all_except_vax11 },
    { 0000000, 0000001, "wait",   false, op0,    model_all_except_vax11 },
    { 0000000, 0000002, "rti",    false, op0,    model_all },
    { 0000000, 0000003, "bpt",    false, op0,    model_all_except_vax11 },
    { 0000000, 0000004, "iot",    false, op0,    model_all_except_vax11 },
    { 0000000, 0000005, "rst",    false, op0,    model_all_except_vax11 },
    { 0000000, 0000005, "reset",  false, op0,    model_all_except_vax11 },
    { 0000000, 0000006, "rtt",    false, op0,    model_all },
    { 0000000, 0000007, "mfpt",   false, op0,
        model_micro_t11 | model_micro_j11 | model_lsi11_2 | model_falcon |
        model_lsi11_23 | model_micro_pdp11 | model_pdp11_23plus |
        model_pdp11_24 },
    { 0000077, 0000100, "jmp",    false, op1,    model_all },
    { 0000007, 0000200, "rts",    false, op0_5,  model_all },
    { 0000007, 0000230, "spl",    false, setpri,
        model_micro_j11 | model_pdp11_44 },
    { 0000000, 0000240, "nop",    false, op0,    model_all },
    { 0000000, 0000257, "ccc",    false, op0,    model_all },
    { 0000017, 0000240, "cl",     false, chgflg, model_all },
    { 0000000, 0000277, "scc",    false, op0,    model_all },
    { 0000017, 0000260, "se",     false, chgflg, model_all },
    { 0000077, 0000300, "swab",   false, op1,    model_all },
    { 0000377, 0000400, "br",     false, mode_branch, model_all },
    { 0000377, 0001000, "bne",    false, mode_branch, model_all },
    { 0000377, 0001400, "beq",    false, mode_branch, model_all },
    { 0000377, 0002000, "bge",    false, mode_branch, model_all },
    { 0000377, 0002400, "blt",    false, mode_branch, model_all },
    { 0000377, 0003000, "bgt",    false, mode_branch, model_all },
    { 0000377, 0003400, "ble",    false, mode_branch, model_all },
    { 0000777, 0004000, "jsr",    false, op1_5, model_all },
    { 0100077, 0005000, "clr",    true,  op1, model_all },
    { 0100077, 0005100, "com",    true,  op1, model_all },
    { 0100077, 0005200, "inc",    true,  op1, model_all },
    { 0100077, 0005300, "dec",    true,  op1, model_all },
    { 0100077, 0005400, "neg",    true,  op1, model_all },
    { 0100077, 0005500, "adc",    true,  op1, model_all },
    { 0100077, 0005600, "sbc",    true,  op1, model_all },
    { 0100077, 0005700, "tst",    true,  op1, model_all },
    { 0100077, 0006000, "ror",    true,  op1, model_all },
    { 0100077, 0006100, "rol",    true,  op1, model_all },
    { 0100077, 0006200, "asr",    true,  op1, model_all },
    { 0100077, 0006300, "asl",    true,  op1, model_all },
    { 0000077, 0006400, "mark",   false, op1,
        model_all - model_falcon - model_vax11 },
    { 0000077, 0006500, "mfpi",   false, op1,
        model_micro_j11 | model_lsi11_23 | model_micro_pdp11 |
        model_pdp11_23plus | model_pdp11_24 | model_pdp11_44 | model_vax11 },
    { 0000077, 0006600, "mtpi",   false, op1,
        model_micro_j11 | model_lsi11_23 | model_micro_pdp11 |
        model_pdp11_23plus | model_pdp11_24 | model_pdp11_44 | model_vax11 },
    { 0000077, 0006700, "sxt",    false, op1, model_all },
    { 0000077, 0007000, "csm",    false, op1, model_pdp11_45 | model_pdp11_70 },
    { 0000077, 0007200, "tstset", false, op1,   model_micro_j11 },
    { 0000077, 0007300, "wrtlck", false, op1,   model_micro_j11 },
    { 0107777, 0010000, "mov",    true,  op2, model_all },
    { 0107777, 0020000, "cmp",    true,  op2, model_all },
    { 0107777, 0030000, "bit",    true,  op2, model_all },
    { 0107777, 0040000, "bic",    true,  op2, model_all },
    { 0107777, 0050000, "bis",    true,  op2, model_all },
    { 0007777, 0060000, "add",    false, op2, model_all },
    { 0000777, 0070000, "mul",    false, muldiv,
        model_micro_pdp11 | model_pdp11_23plus | model_pdp11_24 |
        model_pdp11_44 | model_vax11 | model_lsi11_2 | model_lsi11 |
        model_pdp11_35 | model_pdp11_40 },
    { 0000777, 0071000, "div",    false, muldiv,
        model_micro_pdp11 | model_pdp11_23plus | model_pdp11_24 |
        model_pdp11_44 | model_vax11 | model_lsi11_2 | model_lsi11 |
        model_pdp11_35 | model_pdp11_40 },
    { 0000777, 0072000, "ash",    false, muldiv,
        model_micro_pdp11 | model_pdp11_23plus | model_pdp11_24 |
        model_pdp11_44 | model_vax11 | model_lsi11_2 | model_lsi11 |
        model_pdp11_35 | model_pdp11_40 },
    { 0000777, 0073000, "ashc",   false, muldiv,
        model_micro_pdp11 | model_pdp11_23plus | model_pdp11_24 |
        model_pdp11_44 | model_vax11 | model_lsi11_2 | model_lsi11 |
        model_pdp11_35 | model_pdp11_40 },
    { 0000777, 0074000, "xor",    false, op1_5, model_all },
    { 0000007, 0075000, "fadd",   false, op0_5,
        model_lsi11 | model_pdp11_35 | model_pdp11_40 },
    { 0000007, 0075010, "fsub",   false, op0_5,
        model_lsi11 | model_pdp11_35 | model_pdp11_40 },
    { 0000007, 0075020, "fmul",   false, op0_5,
        model_lsi11 | model_pdp11_35 | model_pdp11_40 },
    { 0000007, 0075030, "fdiv",   false, op0_5,
        model_lsi11 | model_pdp11_35 | model_pdp11_40 },
    { 0000777, 0077000, "sob",    false, decbra,      model_all },
    { 0000377, 0100000, "bpl",    false, mode_branch, model_all },
    { 0000377, 0100400, "bmi",    false, mode_branch, model_all },
    { 0000377, 0101000, "bhi",    false, mode_branch, model_all },
    { 0000377, 0101400, "blos",   false, mode_branch, model_all },
    { 0000377, 0102000, "bvc",    false, mode_branch, model_all },
    { 0000377, 0102400, "bvs",    false, mode_branch, model_all },
    { 0000377, 0103000, "bcc",    false, mode_branch, model_all },
    { 0000077, 0103000, "bhis",   false, mode_branch, model_all },
    { 0000377, 0103400, "bcs",    false, mode_branch, model_all },
    { 0000077, 0103400, "blo",    false, mode_branch, model_all },
    { 0000377, 0104000, "emt",    false, trpemt, model_all_except_vax11 },
    { 0000377, 0104400, "trap",   false, trpemt, model_all_except_vax11 },
    { 0000077, 0106400, "mtps",   false, op1,
        model_micro_t11 | model_micro_j11 | model_lsi11_2 | model_falcon |
        model_lsi11_23 | model_micro_pdp11 | model_pdp11_23plus |
        model_pdp11_24 },
    { 0000077, 0106500, "mfpd",   false, op1,
        model_micro_j11 | model_lsi11_23 | model_micro_pdp11 |
        model_pdp11_23plus | model_pdp11_24 | model_pdp11_44 | model_vax11 },
    { 0000077, 0106600, "mtpd",   false, op1,
        model_micro_j11 | model_lsi11_23 | model_micro_pdp11 |
        model_pdp11_23plus | model_pdp11_24 | model_pdp11_44 | model_vax11 },
    { 0000077, 0106700, "mfps",   false, op1,
        model_micro_t11 | model_micro_j11 | model_lsi11_2 | model_falcon |
        model_lsi11_23 | model_micro_pdp11 | model_pdp11_23plus |
        model_pdp11_24 },
    { 0007777, 0160000, "sub",    false, op2, model_all },
    { 0000000, 0170000, "cfcc",   false, op0, model_all },
    { 0000000, 0170001, "setf",   false, op0, model_all },
    { 0000000, 0170002, "seti",   false, op0, model_all },
    { 0000000, 0170011, "setd",   false, op0, model_all },
    { 0000000, 0170012, "setl",   false, op0, model_all },
    { 0000077, 0170100, "ldfps",  false, op1, model_all },
    { 0000077, 0170200, "stfps",  false, op1, model_all },
    { 0000077, 0170300, "stst",   false, op1, model_all },
    { 0000077, 0170400, "clrf",   false, fop1, model_all },
    { 0000077, 0170500, "tstf",   false, fop1, model_all },
    { 0000077, 0170600, "absf",   false, fop1, model_all },
    { 0000077, 0170700, "negf",   false, fop1, model_all },
    { 0000377, 0171000, "mulf",   false, fop2a, model_all },
    { 0000377, 0171400, "modf",   false, fop2a, model_all },
    { 0000377, 0172000, "addf",   false, fop2a, model_all },
    { 0000377, 0172400, "ldf",    false, fop2a, model_all },
    { 0000377, 0173000, "subf",   false, fop2a, model_all },
    { 0000377, 0173400, "cmpf",   false, fop2a, model_all },
    { 0000377, 0174000, "stf",    false, fop2b, model_all },
    { 0000377, 0174400, "divf",   false, fop2a, model_all },
    { 0000377, 0175000, "stexp",  false, fop2d, model_all },
    { 0000377, 0175400, "stci",   false, fop2d, model_all },
    { 0000377, 0176000, "stcd",   false, fop2b, model_all },
    { 0000377, 0176400, "ldexp",  false, fop2c, model_all },
    { 0000377, 0177000, "ldcif",  false, fop2c, model_all },
    { 0000377, 0177400, "ldcdf",  false, fop2a, model_all },
    { 0177777, 0000000, ".word",  false, mode_undefined, 0 },
};


static const table_t *
find(unsigned short inst)
{
    for (const table_t *tp = table; ; ++tp)
    {
        assert(tp < ENDOF(table));
        if ((inst & ~tp->mask) == tp->opcode)
            return tp;
    }
}


unsigned short
disassembler_pdp11::get_next_word(void)
{
    unsigned short value = get_word(memory + pc);
    pc += 2;
    return value;
}


void
disassembler_pdp11::preprocess(const unsigned char *data, unsigned entry_ic,
    unsigned, size_t data_size)
{
    memory = data;
    highwater = data_size - 6;
    pc = entry_ic;
    while (pc < highwater)
    {
        assert(pc < data_size);
        unsigned short inst = get_next_word();
        const table_t *tp = find(inst);
        switch (tp->mode)
        {
        case op0:
        case op0_5:
        case trpemt:
        case setpri:
        case chgflg:
        case mode_undefined:
            break;

        case op1:
        case op1_5:
        case muldiv:
        case fop2c:
        case fop2d:
            prep_proper(inst);
            break;

        case op2:
            // two operand
            prep_proper(inst >> 6);
            prep_proper(inst);
            break;

        case mode_branch:
            {
                // BRanch instruction
                unsigned short locn = inst & 0x00FF;
                if (locn & 0x0080)
                    locn |= 0xFF00;
                locn <<= 1;
                locn += pc;
                prep_prlab(locn);
            }
            break;

        case decbra:
            {
                // SOB instruction
                unsigned short locn = inst & 077;
                locn <<= 1;
                locn += pc;
                prep_prlab(locn);
            }
            break;

        case fop1:
        case fop2a:
        case fop2b:
            prep_pfoper(inst);
            break;
        }
    }
}


unsigned
disassembler_pdp11::disassemble_one_instruction(const unsigned char *data,
    unsigned address, size_t data_size, int)
{
    memory = data;
    assert(address < data_size);
    assert(!(address & 1));
    pc = address;
    unsigned short inst = get_next_word();
    const table_t *tp = find(inst);
    report.opcode_column->fputs(tp->name);
    if (tp->bflag && (inst & 0x8000))
        report.opcode_column->fputc('b');
    output::pointer op = report.argument_column;
    switch (tp->mode)
    {
    case op0:
        break;

    case op0_5:
        // half operand (one register)
        prreg(op, inst & 07);
        break;

    case op1:
        // single operand
        proper(op, inst);
        break;

    case op1_5:
        // 1-1/2 operand (reg,operand)
        prreg(op, inst >> 6);
        op->fputs(", ");
        proper(op, inst);
        break;

    case op2:
        // two operand
        proper(op, inst >> 6);
        op->fputs(", ");
        proper(op, inst);
        break;

    case mode_branch:
        {
            // BRanch instruction
            unsigned short locn = inst & 0x00FF;
            if (locn & 0x0080)
                locn |= 0xFF00;
            locn <<= 1;
            locn += pc;
            prlab(op, locn);
        }
        break;

    case muldiv:
        // multiply or divide (or ash[c])
        proper(op, inst);
        op->fputs(", ");
        prreg(op, inst >> 6);
        break;

    case trpemt:
        // trap or emt
        op->fprintf("%u", inst & 0xFF);
        break;

    case decbra:
        {
            // SOB instruction
            prreg(op, inst >> 6);
            op->fputs(", ");
            unsigned short locn = inst & 077;
            locn <<= 1;
            locn += pc;
            prlab(op, locn);
        }
        break;

    case fop1:
        // fdst
        pfoper(op, inst);
        break;

    case fop2a:
        // fsrc,ac
        pfoper(op, inst);
        op->fputs(", ac");
        break;

    case fop2b:
        // ac,fdst
        op->fputs("ac, ");
        pfoper(op, inst);
        break;

    case fop2c:
        // src,ac
        proper(op, inst);
        op->fputs(", ac");
        break;

    case fop2d:
        // ac,dst
        op->fputs("ac, ");
        proper(op, inst);
        break;

    case setpri:
        // SPL instruction
        op->fputc('0' + (inst & 7));
        break;

    case chgflg:
        // SEx, CLx
        if (inst & 1)
            report.opcode_column->fputc('c');
        if (inst & 2)
            report.opcode_column->fputc('v');
        if (inst & 4)
            report.opcode_column->fputc('z');
        if (inst & 8)
            report.opcode_column->fputc('n');
        break;

    case mode_undefined:
        // undefined instruction
        op->fprintf("%u", inst);
        break;
    }
    for (unsigned j = address; j < pc; ++j)
    {
        if (j > address)
            report.data_column->fputc(' ');
        report.data_column->fprintf("%02X", data[j]);
    }
    return (pc - address);
}


void
disassembler_pdp11::proper(const output::pointer &op, unsigned short r0)
{
    // print operand in r0 <5:0>
    if ((r0 & 7) == 7)
        pcoper(op, r0);
    else
        prop1(op, r0);
}


void
disassembler_pdp11::prep_proper(unsigned short r0)
{
    // print operand in r0 <5:0>
    if ((r0 & 7) == 7)
        prep_pcoper(r0);
    else
        prep_prop1(r0);
}


void
disassembler_pdp11::prop1(const output::pointer &op, unsigned short r0)
{
    // print operand in r0 <5:0>
    switch ((r0 >> 3) & 7)
    {
    case 7:
        op->fputc('@');
        // Fall through...

    case 6:
        prlab(op, get_next_word());
        prrgp(op, r0);
        break;

    case 5:
        op->fputc('@');
        // Fall through...

    case 4:
        op->fputc('-');
        prrgp(op, r0);
        break;

    case 3:
        op->fputc('@');
        // Fall through...

    case 2:
        prrgp(op, r0);
        op->fputc('+');
        break;

    case 1:
        prrgp(op, r0);
        break;

    case 0:
        prreg(op, r0);
        break;
    }
}


void
disassembler_pdp11::prep_prop1(unsigned short r0)
{
    // print operand in r0 <5:0>
    switch ((r0 >> 3) & 7)
    {
    case 7:
    case 6:
        prep_prlab(get_next_word());
        break;

    case 5:
    case 4:
    case 3:
    case 2:
    case 1:
    case 0:
        break;
    }
}


void
disassembler_pdp11::pcoper(const output::pointer &op, unsigned short r0)
{
    // handle special interpretations of 27, 37, 67, and 77
    switch ((r0 >> 3) & 7)
    {
    case 7:
        // relative deferred
        op->fputc('@');

    case 6:
        // relative
        prlab(op, (unsigned short)(pc + get_next_word()));
        break;

    case 3:
        // absolute
        op->fputc('@');

    case 2:
        op->fputc('#');
        prlab(op, get_next_word());
        break;

    default:
        prop1(op, r0);
        break;
    }
}


void
disassembler_pdp11::prep_pcoper(unsigned short r0)
{
    // handle special interpretations of 27, 37, 67, and 77
    switch ((r0 >> 3) & 7)
    {
    case 7:
    case 6:
        // relative
        prep_prlab((unsigned short)(pc + get_next_word()));
        break;

    case 3:
    case 2:
        prep_prlab(get_next_word());
        break;

    default:
        prep_prop1(r0);
        break;
    }
}


void
disassembler_pdp11::pfoper(const output::pointer &op, unsigned short r0)
{
    if (r0 & 070)
        proper(op, r0);
    else
        prfpac(op, r0);
}


void
disassembler_pdp11::prep_pfoper(unsigned short r0)
{
    if (r0 & 070)
        prep_proper(r0);
}


void
disassembler_pdp11::prrgp(const output::pointer &op, unsigned short r0)
{
    op->fputc('(');
    prreg(op, r0);
    op->fputc(')');
}


void
disassembler_pdp11::prreg(const output::pointer &op, unsigned short r0)
{
    r0 &= 7;
    switch (r0)
    {
    default:
        op->fputc('r');
        op->fputc('0' + r0);
        break;

    case 6:
        op->fputs("sp");
        break;

    case 7:
        op->fputs("pc");
        break;
    }
}


void
disassembler_pdp11::prfpac(const output::pointer &op, unsigned short r0)
{
    // print FP accumulator whose # is in r0 <2:0>
    op->fputs("ac");
    op->fputc('0' + (r0 & 7));
}


void
disassembler_pdp11::prac(const output::pointer &op, unsigned short r0)
{
    // print FP accumulator whose # is in r0 <7:6>
    prfpac(op, (r0 >> 6) & 3);
}


void
disassembler_pdp11::prlab(const output::pointer &op, unsigned short addr)
{
    rcstring name = where.get_name(addr);
    if (name.empty())
        op->fprintf("%u", addr);
    else
        op->fputs(name);
}


void
disassembler_pdp11::prep_prlab(unsigned short addr)
{
    where.add(addr);
}


bool
disassembler_pdp11::is_native(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
