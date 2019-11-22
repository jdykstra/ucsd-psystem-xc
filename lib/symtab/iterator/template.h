//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2012 Peter Miller
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

#ifndef LIB_SYMTAB_ITERATOR_TEMPLATE_H
#define LIB_SYMTAB_ITERATOR_TEMPLATE_H

#include <lib/symtab/iterator.h>

template <class value_type_t> class symtab; // forward

/**
  * The symtab_iterator<> class is used to represent a type safe
  * iterator implemented using the underlying void pointer iterator.
  */
template <class value_type_t>
class symtab_iterator
{
public:
    /**
      * The destructor.
      */
    ~symtab_iterator()
    {
    }

    /**
      * The default constructor.
      */
    symtab_iterator()
    {
    }

    /**
      * The constructor.
      */
    symtab_iterator(symtab_t *a_stp, bool begin) :
        sti(a_stp, begin)
    {
    }

    /**
      * The copy constructor.
      */
    symtab_iterator(const symtab_iterator &arg)
    {
        sti = arg.sti;
    }

    /**
      * The assignment operator.
      */
    symtab_iterator &
    operator=(const symtab_iterator &arg)
    {
        if (this != &arg)
            sti = arg.sti;
        return *this;
    }

    void
    operator++()
    {
        sti.next();
    }

    value_type_t *
    operator*()
    {
        return (value_type_t *)sti.get();
    }

    value_type_t *
    operator->()
    {
        return (value_type_t *)sti.get();
    }

    bool
    operator==(const symtab_iterator<value_type_t> &arg)
    {
        return sti.equal(arg.sti);
    }

    bool
    operator!=(const symtab_iterator<value_type_t> &arg)
    {
        return !sti.equal(arg.sti);
    }

private:
    /**
      * The sti instance variable is used to remember where we are up to
      * in the underlying void pointer iterator.
      */
    symtab_iterator_t sti;
};

#endif // LIB_SYMTAB_ITERATOR_TEMPLATE_H
// vim: set ts=8 sw=4 et :
