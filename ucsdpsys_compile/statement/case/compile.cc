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

#include <lib/pascal/lex.h>
#include <lib/pcode.h>

#include <ucsdpsys_compile/label/compile.h>
#include <ucsdpsys_compile/statement/case/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


statement_case_compile::~statement_case_compile()
{
}


statement_case_compile::statement_case_compile(
    const expression::pointer &a_control,
    const case_list &a_cases,
    const statement::pointer &a_otherwise,
    translator_compile *a_code
) :
    statement_case(a_control, a_cases, a_otherwise),
    code(*a_code)
{
    int minimum = get_minimum();
    int maximum = get_maximum();
    unsigned size_of_table = 8 + 2 * (maximum - minimum);
    if (size_of_table > 1200)
    {
        // That 1200 looks arbitrary, but it isn't.  It's the maximum
        // code size per function permitted by the Apple Pascal compiler.
        pascal_lex_error
        (
            get_control()->get_location(),
            "case statement XJP is way too big (%u cases, %u bytes), "
                "now is the time to rethink your algorithm",
            maximum + 1 - minimum,
            size_of_table
        );
    }
}


statement_case_compile::pointer
statement_case_compile::create(const expression::pointer &a_control,
    const case_list &a_cases, const statement::pointer &a_otherwise,
    translator_compile *a_code)
{
    return
        pointer
        (
            new statement_case_compile(a_control, a_cases, a_otherwise, a_code)
        );
}


void
statement_case_compile::traversal(void)
    const
{
    if (code.get_short_case())
        traversal_new_style();
    else
        traversal_old_style();
}


void
statement_case_compile::traversal_old_style(void)
    const
{
    // Generate the code for the control expression,
    // but we don't use it yet.
    get_control()->optimize()->traversal(0);
    label::pointer actually_xjp = label_compile::create(&code);
    actually_xjp->goto_from_here();

    //
    // Spew out the cases, keeping track of duplicates for re-use.
    //
    label::pointer end_label = label_compile::create(&code);
    label::pointer otherwise_label = end_label;
    typedef std::map<statement::pointer, label::pointer> magic_t;
    magic_t magic;
    int minimum = get_minimum();
    int maximum = get_maximum();
    for (int value = minimum; value <= maximum; ++value)
    {
        statement::pointer sp = get_case(value);
        if (!sp)
            continue;
        magic_t::const_iterator it = magic.find(sp);
        if (it != magic.end())
            continue;
        label::pointer lp = label_compile::create(&code);
        lp->define_here();
        sp->traversal();
        end_label->goto_from_here();
        magic.insert(magic_t::value_type(sp, lp));
    }
    if (get_otherwise())
    {
        otherwise_label = label_compile::create(&code);
        otherwise_label->define_here();
        get_otherwise()->traversal();
        end_label->goto_from_here();
    }

    //
    // Now emit the actual table.
    //
    code.emit_odd_alignment(NOP);
    actually_xjp->define_here();
    code.emit_byte(XJP);
    code.emit_word(minimum);
    code.emit_word(maximum);
    label::pointer proxy_otherwise = label_compile::create(&code);
    proxy_otherwise->define_here();
    otherwise_label->goto_from_here();
    for (int value = minimum; value <= maximum; ++value)
    {
        statement::pointer sp = get_case(value);
        if (sp)
        {
            magic_t::const_iterator it = magic.find(sp);
            assert(it != magic.end());
            label::pointer lp = it->second;
            lp->self_relative_from_here();
        }
        else
        {
            proxy_otherwise->self_relative_from_here();
        }
    }
    end_label->define_here();
}


void
statement_case_compile::traversal_new_style(void)
    const
{
    //
    // Generate the code for the control expression.
    //
    get_control()->optimize()->traversal(0);

    //
    // Now emit the actual table.
    //
    code.emit_odd_alignment(NOP);
    code.emit_byte(XJP);
    int minimum = get_minimum();
    int maximum = get_maximum();
    code.emit_word(minimum);
    code.emit_word(maximum);
    label::pointer end_label = label_compile::create(&code);
    label::pointer otherwise_label = label_compile::create(&code);
    otherwise_label->goto_from_here();
    typedef std::map<statement::pointer, label::pointer> lbl_stmt_t;
    lbl_stmt_t lbl_stmt;
    for (int value = minimum; value <= maximum; ++value)
    {
        statement::pointer sp = get_case(value);
        if (sp)
        {
            lbl_stmt_t::const_iterator it = lbl_stmt.find(sp);
            if (it != lbl_stmt.end())
            {
                label::pointer lp = it->second;
                lp->self_relative_from_here();
            }
            else
            {
                label::pointer lp = label_compile::create(&code);
                lbl_stmt.insert(lbl_stmt_t::value_type(sp, lp));
                lp->self_relative_from_here();
            }
        }
        else
        {
            otherwise_label->self_relative_from_here();
        }
    }

    //
    // Code generate the cases.
    //
    // We care about the order we generate the cases because of the
    // automatic testing.  Rather than have the order change seemingly
    // at random between runs, we generate them in ascending order of
    // case values.
    //
    typedef std::list<statement::pointer> done_t;
    done_t done;
    for (int value = minimum; value <= maximum; ++value)
    {
        statement::pointer sp = get_case(value);
        if (!sp)
            continue;
        done_t::const_iterator it = std::find(done.begin(), done.end(), sp);
        if (it != done.end())
            continue;
        done.push_back(sp);

        lbl_stmt_t::const_iterator it2 = lbl_stmt.find(sp);
        assert(it2 != lbl_stmt.end());
        label::pointer lp = it2->second;
        assert(lp);
        lp->define_here();
        sp->traversal();
        end_label->goto_from_here();
    }
    if (get_otherwise())
    {
        otherwise_label->define_here();
        get_otherwise()->traversal();
    }
    else
        otherwise_label->define_here();
    end_label->define_here();
}


// vim: set ts=8 sw=4 et :
