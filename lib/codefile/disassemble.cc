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

#include <lib/codefile.h>
#include <lib/output/filter/prefix.h>
#include <lib/rcstring/list.h>
#include <lib/segment.h>

struct table_t
{
    int block_start;
    int block_length;
    const segment *sp;
    bool interface;
};


static int
cmp(const void *va, const void *vb)
{
    const table_t *a = (const table_t *)va;
    const table_t *b = (const table_t *)vb;
    return (a->block_start - b->block_start);
}


void
codefile::disassemble(output::pointer &os, int flags)
{
    os->fprintf("; Disassembly of %s\n", get_filename().quote_c().c_str());

    //
    // Print the copyright notice.
    //
    {
        rcstring notice = get_copyright_notice();
        rcstring_list notice_lines;
        notice_lines.split(notice, "\n", false);
        for (size_t j = 0; j < notice_lines.size(); ++j)
        {
            rcstring line = notice_lines[j].trim();
            if (!line.empty())
                os->fprintf("; %s\n", line.c_str());
        }
    }

    //
    // Account for each block of the codefile.
    //
    if (flags & 0x80)
    {
        table_t table[32];
        size_t tablen = 0;
        for (size_t sn = 0; ; ++sn)
        {
            segment::pointer sp = get_segment(sn);
            if (!sp)
                break;
            if (sp->get_kind() == DATASEG)
            {
                assert(sp->get_code_address() == 0);
                continue;
            }
            table_t t;
            t.block_start = sp->get_code_address();
            t.block_length = (sp->get_code_length() + 511) >> 9;
            t.sp = sp.get();
            t.interface = 0;
            table[tablen++] = t;

            t.block_start = sp->get_text_address();
            if (t.block_start)
            {
                t.block_length = 1;
                t.interface = true;
                table[tablen++] = t;
            }
        }
        qsort(table, tablen, sizeof(table[0]), cmp);

        os->fprintf("\n");
        output::pointer pfx = output_filter_prefix::create(os, ";    * ");
        pfx->fprintf("*****************************************************\n");
        pfx->fprintf("\n");
        pfx->fprintf("Layout of segments within the codefile:\n");
        library_map_columns cols(pfx);
        cols.eoln();
        cols.headings();
        cols.codeaddr->fprintf("%4d", 0);
        cols.codeleng->fprintf("%4d", 1);
        cols.name->fputs("seg dict");
        cols.mtype->fputs("data");
        cols.eoln();
        for (size_t j = 0; j < tablen; ++j)
        {
            const table_t *tp = &table[j];
            cols.codeaddr->fprintf("%4d", tp->block_start);
            cols.codeleng->fprintf("%4d", tp->block_length);
            cols.name->fputs(tp->sp->get_name());
            if (tp->interface)
            {
                cols.segkind->fputs("interfac");
                cols.mtype->fputs("text");
            }
            else
            {
                cols.segkind->fputs(segkind_name(tp->sp->get_kind()));
            }
            cols.segnum->fprintf("%3d", tp->sp->get_segment_number());
            cols.eoln();

            unsigned block_end =
                ((j + 1 < tablen) ? table[j + 1].block_start : size_in_blocks);
            unsigned start2 = tp->block_start + tp->block_length;
            if (start2 < block_end)
            {
                cols.codeaddr->fprintf("%4d", start2);
                unsigned length2 = block_end - start2;
                cols.codeleng->fprintf("%4d", length2);
                cols.name->fputc('?');
                cols.segkind->fputc('?');
                cols.segnum->fputc('?');
                cols.mtype->fputc('?');
                cols.version->fputc('?');
                cols.eoln();
            }
        }
        pfx->fprintf("*****************************************************\n");
    }

    //
    // Print each of the segments.
    //
    for (size_t sn = 0; ; ++sn)
    {
        segment::pointer sp = get_segment(sn);
        if (!sp)
            break;
        // Watch out: DATASEG don't have any code in the codefile.
        if (sp->get_code_address())
            sp->disassemble(os, flags, this);
    }
}


// vim: set ts=8 sw=4 et :
