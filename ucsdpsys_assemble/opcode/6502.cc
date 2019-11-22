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

#include <lib/debug.h>
#include <lib/expression/addition.h>
#include <lib/expression/dereference.h>
#include <lib/expression/integer.h>
#include <lib/expression/list.h>
#include <lib/type/integer.h>

#include <ucsdpsys_assemble/assembler.h>
#include <ucsdpsys_assemble/expression/6502/absolute.h>
#include <ucsdpsys_assemble/expression/6502/absolute_indexed.h>
#include <ucsdpsys_assemble/expression/6502/absolute_indexed_indirect.h>
#include <ucsdpsys_assemble/expression/6502/absolute_indirect.h>
#include <ucsdpsys_assemble/expression/6502/absolute_indirect_indexed.h>
#include <ucsdpsys_assemble/expression/6502/immediate.h>
#include <ucsdpsys_assemble/expression/integer/asm.h>
#include <ucsdpsys_assemble/expression/linker_reference.h>
#include <ucsdpsys_assemble/expression/range_check.h>
#include <ucsdpsys_assemble/expression/special/a.h>
#include <ucsdpsys_assemble/expression/special/x.h>
#include <ucsdpsys_assemble/expression/special/y.h>
#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/opcode/6502.h>
#include <ucsdpsys_assemble/type/linksym_relative.h>
#include <ucsdpsys_assemble/type/segment_relative.h>


opcode_6502::~opcode_6502()
{
}


opcode_6502::opcode_6502(assembler *a_context, model_t a_model) :
    opcode(a_context),
    model(a_model)
{
}


void
opcode_6502::argument_list_error(const expression_list &args)
    const
{
    if
    (
        args.size() == 1
    &&
        boost::dynamic_pointer_cast<expression_6502>(args[0])
    )
    {
        lex_error
        (
            args[0]->get_location(),
            "opcode %s: parameter one: addressing mode not supported",
            get_name().quote_c().c_str()
        );
    }
    else
    {
        lex_error
        (
            args.get_location(),
            "opcode %s: parameter list invalid",
            get_name().quote_c().c_str()
        );
    }
}


bool
opcode_6502::mode_is_normal8(const expression_list &args, unsigned char opc)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (args.size() == 1)
        DEBUG(2, "arg = %s", args[0]->lisp_representation().c_str());
    opc &= 0xE3;

    // The order matters,
    //     - test zero page alternatives before absolute
    //     - test least decorated alternatives after more decorated
    return
        (
            mode_is_zeropage_indexed_indirect_x(args, opc | 0x00)
        ||
            mode_is_zeropage_indirect_indexed_y(args, opc | 0x10)
        ||
            mode_is_zeropage_indexed_x(args,          opc | 0x14)
        ||
            mode_is_absolute_indexed_y(args,          opc | 0x18)
        ||
            mode_is_absolute_indexed_x(args,          opc | 0x1C)
        ||
            mode_is_zeropage(args,                    opc | 0x04)
        ||
            mode_is_absolute(args,                    opc | 0x0C)
        ||
            mode_is_immediate(args,                   opc | 0x08)
        );
}


bool
opcode_6502::mode_is_normal5a(const expression_list &args, unsigned char opc)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    opc &= 0xE3;

    // The order matters,
    //     - test zero page alternatives before absolute
    //     - test least decorated alternatives after more decorated
    return
        (
            mode_is_accumulator(args,        opc | 0x08)
        ||
            mode_is_zeropage_indexed_x(args, opc | 0x14)
        ||
            mode_is_absolute_indexed_x(args, opc | 0x1C)
        ||
            mode_is_zeropage(args,           opc | 0x04)
        ||
            mode_is_absolute(args,           opc | 0x0C)
        );
}


bool
opcode_6502::mode_is_absolute(const expression_list &args, unsigned char opc)
{
    if (args.size() != 1)
        return false;
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression_6502_absolute::pointer ep =
        boost::dynamic_pointer_cast<expression_6502_absolute>(args[0]);
    if (!ep)
        return false;
    expression::pointer ep2 = ep->get_address()->optimize();
    DEBUG(2, "ep = %s", ep->lisp_representation().c_str());

    if (type_integer::is_a(ep2))
    {
        emit_byte(opc);
        emit_word(expression_range_check::create(ep2, 0, 65535));
        return true;
    }
    if (type_segment_relative::is_a(ep2))
    {
        expression::pointer enter_ic =
            get_context()->enter_ic_expression_factory(ep2->get_location());
        expression::pointer rel =
            get_context()->subtraction_expression_factory(ep2, enter_ic);
        emit_byte(opc);
        reloc_type1_lcrel();
        emit_word(expression_range_check::create(rel, -32768, 32767));
        return true;
    }
    if (type_linksym_relative::is_a(ep2))
    {
        emit_byte(opc);
        expression_linker_reference::pointer ep3 =
            boost::dynamic_pointer_cast<expression_linker_reference>(ep2);
        if (ep3)
            ep2->traversal(0);
        else
            emit_word(ep2);
        return true;
    }
    return false;
}


bool
opcode_6502::mode_is_implicit(const expression_list &args, unsigned char opc)
{
    if (!args.empty())
        return false;
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    emit_byte(opc);
    return true;
}


bool
opcode_6502::mode_is_zeropage_indexed_x(const expression_list &args,
    unsigned char opc)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (args.size() != 1)
        return false;
    expression_6502_absolute_indexed::pointer ep1 =
        boost::dynamic_pointer_cast<expression_6502_absolute_indexed>(args[0]);
    if (!ep1)
        return false;

    expression::pointer ep2a = ep1->get_address()->optimize();
    expression_integer::pointer ep2 =
        boost::dynamic_pointer_cast<expression_integer>(ep2a);
    if (!ep2)
        return false;
    long n = ep2->get_integer_value();
    if (n < 0 || n >= 256)
        return false;

    expression_special_x::pointer ep3 =
        boost::dynamic_pointer_cast<expression_special_x>(ep1->get_special());
    if (!ep3)
        return false;

    emit_byte(opc);
    emit_byte(n);
    return true;
}


bool
opcode_6502::mode_is_zeropage_indexed_y(const expression_list &args,
    unsigned char opc)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (args.size() != 1)
        return false;
    expression_6502_absolute_indexed::pointer ep1 =
        boost::dynamic_pointer_cast<expression_6502_absolute_indexed>(args[0]);
    if (!ep1)
        return false;

    expression::pointer ep2a = ep1->get_address()->optimize();
    expression_integer::pointer ep2 =
        boost::dynamic_pointer_cast<expression_integer>(ep2a);
    if (!ep2)
        return false;
    long n = ep2->get_integer_value();
    if (n < 0 || n >= 256)
        return false;

    expression_special_y::pointer ep3 =
        boost::dynamic_pointer_cast<expression_special_y>(ep1->get_special());
    if (!ep3)
        return false;

    emit_byte(opc);
    emit_byte(n);
    return true;
}


bool
opcode_6502::mode_is_absolute_indexed_x(const expression_list &args,
    unsigned char opc)
{
    if (args.size() != 1)
        return false;
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression_6502_absolute_indexed::pointer ep1 =
        boost::dynamic_pointer_cast<expression_6502_absolute_indexed>(args[0]);
    if (!ep1)
        return false;

    expression::pointer ep2 = ep1->get_address();

    expression_special_x::pointer ep3 =
        boost::dynamic_pointer_cast<expression_special_x>(ep1->get_special());
    if (!ep3)
        return false;

    if (type_segment_relative::is_a(ep2))
    {
        expression::pointer enter_ic =
            get_context()->enter_ic_expression_factory(ep2->get_location());
        expression::pointer rel =
            get_context()->subtraction_expression_factory(ep2, enter_ic);
        emit_byte(opc);
        reloc_type1_lcrel();
        emit_word(expression_range_check::create(rel, -32768, 32767));
        return true;
    }
    if (type_integer::is_a(ep2))
    {
        emit_byte(opc);
        emit_word(expression_range_check::create(ep2, 0, 65535));
        return true;
    }
    if (type_linksym_relative::is_a(ep2))
    {
        emit_byte(opc);
        ep2->traversal(0);
        return true;
    }
    return false;
}


bool
opcode_6502::mode_is_absolute_indexed_y(const expression_list &args,
    unsigned char opc)
{
    if (args.size() != 1)
        return false;
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression_6502_absolute_indexed::pointer ep1 =
        boost::dynamic_pointer_cast<expression_6502_absolute_indexed>(args[0]);
    if (!ep1)
        return false;

    expression::pointer ep2 = ep1->get_address();

    expression_special_y::pointer ep3 =
        boost::dynamic_pointer_cast<expression_special_y>(ep1->get_special());
    if (!ep3)
        return false;

    if (type_segment_relative::is_a(ep2))
    {
        expression::pointer enter_ic =
            get_context()->enter_ic_expression_factory(ep2->get_location());
        expression::pointer rel =
            get_context()->subtraction_expression_factory(ep2, enter_ic);
        emit_byte(opc);
        reloc_type1_lcrel();
        emit_word(expression_range_check::create(rel, -32768, 32767));
        return true;
    }
    if (type_integer::is_a(ep2))
    {
        emit_byte(opc);
        emit_word(expression_range_check::create(ep2, 0, 65535));
        return true;
    }
    if (type_linksym_relative::is_a(ep2))
    {
        emit_byte(opc);
        ep2->traversal(0);
        return true;
    }
    return false;
}


bool
opcode_6502::mode_is_zeropage(const expression_list &args, unsigned char opc)
{
    if (args.size() != 1)
        return false;
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression_6502_absolute::pointer ep =
        boost::dynamic_pointer_cast<expression_6502_absolute>(args[0]);
    if (!ep)
        return false;
    expression::pointer ep2a = ep->get_address()->optimize();
    expression_integer::pointer ep2 =
        boost::dynamic_pointer_cast<expression_integer>(ep2a);
    if (!ep2)
        return false;
    long n = ep2->get_integer_value();
    if (n < 0 || n >= 256)
        return false;

    emit_byte(opc);
    emit_byte(n);
    return true;
}


bool
opcode_6502::mode_is_immediate(const expression_list &args, unsigned char opc)
{
    if (args.size() != 1)
        return false;
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression_6502_immediate::pointer ep =
        boost::dynamic_pointer_cast<expression_6502_immediate>(args[0]);
    if (!ep)
    {
        DEBUG(2, "no");
        return false;
    }
    emit_byte(opc);
    emit_byte(expression_range_check::create(ep->get_operand(), -128, 255));
    return true;
}


bool
opcode_6502::mode_is_accumulator(const expression_list &args, unsigned char opc)
{
    if (args.size() != 1)
        return false;
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression_special_a::pointer ep =
        boost::dynamic_pointer_cast<expression_special_a>(args[0]);
    if (!ep)
    {
        DEBUG(2, "no");
        return false;
    }
    emit_byte(opc);
    return true;
}


bool
opcode_6502::mode_is_zeropage_indirect(const expression_list &args,
    unsigned char opc)
{
    if (args.size() != 1)
        return false;
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression_6502_absolute_indirect::pointer ep1 =
        boost::dynamic_pointer_cast<expression_6502_absolute_indirect>(args[0]);
    if (!ep1)
    {
        DEBUG(2, "no");
        return false;
    }

    expression::pointer ep2a = ep1->get_address()->optimize();
    expression_integer::pointer ep2 =
        boost::dynamic_pointer_cast<expression_integer>(ep2a);
    if (!ep2)
    {
        DEBUG(2, "no");
        return false;
    }
    long n = ep2->get_integer_value();
    if (n < 0 || n >= 256)
    {
        DEBUG(2, "no");
        return false;
    }

    emit_byte(opc);
    emit_byte(n);
    return true;
}


bool
opcode_6502::mode_is_absolute_indirect(const expression_list &args,
    unsigned char opc)
{
    if (args.size() != 1)
        return false;
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression_6502_absolute_indirect::pointer ep1 =
        boost::dynamic_pointer_cast<expression_6502_absolute_indirect>(args[0]);
    if (!ep1)
    {
        DEBUG(2, "no");
        return false;
    }
    expression::pointer ep2 = ep1->get_address();
    if (type_segment_relative::is_a(ep2))
    {
        expression::pointer enter_ic =
            get_context()->enter_ic_expression_factory(ep2->get_location());
        expression::pointer rel =
            get_context()->subtraction_expression_factory(ep2, enter_ic);
        emit_byte(opc);
        reloc_type1_lcrel();
        emit_word(expression_range_check::create(rel, -32768, 32767));
        return true;
    }
    if (type_integer::is_a(ep2))
    {
        emit_byte(opc);
        emit_word(expression_range_check::create(ep2, 0, 65535));
        return true;
    }
    if (type_linksym_relative::is_a(ep2))
    {
        emit_byte(opc);
        ep2->traversal(0);
        return true;
    }
    return false;
}


bool
opcode_6502::mode_is_absolute_indexed_indirect_x(const expression_list &args,
    unsigned char opc)
{
    if (args.size() != 1)
        return false;
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression_6502_absolute_indexed_indirect::pointer ep1 =
        boost::dynamic_pointer_cast<expression_6502_absolute_indexed_indirect>(
            args[0]);
    if (!ep1)
    {
        DEBUG(2, "no");
        return false;
    }

    expression::pointer ep2 = ep1->get_address();

    expression_special_x::pointer ep3 =
        boost::dynamic_pointer_cast<expression_special_x>(ep1->get_special());
    if (!ep3)
    {
        DEBUG(2, "no");
        return false;
    }

    if (type_integer::is_a(ep2))
    {
        emit_byte(opc);
        emit_word(expression_range_check::create(ep2, 0, 65535));
        return true;
    }
    if (type_linksym_relative::is_a(ep2))
    {
        emit_byte(opc);
        ep2->traversal(0);
        return true;
    }

    return false;
}


bool
opcode_6502::mode_is_zeropage_indexed_indirect_x(const expression_list &args,
    unsigned char opc)
{
    if (args.size() != 1)
        return false;
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression_6502_absolute_indexed_indirect::pointer ep1 =
        boost::dynamic_pointer_cast<expression_6502_absolute_indexed_indirect>(
            args[0]);
    if (!ep1)
    {
        DEBUG(2, "no");
        return false;
    }

    expression::pointer ep2a = ep1->get_address()->optimize();
    expression_integer::pointer ep2 =
        boost::dynamic_pointer_cast<expression_integer>(ep2a);
    if (!ep2)
    {
        DEBUG(2, "no");
        return false;
    }
    long n = ep2->get_integer_value();
    if (n < 0 || n >= 256)
    {
        DEBUG(2, "no");
        return false;
    }

    expression_special_x::pointer ep3 =
        boost::dynamic_pointer_cast<expression_special_x>(ep1->get_special());
    if (!ep3)
    {
        DEBUG(2, "no");
        return false;
    }

    emit_byte(opc);
    emit_byte(n);
    return true;
}


bool
opcode_6502::mode_is_relative(const expression_list &args, unsigned char opc)
{
    if (args.size() != 1)
        return false;
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression::pointer dollar =
        get_context()->dollar_expression_factory(args[0]->get_location(), 2);
    expression::pointer ep =
        get_context()->subtraction_expression_factory(args[0], dollar);

    emit_byte(opc);
    emit_byte(expression_range_check::create(ep, -128, 127));
    return true;
}


bool
opcode_6502::mode_is_zeropage_indirect_indexed_y(const expression_list &args,
    unsigned char opc)
{
    if (args.size() != 1)
        return false;
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression_6502_absolute_indirect_indexed::pointer ep1 =
        boost::dynamic_pointer_cast<expression_6502_absolute_indirect_indexed>(
            args[0]);
    if (!ep1)
    {
        DEBUG(2, "no");
        return false;
    }

    expression::pointer ep2 = ep1->get_address()->optimize();

    expression_special_y::pointer ep3 =
        boost::dynamic_pointer_cast<expression_special_y>(ep1->get_special());
    if (!ep3)
    {
        DEBUG(2, "no");
        return false;
    }

    emit_byte(opc);
    emit_byte(expression_range_check::create(ep2, 0, 255));
    return true;
}


static const char *
model_name(opcode_6502::model_t x)
{
    switch (x)
    {
    default:
    case opcode_6502::model_6502:
        return "6502";

    case opcode_6502::model_65c02:
        return "65C02";

    case opcode_6502::model_65c02s:
        return "65C02s";

    case opcode_6502::model_65c02s_wdc:
        return "65C02s-WDC";
    }
}


bool
opcode_6502::model_warning(model_t first_present_in)
    const
{
    if (model < first_present_in)
    {
        grammar_warning
        (
            "the %s opcode may not be present on a %s chip",
            get_name().quote_c().c_str(),
            model_name(model)
        );

        static model_t extra_warning;
        if (extra_warning < first_present_in)
        {
            extra_warning = first_present_in;
            grammar_warning
            (
                "the .arch \"%s\" pseudo-opcode can be used to silence "
                    "this warning",
                model_name(first_present_in)
            );
        }
    }
    // We return true because we are designed to be used inside an if
    // condition, but not to change the semantics.
    return true;
}


// vim: set ts=8 sw=4 et :
