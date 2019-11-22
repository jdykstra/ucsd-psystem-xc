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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_SCAN_PRETTY_H
#define UCSDPSYS_PRETTY_EXPRESSION_SCAN_PRETTY_H

#include <lib/expression/scan.h>

class translator_pretty; // forward

/**
  * The expression_scan_pretty class is used to represent the processing
  * necessary to pretty print the weird SCAN function call with the
  * embedded non-syntactic "=" or "<>" comparison.
  */
class expression_scan_pretty:
    public expression_scan
{
public:
    typedef boost::shared_ptr<expression_scan_pretty> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_scan_pretty();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param maxdisp
      *     How far to scan; backwards if negative, forwards if positive.
      * @param mask
      *     Scan for equality if 0, inequality if 1.
      * @param value
      *     The character value to scan for.
      * @param start
      *     Expression evaluating to a byte pointer (reference), scan
      *     starting position.
      * @param pretty
      *     The translation context we are working within.
      */
    static pointer create(const expression::pointer &maxdisp, int mask,
        const expression::pointer &value, const expression::pointer &start,
        translator_pretty *pretty);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param maxdisp
      *     How far to scan; backwards if negative, forwards if positive.
      * @param mask
      *     Scan for equality if 0, inequality if 1.
      * @param value
      *     The character value to scan for.
      * @param start
      *     Expression evaluating to a byte pointer (reference), scan
      *     starting position.
      * @param pretty
      *     The translation context we are working within.
      */
    expression_scan_pretty(const expression::pointer &maxdisp, int mask,
        const expression::pointer &value, const expression::pointer &start,
        translator_pretty *pretty);

    /**
      * The pretty instance variable is used to remember the translation
      * context we are working within.
      */
    translator_pretty &pretty;

    /**
      * The default constructor.  Do not use.
      */
    expression_scan_pretty();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_scan_pretty(const expression_scan_pretty &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_scan_pretty &operator=(const expression_scan_pretty &rhs);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_SCAN_PRETTY_H
// vim: set ts=8 sw=4 et :
