//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
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

#include <lib/ac/limits.h>

#include <lib/code_source_map.h>
#include <lib/codefile.h>
#include <lib/debug.h>
#include <lib/disassembler.h>
#include <lib/dislabel.h>
#include <lib/input/file.h>
#include <lib/mtype.h>
#include <lib/output.h>
#include <lib/symbol/variable.h>


code_source_map::~code_source_map()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


code_source_map::code_source_map(const rcstring &filename) :
    report(output::factory(filename))
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


code_source_map::pointer
code_source_map::create(const rcstring &filename)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new code_source_map(filename));
}


void
code_source_map::associate(unsigned segnum, unsigned address,
    const location &locn)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    if (locn.get_file_name().empty())
    {
        DEBUG(1, "}");
        return;
    }
    if (locn.get_file_name() == location::builtin().get_file_name())
    {
        DEBUG(1, "}");
        return;
    }

    //
    // We want to minimize the number of locations we have hanging
    // around in memory, so we cache them.
    //
    DEBUG(2, "segnum = %d", segnum);
    DEBUG(2, "address = %04X", address);
    rcstring name_and_line =
        rcstring::printf
        (
            "%s: %d",
            locn.get_file_name().c_str(),
            locn.get_line_number()
        );
    DEBUG(2, "name_and_line = %s", name_and_line.quote_c().c_str());
    location_p lp = location_by_name_and_line.lookup(name_and_line);
    if (!lp)
    {
        lp = location_p(new location(locn));
        location_by_name_and_line.assign(name_and_line, lp);
    }

    //
    // Now associate the address with the location.  Don't worry about
    // the size of the key, the data is fairly sparse.
    //
    rcstring segaddr = rcstring::printf("%02X:%04X", segnum, address);
    DEBUG(2, "segaddr = %s", segaddr.quote_c().c_str());
    location_by_segnum_and_addr.assign(segaddr, lp);
    DEBUG(1, "}");
}


void
code_source_map::list_to_here(unsigned segnum, unsigned address)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "segnum = %d", segnum);
    DEBUG(2, "address = %04X", address);
    //
    // See if we have a mapping for this line.
    //
    rcstring segaddr = rcstring::printf("%02X:%04X", segnum, address);
    location_p lp = location_by_segnum_and_addr.lookup(segaddr);
    if (!lp)
    {
        DEBUG(1, "}");
        return;
    }

    //
    // Locate (or create) a source file by name.
    //
    progress::pointer pp = progress_by_filename.lookup(lp->get_file_name());
    if (!pp)
    {
        DEBUG(2, "open %s", lp->get_file_name().quote_c().c_str());
        pp = progress::create(lp->get_file_name());
        progress_by_filename.assign(lp->get_file_name(), pp);
    }

    //
    // Print the source up to this point into the listing.
    //
    while (pp->get_line_number() < lp->get_line_number())
    {
        DEBUG(2, "line %d", pp->get_line_number());
        pp->list_one_line(report);
    }
    DEBUG(1, "}");
}


code_source_map::progress::~progress()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


code_source_map::progress::progress(const rcstring &file_name) :
    base(file_name.basename()),
    line_number(0),
    ip(input_file::create(file_name))
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


code_source_map::progress::pointer
code_source_map::progress::create(const rcstring &file_name)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new progress(file_name));
}


int
code_source_map::progress::get_line_number()
{
    return line_number;
}


void
code_source_map::progress::list_one_line(discolumns &report)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    rcstring line;
    if (!ip->one_line(line))
    {
        line_number = INT_MAX;
        return;
    }
    ++line_number;
    DEBUG(2, "line = %s", line.quote_c().c_str());
    // We include a semicolon so it looks like a comment amongst the
    // assembler code.
    report.full->fprintf("; %s: %u:", base.c_str(), line_number);
    report.source_column->fputs(line);
    report.eoln();
}


void
code_source_map::disassemble(mtype_t mtype, const unsigned char *data,
    unsigned enter_ic, unsigned data_size, int seg_num, unsigned proc_number,
    codefile *cfp)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    report.full->fprintf("; Procedure %u:", proc_number);
    report.eoln();
    dislabel where(rcstring::printf("p%u", proc_number));
    bool native = false;
    switch (mtype)
    {
    case mtype_undefined:
        // This is what pre-II.0 system use for p-code.

    case mtype_pcode_be:
    case mtype_pcode_le:
        break;

    case mtype_pdp11:
    case mtype_8080:
    case mtype_z80:
    case mtype_ga440:
    case mtype_6502:
    case mtype_6800:
    case mtype_ti9900:
    default:
        native = true;
        break;
    }
    disassembler::pointer dis =
        (
            native
        ?
            disassembler::factory
            (
                mtype,
                where,
                report,
                seg_num,
                proc_number,
                enter_ic,
                this
            )
        :
            cfp->native_disassembler_factory
            (
                mtype,
                where,
                report,
                seg_num,
                proc_number,
                enter_ic,
                this
            )
        );
    dis->process(data, enter_ic, data_size, proc_number);
    DEBUG(2, "}");
}


void
code_source_map::forget_associations(void)
{
    location_by_name_and_line.clear();
    location_by_segnum_and_addr.clear();
}


void
code_source_map::symbol_listing(const symbol &sym)
{
    report.sym_location_column->fprintf
    (
        "; %s: %d:",
        sym.get_declaration_location().get_file_name().basename().c_str(),
        sym.get_declaration_location().get_line_number()
    );
    const symbol_variable *svp = dynamic_cast<const symbol_variable *>(&sym);
    if (svp)
    {
        rcstring addr = sym.get_bit_offset().representation();
        report.sym_address_column->fprintf("%4s", addr.c_str());
        unsigned n = sym.get_size_in_bits();
        n = (n + 15) >>4;
        report.sym_size_column->fprintf("%3u", n);
    }
    report.sym_kind_column->fputs(sym.get_kind());
    report.sym_name_column->fputs(sym.get_name());
    report.sym_type_column->fputs(sym.get_type()->get_pascal_name());
    report.eoln();
}


// vim: set ts=8 sw=4 et :
