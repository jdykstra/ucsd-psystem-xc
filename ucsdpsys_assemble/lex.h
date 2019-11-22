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

#ifndef UCSDPSYS_ASSEMBLE_LEX_H
#define UCSDPSYS_ASSEMBLE_LEX_H

#include <lib/expression.h>
#include <lib/format_printf.h>
#include <lib/location.h>
#include <lib/rcstring.h>

#include <ucsdpsys_assemble/opcode.h>

extern int grammar_debug;

void lex_open(const rcstring &filename);
void lex_open_include(const rcstring &filename);
void lex_close(void);
void lex_error(const location &locn, const char *fmt, ...)  FORMAT_PRINTF(2, 3);
void lex_warning(const char *fmt, ...) FORMAT_PRINTF(1, 2);
void lex_message(const location &loc, const char *fmt, ...) FORMAT_PRINTF(2, 3);
location lex_location(void);

void grammar_error(const char *fmt, ...);
void grammar_warning(const char *fmt, ...);
int grammar_lex(void);
opcode::pointer grammar_lookup_opcode(const rcstring &name);
expression::pointer grammar_lookup_special(const rcstring &name);

/**
  * The lex_trace_print function is used by the bison (yacc) generated
  * grammar parser to print debugging information (mostly state
  * transitions).
  *
  * @param fmt
  *     The format string describing the text to be printed and the rest
  *     of the arguments.  See printf(3) for more information
  */
void lex_trace_print(void *, const char *fmt, ...)          FORMAT_PRINTF(2, 3);

/**
  * The lex_default_radix_set function is called to set the lexer's
  * default number radix.  This is altered by the .arch and .radix
  * pseudo-ops
  *
  * @param n
  *     The new radix.
  *     assert(n >= 2);
  *     assert(n <= 26);
  */
void lex_default_radix_set(int n);

/**
  * The lex_include_path function is used to append another directory to
  * the lexer's include search path.
  *
  * @param path
  *     The directory to search.
  */
void lex_include_path(const rcstring &path);

/**
  * The lex_view_path function is used to append another directory to
  * the lexer's view path.
  *
  * @param path
  *     The directory to search.
  */
void lex_view_path(const rcstring &path);

class rcstring_list; // forward

/**
  * The lex_macro_slurp function is used to parse a macro body from the
  * input stream.
  *
  * @param contents
  *     Where to put the contents of the macro (one line per string)
  *     once it has been read.
  */
void lex_macro_slurp(rcstring_list &contents);

/**
  * The lex-macro_insert function is used to insert the given text into
  * the input stream, when implementing macro body expansion.
  *
  * @param text
  *     The text to be inserted.  It must either be empty, or end with a
  *     newline.  There is no guarantee the text is syntactically valid.
  */
void lex_macro_insert(const rcstring &text);

/**
  * The lex_conditional_push function is used to push a boolean onto the
  * conditional stack.  This is in response to a .IF pseudo-op.
  *
  * @param yesno
  *     True if conditional is emabled, false if disabled.
  */
void lex_conditional_push(bool yesno);

/**
  * The lex_discard_input function is used to discard the remaining
  * input, once a valid .END is seen by the grammar.
  */
void lex_discard_input(void);

#endif // UCSDPSYS_ASSEMBLE_LEX_H
// vim: set ts=8 sw=4 et :
