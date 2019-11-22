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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_STORE_INDIRECT_PACKED_H
#define UCSDPSYS_COMPILE_EXPRESSION_STORE_INDIRECT_PACKED_H

#include <ucsdpsys_compile/expression/store.h>

class translator_compile; // forward

/**
  * The expression_store_indirect_packed class is used to represent the code
  * generation for storing through a bit field pointer indirection.
  */
class expression_store_indirect_packed:
    public expression_store
{
public:
    typedef boost::shared_ptr<expression_store_indirect_packed> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_store_indirect_packed();

private:
    /**
      * The constructor.
      */
    expression_store_indirect_packed(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *cntxt);

protected:
    // See base class for documentation.
    void pre_order_traversal() const;

    // See base class for documentation.
    void post_order_traversal() const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The lhs instance variable is used to remember the packed address
      * (48 bits) of the field to be stored into.
      */
    expression::pointer lhs;

    /**
      * The default constructor.
      */
    expression_store_indirect_packed();

    /**
      * The copy constructor.
      */
    expression_store_indirect_packed(const expression_store_indirect_packed &);

    /**
      * The assignment operator.
      */
    expression_store_indirect_packed &operator=(
        const expression_store_indirect_packed &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_STORE_INDIRECT_PACKED_H
// vim: set ts=8 sw=4 et :
