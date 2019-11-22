//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#include <lib/ac/assert.h>
#include <lib/ac/ctype.h>
#include <lib/ac/string.h>

#include <lib/columnar.h>
#include <lib/output/filter/expand.h>
#include <lib/output/filter/wrap.h>



columnar::~columnar()
{
    for (size_t j = 0; j < length; ++j)
        delete list[j];
    delete [] list;
}


columnar::columnar(const output::pointer &a_deeper) :
    deeper(a_deeper),
    length(0),
    maximum(0),
    list(0),
    in_col(0),
    out_col(0)
{
}


columnar::col_t::~col_t()
{
}


columnar::col_t::col_t(int a_min, int a_max):
    icol(0),
    ocol(0),
    min(a_min),
    max(a_max)
{
    contents = output_string_list::create(max - min);
    output::pointer os2 = output_filter_wrap::create(contents, true);
    external_presentation = output_filter_expand::create(os2);
}


output::pointer
columnar::create_new_column(int min, int max)
{
    if (max <= 0)
    {
        max = deeper->get_width();
        if (max <= min)
            max = min + 1;
    }
    assert(min < max);
    if (length >= maximum)
    {
        size_t new_maximum = maximum * 2 + 8;
        col_t **new_list = new col_t * [new_maximum];
        for (size_t j = 0; j < length; ++j)
            new_list[j] = list[j];
        delete [] list;
        list = new_list;
        maximum = new_maximum;
    }
    col_t *cp = new col_t(min, max);
    list[length++] = cp;
    return cp->external_presentation;
}


/**
  * The emit_char function is used to emit characters.
  * Leading whitespace is suppressed and replaced with its own
  * idea of coling.
  *
  * @param c
  *     the character to emit.
  */

void
columnar::emit_char(char c)
{
    switch (c)
    {
    case '\n':
        deeper->fputc('\n');
        in_col = 0;
        out_col = 0;
        break;

    case ' ':
        ++in_col;
        break;

    default:
        while (out_col < in_col)
        {
            deeper->fputc(' ');
            ++out_col;
        }
        deeper->fputc(c);
        in_col++;
        out_col = in_col;
        break;
    }
}


void
columnar::eoln(void)
{
    size_t nlines = 1;
    for (size_t j = 0; j < length; ++j)
    {
        col_t *cp = list[j];
        cp->external_presentation->flush();
        cp->text = cp->contents->get();
        cp->contents->clear();

        // get rid of trailing blank lines
        while (!cp->text.empty() && cp->text.back().empty())
            cp->text.pop_back();

        if (nlines < cp->text.size())
            nlines = cp->text.size();
    }

    for (size_t line = 0; line < nlines; ++line)
    {
        for (size_t j = 0; j < length; ++j)
        {
            const col_t *cp = list[j];
            if (line >= cp->text.size())
                continue;
            rcstring s = cp->text[line];
            if (s.empty())
                continue;
            if (cp->min > 0 && in_col >= cp->min)
                emit_char('\n');
            while (in_col < cp->min)
                emit_char(' ');
            for (const char *ss = s.c_str(); *ss; ++ss)
                emit_char(*ss);
        }
        emit_char('\n');
    }

    for (size_t j = 0; j < length; ++j)
    {
        col_t *cp = list[j];
        cp->text.clear();
    }
}


void
columnar::flush(void)
{
    deeper->flush();
}


// vim: set ts=8 sw=4 et :
