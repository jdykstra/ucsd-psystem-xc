//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef LIB_RCSTRING_GIZZARDS_H
#define LIB_RCSTRING_GIZZARDS_H

#include <lib/ac/stddef.h>

/**
  * @struct rcstring_gizzards
  * The rcstring_gizzards struct is used to remember information about
  * strings.  Users should always refer to strings as @em pointers.
  * The only members within the struct that should be accessed are the
  * rcstring_gizzards_text member, which is a NUL terminated array of
  * characters, and the rcstring_gizzards_length member, which contains
  * the length of the string's text (not including the NUL terminator).
  *
  * It is guaranteed that all equal strings will not only have the same
  * hash, but will, if fact, be exactly the same sting.  The reference
  * count is how many strings currently exist with this value.  Thus,
  * the commonest string test, a string equality test becomes a pointer
  * equality test.
  *
  * <b>Thou shalt not</b> modify any member of a string struct, or the
  * forces of evil shall wreak misery in thy life.
  */
struct rcstring_gizzards
{
    typedef unsigned long hash_t;

    /**
      * The hash of the string.  Used internally by the string table,
      * users shall never access this member.
      */
    hash_t rcstring_gizzards_hash;

    /**
      * The next string in a hash bucket.  Used internally by the string
      * table, users shall never access this member.
      */
    rcstring_gizzards *rcstring_gizzards_next;

    /**
      * The reference count for this string.  It is guaranteed that
      * all equal strings will not only have the same hash, but will,
      * if fact, be exactly the same sting.  The reference count is how
      * many strings currently exist with this value.  Used internally
      * by the string table, users shall never access this member.
      */
    long rcstring_gizzards_references;

    /**
      * The length of the string (not including the terminating NUL).
      * Read-only access permitted to users.
      */
    size_t rcstring_gizzards_length;

    /**
      * The text value of the string.  It's actually longer than its
      * declaration when the string length is more than zero.
      * Read-only access permitted to users.
      */
    char rcstring_gizzards_text[1];

    /**
      * @brief
      *     check that the string is valid
      *
      * The rcstring_gizzards_validate function is used to confirm that the
      * given string pointer, @a str, points to a valid string.  Usually
      * used for debugging, often in assert()s.
      *
      * @return
      *     Non-zero if valid, zero if not valid.
      */
    bool validate() const;
};

/**
  * @brief
  *     make string from C string
  *
  * The rcstring_gizzards_from_c function is used to make a string from
  * a null terminated C string.
  *
  * @return
  *     a pointer to a string in dynamic memory.  Use
  *     rcstring_gizzards_free() when finished with.  The contents of
  *     the structure pointed to <b>shall not</b> be altered.
  */
rcstring_gizzards *rcstring_gizzards_from_c(const char *str);

/**
  * @brief
  *     make string from C string
  *
  * The rcstring_gizzards_n_from_c function is used to make a string
  * from an array of characters.  No null terminator is assumed.
  *
  * @param str
  *     The C string to be copied.  Will not be modified.
  * @param len
  *     The maximum number of characters to be used (fewer will be used
  *     if there is an included NUL).
  *
  * @return
  *     a pointer to a string in dynamic memory.  Use
  *     rcstring_gizzards_free() when finished with.  The contents of
  *     the structure pointed to <b>shall not</b> be altered.
  */
rcstring_gizzards *rcstring_gizzards_n_from_c(const char *str, size_t len);

/**
  * @brief
  *     make string from C string
  *
  * The rcstring_gizzards_n_from_c function is used to make a string
  * from an array of characters.  No null terminator is assumed.
  *
  * @param str
  *     The C string to be copied.  Will not be modified.
  * @param len
  *     The maximum number of characters to be used (fewer will be used
  *     if there is an included NUL).
  *
  * @return
  *     a pointer to a string in dynamic memory.  Use
  *     rcstring_gizzards_free() when finished with.  The contents of
  *     the structure pointed to <b>shall not</b> be altered.
  */
rcstring_gizzards *rcstring_gizzards_n_from_c(const unsigned char *str,
    size_t len);

/**
  * @brief
  *     make a copy of a string
  *
  * The rcstring_gizzards_copy function is used to make a copy of a string.
  *
  * @param str
  *     The string to be copied.  Will not be modified.
  *
  * @return
  *     a pointer to a string in dynamic memory.  Use
  *     rcstring_gizzards_free() when finished with.  The contents of
  *     the structure pointed to <b>shall not</b> be altered.
  */
rcstring_gizzards *rcstring_gizzards_copy(rcstring_gizzards *str);

/**
  * @brief
  *     release a string
  *
  * The rcstring_gizzards_free function is used to indicate that a
  * string hash been finished with.  This is the only way to release
  * strings. <b>Do not</b> use the free() function or the delete operator.
  *
  * @param str
  *     The string to be freed.
  */
void rcstring_gizzards_free(rcstring_gizzards *str);

/**
  * @brief
  *     dump the string table
  *
  * The rcstring_gizzards_dump function is used to dump the contents of
  * the string table to the standard error.  Only useful for debugging.
  */
void rcstring_gizzards_dump(void);

/**
  * @brief
  *     test string equality
  * @see
  *     rcstring_gizzards_equal()
  *
  * The rcstring_gizzards_equal macro is used to accellerate string
  * equality tests.  Users shall always write code as if they did not
  * know that a string equality test is a pointer equality test.
  */
inline bool
rcstring_gizzards_equal(const rcstring_gizzards *s1,
    const rcstring_gizzards *s2)
{
    return (s1 == s2);
}

#endif // LIB_RCSTRING_GIZZARDS_H
// vim: set ts=8 sw=4 et :
