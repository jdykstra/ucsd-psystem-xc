//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
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

#ifndef UCSDPSYS_PRETTY_SCOPE_FUNCTION_PRETTY_H
#define UCSDPSYS_PRETTY_SCOPE_FUNCTION_PRETTY_H

#include <lib/scope/function.h>

class translator_pretty; // forward

/**
  * The scope_function_pretty class is used to represent a pretty prining
  * of the outer-most function scope.
  */
class scope_function_pretty:
    public scope_function
{
public:
    typedef boost::shared_ptr<scope_function_pretty> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_function_pretty();

    /**
      * The create class method is used to create dynamically allocated
      * instances of this class.
      */
    static pointer create(const rcstring &name, bool is_func, bool is_seg,
        translator_pretty *cntxt);

protected:
    // See base class for documentation.
    void begin_notify(void);

    // See base class for documentation.
    void pre_order_traversal(void) const;

    // See base class for documentation.
    void post_order_traversal(void) const;

    // See base class for documentation.
    void notify_function_parameters(const name_type_list &ntl);

    // See base class for documentation.
    void constant_declaration_notify(const variable_name &name,
        const expression::pointer &value);

    // See base class for documentation.
    void type_declaration_notify(const variable_name &name,
        const type::pointer &tp);

    // See base class for documentation.
    void variable_declaration_notify(const symbol_variable_p &svp);

    // See base class for documentation.
    symbol::pointer constant_factory(const variable_name &name,
        int lex_level, const expression::pointer &value);

    // See base class for documentation.
    symbol_variable::pointer variable_factory(const variable_name &name,
        const type::pointer &tp, int lex_level);

    // See base class for documentation.
    symbol_variable::pointer parameter_factory(const variable_name &name,
        const type::pointer &tp, int lex_level);

private:
    /**
      * The constructor.
      */
    scope_function_pretty(const rcstring &name, bool is_func, bool is_seg,
        translator_pretty *cntxt);

    /**
      * The pretty instance variable is used to remember where to print
      * our textual output.
      */
    translator_pretty &pretty;

    bool print_name_needed;
    void print_name(void);
    bool print_lp_needed;
    void print_lp(void);
    bool print_rp_needed;
    bool print_return_type_needed;
    void print_return_type(void);
    bool print_const_needed;
    void print_const(void);
    bool print_type_needed;
    void print_type(void);
    bool print_var_needed;
    void print_var(void);

    /**
      * The default constructor.  Do not use.
      */
    scope_function_pretty();

    /**
      * The copy constructor.  Do not use.
      */
    scope_function_pretty(const scope_function_pretty &);

    /**
      * The assignment operator.  Do not use.
      */
    scope_function_pretty &operator=(const scope_function_pretty &);
};

#endif // UCSDPSYS_PRETTY_SCOPE_FUNCTION_PRETTY_H
// vim: set ts=8 sw=4 et :
