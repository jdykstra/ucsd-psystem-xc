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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_STRING_PRETTY_H
#define UCSDPSYS_PRETTY_EXPRESSION_STRING_PRETTY_H

#include <lib/expression/string.h>

class translator_littoral; // forward

/**
  * The expression_string_littoral class is used to represent
  * a string constant expression node littoral printer.
  */
class expression_string_littoral:
    public expression_string
{
public:
    typedef boost::shared_ptr<expression_string_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_string_littoral();

private:
    /**
      * The constructor.
      */
    expression_string_littoral(const location &locn, const rcstring &value,
        translator_littoral *tp);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const location &locn, const rcstring &value,
        translator_littoral *tp);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    expression::pointer optimize() const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The littoral instance valiable is used to remember our enclosing
      * translator, for things like page widths and indent levels.
      */
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    expression_string_littoral();

    /**
      * The copy constructor.  Do not use.
      */
    expression_string_littoral(const expression_string_littoral &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_string_littoral &operator=(const expression_string_littoral &);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_STRING_PRETTY_H
// vim: set ts=8 sw=4 et :
