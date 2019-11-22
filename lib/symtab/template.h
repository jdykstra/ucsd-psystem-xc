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

#ifndef LIB_SYMTAB_TEMPLATE_H
#define LIB_SYMTAB_TEMPLATE_H

#include <lib/rcstring/list.h>
#include <lib/symtab.h>
#include <lib/symtab/iterator/template.h>

/**
  * The symtab template class is used to provide a type-safe interface
  * to the generic symtab_* API.
  *
  * This template is intended as a <i>very</i> thin veneer over the
  * underlying functionality.  For this reason all of the methods are
  * inline.  If you add more methods, make sure they are also inline.
  */
template <class value_type_t>
class symtab
{
public:
    typedef symtab_iterator<value_type_t> iterator;
    typedef symtab_iterator<const value_type_t> const_iterator;

    /**
      * The destructor.
      * It is not virtual, do not derive from this class.
      */
    ~symtab()
    {
        delete stp;
        stp = 0;
    }

    /**
      * The default constructor.
      */
    symtab() :
        stp(0)
    {
    }

    /**
      * The copy constructor.
      */
    symtab(const symtab &arg) :
        stp(0)
    {
        copy(arg);
    }

    /**
      * The assignment operator.
      */
    symtab &
    operator=(const symtab &arg)
    {
        if (this != &arg)
        {
            clear();
            copy(arg);
        }
        return *this;
    }

    /**
      * The query method is used to locate the given key in the symbol table.
      *
      * @param key
      *     The symbol table entry to look for.
      * @returns
      *     a pointer to the associated data, or NULL if not found
      */
    value_type_t *
    query(const rcstring &key)
        const
    {
        if (!stp)
            return 0;
        return (value_type_t *)stp->query(key);
    }


    /**
      * The lookup method is used to locate the given key in the symbol table.
      * This only works if the vale type has a useful default constructor.
      *
      * @param key
      *     The symbol table entry to look for.
      * @returns
      *     the associated data, or defualt value for the value type.
      */
    value_type_t
    lookup(const rcstring &key)
        const
    {
        value_type_t *vp = query(key);
        return (vp ? *vp : value_type_t());
    }

    /**
      * The query_fuzzy method is used to locate the given key in the
      * symbol table, being generous about what matches.
      *
      * @param key
      *     The symbol table entry to look for.
      * @param best
      *     The weight of the best match so far.
      * @returns
      *     the name of a better match, or the empty string on failure.
      */
    rcstring
    query_fuzzy(const rcstring &key, double &best)
        const
    {
        if (!stp)
            return "";
        return stp->query_fuzzy(key, best);
    }

    /**
      * The query_fuzzy method is used to locate the given key in the
      * symbol table, being generous about what matches.
      *
      * @param key
      *     The symbol table entry to look for.
      * @returns
      *     the name of a better match, or the empty string on failure.
      */
    rcstring
    query_fuzzy(const rcstring &key)
        const
    {
        if (!stp)
            return "";
        return stp->query_fuzzy(key);
    }

    /**
      * The assign method is used to associate a value with a key.
      *
      * @param key
      *     The symbol table entry to set.
      * @param value
      *     The value to assign.  Note that it is always a pointer.
      *     If you have called the set_reaper method, it will have
      *     operator delete called on it (non array) when the symbol
      *     table destructor is run.
      */
    void
    assign(const rcstring &key, value_type_t *value)
    {
        if (!stp)
            stp = new symtab_t(5);
        stp->assign(key, (void *)value);
    }

    /**
      * The assign method is used to associate a value with a key.
      *
      * @param key
      *     The symbol table entry to set.
      * @param value
      *     The value to assign.  A copy will be made in dynamic memory.
      *     You need to call set_reaper to ensure there is no memory
      *     leak when the symbol table destructor is run.
      */
    void
    assign(const rcstring &key, const value_type_t &value)
    {
        // I hope they called set_reaper already.
        assign(key, new value_type_t(value));
    }

    /**
      * The remove method is used to remove a value (and its key) from a
      * symbol table. It is not an error if it has already been removed.
      *
      * @param key
      *     The symbol table entry to look for.
      * @note
      *     If set_reaper has been called, the operator delete will be
      *     called for the value.
      */
    void
    remove(const rcstring &key)
    {
        if (stp)
            stp->remove(key);
    }

    /**
      * The remove method is used to remove several names values and
      * their keys from the symbol table.
      *
      * @param key_list
      *     The symbol table entries to look for.
      */
    void
    remove(const rcstring_list &key_list)
    {
        if (stp)
            stp->remove(key_list);
    }

    /**
      * The dump method is used during debugging to print the contents
      * of a symbol table.
      *
      * @param caption
      *     The heading to print before the contents.
      */
    void
    dump(const char *caption)
        const
    {
        if (stp)
            stp->dump(caption);
    }

    /**
      * The set_reaper method is used to set the reper function of the
      * inner symtab_ty.  This not done by default, because not all
      * symbol tables need their contents deleted when they are deleted.
      */
    void
    set_reaper()
    {
        if (!stp)
            stp = new symtab_t(5);
        stp->set_reap(reaper);
    }

    /**
      * The empty method may be used to determine if there symbol table
      * is empty (i.e. there are no rows).
      */
    bool
    empty()
        const
    {
        return (!stp || stp->empty());
    }

    /**
      * The size method may be used to determine how many rows there are
      * in the symbol table.
      */
    size_t
    size()
        const
    {
        return (stp ? stp->size() : 0);
    }

    /**
      * The keys method may be used to extract the list of row names
      * from the symbol table.
      *
      * \param result
      *     Where to put the row names.  It is cleared before any row
      *     names are placed in it.  It is not sorted.
      *
      * \note
      *     If you have used assign_push method, it is possible to have
      *     duplicates in the list of keys.
      * \note
      *     This method has O(n) execution time.
      */
    void
    keys(rcstring_list &result)
        const
    {
        if (stp)
            stp->keys(result);
        else
            result.clear();
    }

    /**
      * The clear method is used to delete all entries from the symbol table.
      */
    void
    clear()
    {
        if (stp)
            stp->clear();
    }

    /**
      * The begin method is used to obtain an iterator representing the
      * start of the pseudo list of symbols in this symbol tables.
      */
    const_iterator begin() const { return const_iterator(stp, true); }
    iterator begin() { return iterator(stp, true); }

    /**
      * The end method is used to obtain an iterator representing the
      * end of the pseudo list of symbols in this symbol tables.
      */
    const_iterator end() const { return const_iterator(stp, false); }
    iterator end() { return iterator(stp, false); }

private:
    /**
      * The stp instance variable is used to remember the location of
      * the dynamically allocated symbol table.
      */
    symtab_t *stp;

    /**
      * The reaper class methos is used to delete symbol table contents
      * when assigned over, and when the symbol table destructor is run.
      */
    static void reaper(void *p) { delete (value_type_t *)p; }

    /**
      * The copy method is used to copy the contents of one symbol table
      * into another.
      */
    void
    copy(const symtab &arg)
    {
        rcstring_list names;
        arg.keys(names);
        for (size_t j = 0; j < names.size(); ++j)
        {
            value_type_t *p = arg.query(names[j]);
            if (p)
                assign(names[j], *p);
        }
    }
};

#endif // LIB_SYMTAB_TEMPLATE_H
// vim: set ts=8 sw=4 et :
