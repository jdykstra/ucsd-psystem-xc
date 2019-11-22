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

#ifndef LIB_SYMTAB_ITERATOR_H
#define LIB_SYMTAB_ITERATOR_H

class symtab_t; // forward

/**
  * The symtab_iterator_t class is used to represent the state of an
  * iterator_t as it progresses through the symbols in a symbol table.
  */
class symtab_iterator_t
{
public:
    /**
      * The destructor.
      *
      * @note
      *     it is not virtual, thou shalt not derive from it.
      */
    ~symtab_iterator_t();

    /**
      * The default constructor.
      */
    symtab_iterator_t();

    /**
      * The constructor.
      */
    symtab_iterator_t(const symtab_t *stp, bool begin);

    /**
      * The copy constructor.
      */
    symtab_iterator_t(const symtab_iterator_t &rhs);

    /**
      * The assignment operator.
      */
    symtab_iterator_t &operator=(const symtab_iterator_t &rhs);

    /**
      * The next method is used to advance the iterator to the next item
      * in the symbol table.
      */
    void next(void);

    /**
      * The get method is used to obtain the void pointer to the item in
      * the symbol table, or NULL if we are at the end(void).
      */
    void *get(void) const;

    /**
      * The equal method is used determine whether or not two iterators
      * are equal.
      */
    bool equal(const symtab_iterator_t &rhs) const;

private:
    /**
      * The stp instance variable is used to remember the symbol table
      * we are iterating over.
      */
    const symtab_t *stp;

    /**
      * The idx instance variable is used to remember which hash table
      * bucket we are next to walk.
      */
    unsigned long idx;

    /**
      * The rp instance variable is used to remember where we are in
      * walking down the bucket list.
      */
    void *rp;

    /**
      * The valid method is used to determine whether or not this
      * iterator instance is in a valid state.  Used within assert()s.
      */
    bool valid(void) const;
};

#endif // LIB_SYMTAB_ITERATOR_H
// vim: set ts=8 sw=4 et :
