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
#include <lib/expression/list.h>
#include <lib/pascal/lex.h>
#include <lib/translator.h>


void
translator::variant_record_begin(const type::pointer &tp)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    variant::pointer vp = variant::create(tp);
    unsigned nwords = get_data_mark().get_size_in_words();
    vp->set_otherwise_size(nwords);
    variants.push_back(vp);
    DEBUG(1, "}");
}


void
translator::variant_record_field(const expression_list &selectors,
    const variant::pointer &child)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    assert(!variants.empty());
    variant::pointer vp = variants.back();
    assert(vp);
    for (size_t j = 0; j < selectors.size(); ++j)
    {
        DEBUG(2, "j = %d", (int)j);
        expression::pointer ep = selectors[j]->optimize();
        if (!ep->is_constant())
        {
            DEBUG(2, "not constant");
            pascal_lex_error
            (
                ep->get_location(),
                "record variant selectors must be constant"
            );
        }
        else if (vp->get_type() != ep->get_type())
        {
            DEBUG(2, "not correct type");
            pascal_lex_error
            (
                ep->get_location(),
                "expected a variant selector of %s type, not a %s",
                vp->get_type()->get_name().c_str(),
                ep->get_description().c_str()
            );
        }
        else
        {
            DEBUG(2, "looks ok");
            long value = ep->get_integer_value();
            assert(!variants.empty());
            vp->register_child(value, child);
        }
    }
    DEBUG(1, "}");
}


variant::pointer
translator::variant_record_end(void)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    // This method is called just before translator::set_data_mark() is
    // called by the grammar at the end of a record declaration
    assert(!variants.empty());
    variant::pointer result = variants.back();
    assert(result);
    variants.pop_back();
    if (result->get_size_in_words() == 0)
    {
        // no children inside the variant
        unsigned sz = get_data_mark().get_size_in_words();
        assert(sz > 0);
        result->set_size(sz);
    }
    DEBUG(1, "}");
    return result;
}


// vim: set ts=8 sw=4 et :
