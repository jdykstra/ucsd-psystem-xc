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

#ifndef LIB_LABEL_PLACEBO_H
#define LIB_LABEL_PLACEBO_H

#include <lib/label.h>

/**
  * The label_placebo class is used to represent a label object which
  * does nothing.  Useful for code analysis or manipulations which have
  * no need of labels.
  */
class label_placebo:
    public label
{
public:
    typedef boost::shared_ptr<label_placebo> pointer;

    /**
      * The destructor.
      */
    virtual ~label_placebo();

private:
    /**
      * The default constructor.
      */
    label_placebo();

public:
    static pointer create(void);

public:
    // See base class for documentation.
    void define_here(void);

    // See base class for documentation.
    void goto_from_here(void);

    // See base class for documentation.
    void branch_from_here_if_false(void);

    // See base class for documentation.
    void branch_from_here_if_true(void);

    // See base class for documentation.
    void branch_from_here_if_equal(void);

    // See base class for documentation.
    void branch_from_here_if_not_equal(void);

    // See base class for documentation.
    void self_relative_from_here(void);

    // See base class for documentation.
    bool is_defined(void) const;

private:
    bool defined_flag;

    /**
      * The copy constructor.  Do not use.
      */
    label_placebo(const label_placebo &);

    /**
      * The assignment operator.  Do not use.
      */
    label_placebo &operator=(const label_placebo &);
};

#endif // LIB_LABEL_PLACEBO_H
// vim: set ts=8 sw=4 et :
