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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_REAL_PRETTY_H
#define UCSDPSYS_PRETTY_EXPRESSION_REAL_PRETTY_H

#include <lib/expression/real.h>

class translator_pretty; // forward

/**
  * The expression_real_pretty class is used to represent
  * a real constant expression node pretty printer.
  */
class expression_real_pretty:
    public expression_real
{
public:
    typedef boost::shared_ptr<expression_real_pretty> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_real_pretty();

private:
    /**
      * The constructor.
      */
    expression_real_pretty(const location &locn, const rcstring &value,
        translator_pretty *tp);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const location &locn, const rcstring &value,
        translator_pretty *tp);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The pretty instance valiable is used to remember our enclosing
      * translator, for things like page widths and indent levels.
      */
    translator_pretty &pretty;

    /**
      * The original instance variable is used to remember the original
      * text of the real constant, just as it was written in the
      * source code.
      */
    rcstring original;

    /**
      * The default constructor.  Do not use.
      */
    expression_real_pretty();

    /**
      * The copy constructor.  Do not use.
      */
    expression_real_pretty(const expression_real_pretty &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_real_pretty &operator=(const expression_real_pretty &);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_REAL_PRETTY_H
// vim: set ts=8 sw=4 et :
