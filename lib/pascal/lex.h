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

#ifndef LIB_PASCAL_LEX_H
#define LIB_PASCAL_LEX_H

#include <lib/format_printf.h>
#include <lib/location.h>

/**
  * The pascal_lex_open function is used to open a file for lexical
  * analysis.  Only one file can be open at a time.
  *
  * @param filename
  *     The name of the file to be opened.
  */
void pascal_lex_open(const rcstring &filename);

/**
  * The pascal_grammar_lex function is used by the bison (yacc) generated
  * grammar parser to fetch the next token from the input.
  */
int pascal_grammar_lex(void);

/**
  * The pascal_grammar_error function is used by the bison (yacc) generated
  * grammar parser to report errors, and also other places.
  *
  * @param text
  *     The text of the error message.
  *
  * @note
  *     The function signature is dictaed by Bison.  Use #lex_error
  *     instead, if you want a format string and variable arguments.
  */
void pascal_grammar_error(const char *text);

/**
  * The pascal_lex_error function is used to report errors.
  *
  * @param loc
  *     The position of the error being reported.
  * @param fmt
  *     The format string describing the text to be printed and the rest
  *     of the arguments.  See printf(3) for more information
  */
void pascal_lex_error(const location &loc, const char *fmt, ...)
                                                            FORMAT_PRINTF(2, 3);
/**
  * The pascal_lex_trace_print function is used by the bison (yacc) generated
  * grammar parser to print debugging information (mostly state transitions).
  *
  * @param fmt
  *     The format string describing the text to be printed and the rest
  *     of the arguments.  See printf(3) for more information
  */
void pascal_lex_trace_print(void *, const char *fmt, ...)
                                                            FORMAT_PRINTF(2, 3);
/**
  * The pascal_lex_close function is used to indicate that lexical analysis
  * is complete, and the input file (and other resources) may be
  * released.
  */
void pascal_lex_close(void);

/**
  * The pascal_lex_warning function is used to print warnings to the user,
  * with filename and line number information included.
  *
  * @param fmt
  *     The format string describing the text to be printed and the rest
  *     of the arguments.  See printf(3) for more information
  */
void pascal_lex_warning(const char *fmt, ...)
                                                            FORMAT_PRINTF(1, 2);
/**
  * The pascal_lex_warning function is used to print warnings to the user,
  * with filename and line number information included.
  *
  * @param loc
  *     The position of the error being reported.
  * @param fmt
  *     The format string describing the text to be printed and the rest
  *     of the arguments.  See printf(3) for more information
  */
void pascal_lex_warning(const location &loc, const char *fmt, ...)
                                                            FORMAT_PRINTF(2, 3);
/**
  * The pascal_lex_location function is used to get the current file position.
  */
location pascal_lex_location();

/**
  * The pascal_lex_error_summary function is used to print the error count
  * and exit, if there have been errors.  If there haven't been any, it
  * returns.
  */
void pascal_lex_error_summary();

/**
  * The pascal_lex_include function is used to open an include file.
  *
  * @param filename
  *     The name of the file to include.
  */
void pascal_lex_include(const rcstring &filename);

/**
  * The pascal_lex_include_directory is used to append another directory
  * to the list of directories to search for include files.
  *
  * @param path
  *     The directory to search
  */
void pascal_lex_include_directory(const rcstring &path);

/**
  * The pascal_lex_viewpath is used to append another directory
  * to the list of directories in the view path.
  *
  * @param dir
  *     The directory to search
  */
void pascal_lex_viewpath(const rcstring &dir);

/**
  * The pascal_lex_include_unit function is used to inject text from a
  * unit's interface section.
  *
  * @param code_file_name
  *     The name of the codefile from which the interface text was obtained.
  * @param unit_name
  *     The name of the unit who's interface is being included (the
  *     segment name).
  * @param code_seg
  *     The segment number of the unit's code.
  * @param data_seg
  *     The segment number of the unit's data, or -1 if none.
  * @param segkind
  *     The kind of segment (UNITSEG, LINKED_INTRINS, etc).
  * @param unit_interface_text
  *     The text of the segment's interface section.
  */
void pascal_lex_include_unit(const rcstring &code_file_name,
    const rcstring &unit_name, int code_seg, int data_seg, int segkind,
    const rcstring &unit_interface_text);

/**
  * The pascal_grammar_comment is called whenever there is a comment in
  * the source file.
  *
  * @param text
  *     The text of the comment, with out the delimiters.
  */
void pascal_grammar_comment(const rcstring &text);

/**
  * The pascal_grammar_warnings_are_errors function is used to ask the
  * grammar's context if warnings are errors.
  */
bool pascal_grammar_warnings_are_errors(void);

/**
  * The pascal_grammar_warnings_are_silent function is used to ask the
  * grammar's context if warnings are silent.
  */
bool pascal_grammar_warnings_are_silent(void);

/**
  * The pascal_grammar_type_name function is used to determine whether
  * or not the given name is the name of a type.
  *
  * @param name
  *     The name to test for type-ness
  * @returns
  *     bool; true if the name is a type, false if it is undefined
  *     or defined but not a type.
  */
bool pascal_grammar_type_name(const rcstring &name);

/**
  * The pascal_lex_number_of_lines function is used to obtain the total
  * number of lines processed by the lexer in theis compile run.  Used
  * to print statistics about compile speed.
  */
int pascal_lex_number_of_lines();

/**
  * Integer constants are returned as strings by the lexer.  This
  * allows the pretty printer to accurately reproduce them.  The
  * pascal_lex_integer_constant_to_int function may be used to turn the
  * string representation into a numeric value.  It understands the
  * various radix conventions.
  */
int pascal_lex_integer_constant_to_int(const rcstring &text);

/**
  * The pascal_grammar_complain_about_hex_constant is used to have the
  * grammar's context complain about the use of hex constants.
  */
void pascal_grammar_complain_about_hex_constant(void);

/**
  * The pascal_grammar_complain_about_binary_constant is used to have the
  * grammar's context complain about the use of hex constants.
  */
void pascal_grammar_complain_about_binary_constant(void);

/**
  * The pascal_grammar_fail_hook gives access to the lexical analyzer to
  * the translator::fail_hook method.
  */
void pascal_grammar_fail_hook(void);

/**
  * The pascal_grammar_string_escapes function is used to determine
  * whether or not escape sequences are permitted.  If not, they are
  * just normal string characters.
  */
bool pascal_grammar_string_escapes(void);

/**
  * The pascal_grammar_notify_interface_text function is sued to inform
  * the grammar that an interface text is available.
  *
  * @param text
  *     The text of the interface.
  */
void pascal_grammar_notify_interface_text(const rcstring &text);

#endif // LIB_PASCAL_LEX_H
// vim: set ts=8 sw=4 et :
