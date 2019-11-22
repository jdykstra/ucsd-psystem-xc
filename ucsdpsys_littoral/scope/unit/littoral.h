//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
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

#ifndef UCSDPSYS_PRETTY_SCOPE_UNIT_PRETTY_H
#define UCSDPSYS_PRETTY_SCOPE_UNIT_PRETTY_H

#include <lib/scope/unit.h>
#include <lib/name_type/list.h>

class translator_littoral; // forward

/**
  * The scope_unit_littoral class is used to represent a littoral prining
  * of the outer-most UNIT scope.
  */
class scope_unit_littoral:
    public scope_unit
{
public:
    typedef boost::shared_ptr<scope_unit_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_unit_littoral();

    /**
      * The create class method is used to create dynamically allocated
      * instances of this class.
      */
    static pointer create(const rcstring &name, int a_code_seg, int a_data_seg,
        int llht, translator_littoral *cntxt);

protected:
    // See base class for documentation.
    void begin_notify(void);

    // See base class for documentation.
    void pre_order_traversal(void) const;

    // See base class for documentation.
    void post_order_traversal(void) const;

    // See base class for documentation.
    void constant_declaration_notify(const variable_name &name,
        const expression::pointer &value);

    // See base class for documentation.
    void type_declaration_notify(const variable_name &name,
        const type::pointer &tp);

    // See base class for documentation.
    void variable_declaration_notify(const symbol_variable_p &svp);

    // See base class for documentation.
    void label_declaration_notify(const variable_name_list &names);

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
    scope_unit_littoral(const rcstring &name, int a_code_seg, int a_data_seg,
        int llht, translator_littoral *cntxt);

    /**
      * The littoral instance variable is used to remember where to print
      * our textual output.
      */
    translator_littoral &littoral;

    /**
      * The have_printed_const_keyword instance variable is used to
      * remember whether ot not we have printed the "const" keyword,
      * when printing constant declarations.
      */
    bool have_printed_const_keyword;

    /**
      * The have_printed_type_keyword instance variable is used to
      * remember whether ot not we have printed the "type" keyword, when
      * printing type declarations.
      */
    bool have_printed_type_keyword;

    /**
      * The have_printed_var_keyword instance variable is used to
      * remember whether ot not we have printed the "var" keyword, when
      * printing variable declarations.
      */
    bool have_printed_var_keyword;

    /**
      * The default constructor.  Do not use.
      */
    scope_unit_littoral();

    /**
      * The copy constructor.  Do not use.
      */
    scope_unit_littoral(const scope_unit_littoral &);

    /**
      * The assignment operator.  Do not use.
      */
    scope_unit_littoral &operator=(const scope_unit_littoral &);
};

#endif // UCSDPSYS_PRETTY_SCOPE_UNIT_PRETTY_H
// vim: set ts=8 sw=4 et :
