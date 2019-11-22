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

#ifndef LIB_RCSTRING_LIST_H
#define LIB_RCSTRING_LIST_H

#include <lib/rcstring.h>

class input; // forward
class output; // forward

/**
  * The rcstring_list type is used to represent a list of rcstring
  * values.
  */
class rcstring_list
{
public:
    /**
      * The destructor.
      */
    ~rcstring_list();

    /**
      * The default constructor.  This list will be initialised as
      * being empty.
      */
    rcstring_list();

    /**
      * The copy constructor.
      */
    rcstring_list(const rcstring_list &);

    /**
      * The assignment operator.
      */
    rcstring_list &operator=(const rcstring_list &);

    /**
      * The member method is used to test whether a given string value
      * is a member of the string list.
      *
      * \param arg
      *     The string value to search for.
      * \returns
      *     bool: true if the string is present, false if it is not.
      */
    bool member(const rcstring &arg) const;

    /**
      * The member_nocase method is used to test whether a given string
      * value is a member of the string list.  The conparison is
      * performed in a case-INsensitive way.
      *
      * \param arg
      *     The string value to search for.
      * \returns
      *     bool: true if the string is present, false if it is not.
      */
    bool member_nocase(const rcstring &arg) const;

    /**
      * The gmatch method may be used to determine whether or not at
      * least one string in the list matches the given pattern.
      *
      * @param pattern
      *     The file globbing pattern to search for
      * @returns
      *     bool; true if at least one match, false if no match or error
      */
    bool gmatch(const rcstring &pattern) const;

    /**
      * The size method is used to obtain the number of string in the
      * list.
      */
    size_t size() const { return nstrings; }

    /**
      * The empty method i used to determine if the list is empty
      * (contains no strings) or not.
      *
      * \returns
      *     bool: true if the list contains no strings, false if the
      *     list contains one or more strings.
      */
    bool empty() const { return (nstrings == 0); }

    /**
      * The [] operator may be used to extract a list member.  It may
      * only be used as an r-value.
      *
      * \note
      *     No bounds checking is performed.
      */
    const rcstring &operator[](size_t n) const { return string[n]; }

    /**
      * The unsplit method is used to form a string from a word list.
      *
      * \param start
      *     The first string in the list to start from.
      * \param finish
      *     One past the last string in the list be be used.
      * \param sep
      *     The seapator string between words.  If yoy give NULL, it
      *     will use a single space.
      * \returns
      *     A pointer to the newly formed string in dynamic memory.
      *
      * \note
      *     It is the responsibility of the caller to ensure that the
      *     new string is freed when finished with, by a call to str_free().
      */
    rcstring unsplit(size_t start, size_t finish, const char *sep) const;

    /**
      * The unsplit method is used to form a string from a word list.
      *
      * \param sep
      *     The seapator string between words.  If yoy give NULL, it
      *     will use a single space.
      * \returns
      *     A pointer to the newly formed string in dynamic memory.
      *
      * \note
      *     It is the responsibility of the caller to ensure that the
      *     new string is freed when finished with, by a call to str_free().
      */
    rcstring unsplit(const char *sep = 0) const;

    /**
      * The push_front method is used to prepend a string to the list.
      *
      * \param arg
      *     The string to be appended.
      * \note
      *     This is not terribly efficient, try not to use it too often,
      *     because it has to shuffle all of the string contents up by
      *     one.
      */
    void push_front(const rcstring &arg);

    /**
      * The push_front method is used to prepend a string to the list.
      *
      * \param arg
      *     The list of string to be appended.
      * \note
      *     This is not terribly efficient, try not to use it too often,
      *     because it has to shuffle all of the string contents up first.
      */
    void push_front(const rcstring_list &arg);

    /**
      * The push_back method is used to append a string to the list.
      * This has O(1) insert times.
      *
      * \param arg
      *     The string to be appended.
      */
    void push_back(const rcstring &arg);

    /**
      * The push_back method is used to append a string to the list,
      * but only if it isn't already in the list.
      * This has O(n) insert times.
      *
      * \param arg
      *     The string to be appended.
      */
    void push_back_unique(const rcstring &arg);

    /**
      * The push_back method is used to append all the strings string in
      * one list to the end of this list.  This has O(1) insert times.
      *
      * \param arg
      *     The strings to be appended.
      */
    void push_back(const rcstring_list &arg);

    /**
      * The push_back method is used to append all the strings string in
      * one list to the end of this list.  This has O(n*m) insert times.
      *
      * \param arg
      *     The strings to be appended.
      */
    void push_back_unique(const rcstring_list &arg);

    /**
      * The split method is used to convert a string to a word list.
      * The string list is cleared()ed before the string is split into it.
      *
      * \param arg
      *     The string to be split into pieces.
      * \param sep
      *      Separator characters; sequences of noe or more of these
      *      characters seaprate each part.  Defaults to " " if NULL is
      *      given.
      * \param white
      *      if true, supress extra white space around separators
      * \note
      *     Quoting is not understood.
      */
    void split(const rcstring &arg, const char *sep = 0, bool white = false);

    /**
      * The remove method is used to remove a string.  It is not an
      * error if the string is not present.
      * This has O(n) behaviour.
      *
      * \param arg
      *     The string value to be removed.
      */
    void remove(const rcstring &arg);

    /**
      * The remove method is used to remove the string at the given
      * position in the list.  Everything else shuffles down to fill the
      * gap, which is why this method has O(n) behaviour.
      */
    void remove(size_t n);

    /**
      * The remove method is used to remove a set of string.  It is not
      * an error if one or more of the strings are not present.
      * This has O(n*m) behaviour.
      *
      * \param arg
      *     The string values to be removed.
      */
    void remove(const rcstring_list &arg);

    /**
      * The clear method is used to remove all list elements.  This has
      * O(n) behaviour.  Afterwards, the list is once again empty.
      */
    void clear();

    /**
      * The front method is used to obtain the first string value in the
      * list.  If the string list is empty, the empty string is returned.
      */
    const rcstring &
    front()
    {
        return (nstrings ? string[0] : nothing);
    }

    /**
      * The back method is used to obtain the last string value in the
      * list.
      */
    const rcstring &
    back()
    {
        return (nstrings ? string[nstrings - 1] : nothing);
    }

    /**
      * The pop_front method is used to discard the first value in the list.
      * This has O(n) behaviour.
      */
    void pop_front();

    /**
      * The pop_back method is used to discard the last value in the list.
      * This has O(1) behaviour.
      */
    void pop_back();

    /**
      * The equal method is used to determine if this string list is
      * equal to another string list.  Two lists are considered equal if
      * they both contains the same strings, regardless of order.
      *
      * \returns
      *     bool: true if equal, false if not
      */
    bool equal(const rcstring_list &arg) const;

    /**
      * The subset method is used to determine if this string list is
      * a subset of another string list, regardless of order.
      *
      * \param arg
      *     strings list to test against,
      *     i.e. that is (*this is-a-subset-of arg).
      * \returns
      *     bool: true if subset, false if not
      *
      * \note
      *     By subset, this also includes improper subsets (equality).
      */
    bool subset(const rcstring_list &arg) const;

    /**
      * The sort method is used to perform an <i>in situ</i> sort the
      * string list values in a string list.  The comparison function
      * used is strcmp.
      */
    void sort();

    /**
      * The sort_nocase method is used to perform an <i>in situ</i>
      * sort the string list values in a string list.  The comparison
      * function used is strcasecmp.
      */
    void sort_nocase();

    /**
      * The sort_version method is used to perform an <i>in situ</i>
      * sort the string list values in a string list.  The comparison
      * function used is strverscmp.
      */
    void sort_version();

    /**
      * The sort_longest_to_shortest method is used to perform an <i>in
      * situ</i> sort the string list values in a string list, from the
      * longest string to the shortest string.
      */
    void sort_longest_to_shortest();

    /**
      * The quote_shell method is used to produce a new string list by
      * quoting the strings of this string list.
      */
    rcstring_list quote_shell() const;

    /**
      * The validate method is used to validate a string list.
      * Usually used for debugging, usually with assert.
      *
      * \returns
      *     bool: true if the string list is valis, false if not.
      */
    bool validate() const;

    /**
      * The intersection method is used to calculate the set
      * intersection between this set of strings and the rhs set of
      * strings.
      */
    rcstring_list intersection(const rcstring_list &rhs) const;

    /**
      * The exclusive_or method is used to calculate the set of strings
      * on the left or on the right, but not both.  Duplicates are
      * omitted.
      */
    rcstring_list exclusive_or(const rcstring_list &rhs) const;

    /**
      * The tail method is used to create a new string list from this
      * string list, without the first element.
      */
    rcstring_list tail(void) const;

private:
    /**
      * The nstrings instance variable is used to remember how many
      * strings are present in the list.
      */
    size_t nstrings;

    /**
      * The nstrings_max instance variable is used to remember how many
      * elements are have been allocated to the list.
      */
    size_t nstrings_max;

    /**
      * The string instance variable is used to remember the base of a
      * dynamically allocated array used to hold pointers to the strings
      * in the list.
      */
    rcstring *string;

    /**
      * The nothing class variable is used when we have to return a
      * reference to an empty string.
      */
    static rcstring nothing;
};

/**
  * The quality (==) operator is used to determine if two string lists
  * contain the same strings.  The _ordering_ of the strings is not
  * considered.  This is an O(n**2) operation.
  *
  * @return
  *     Returns true if the two sets of strings are the same,
  *     false if they are not.
  */
inline bool
operator==(const rcstring_list &lhs, const rcstring_list &rhs)
{
    return lhs.equal(rhs);
}


/**
  * The inquality (!=) operator is used to determine if two string lists
  * do not contain the same strings.  The _ordering_ of the strings is
  * not considered.  This is an O(n**2) operation.
  *
  * @return
  *     Returns true if the two sets of strings are different, false if
  *     they are the same.
  */
inline bool
operator!=(const rcstring_list &lhs, const rcstring_list &rhs)
{
    return !lhs.equal(rhs);
}

/**
  * The in situ addition operator is used to union values into a set of
  * strings.  Duplicates on the right will be omitted.
  */
inline const rcstring_list &
operator+=(rcstring_list &lhs, const rcstring_list &rhs)
{
    lhs.push_back_unique(rhs);
    return lhs;
}

/**
  * The addition operator is used to union two sets of strings.
  * Duplicates will be omitted.
  */
inline rcstring_list
operator+(const rcstring_list &lhs, const rcstring_list &rhs)
{
    rcstring_list result;
    result.push_back_unique(lhs);
    result.push_back_unique(rhs);
    return result;
}

/**
  * The in situ subtract operator is used to difference two sets of
  * strings by removing strings from the left which appear in the right.
  */
inline const rcstring_list &
operator-=(rcstring_list &lhs, const rcstring_list &rhs)
{
    lhs.remove(rhs);
    return lhs;
}

/**
  * The subtract operator is used to create a new set of strings
  * by removing strings from the left which appear in the right.
  */
inline rcstring_list
operator-(const rcstring_list &lhs, const rcstring_list &rhs)
{
    rcstring_list result;
    result.push_back_unique(lhs);
    result.remove(rhs);
    return result;
}

/**
  * The in situ multiply operator is used to keep only those strings on
  * the left which also appear on the right.  Duplicates are omitted.
  */
inline const rcstring_list &
operator*=(rcstring_list &lhs, const rcstring_list &rhs)
{
    lhs = lhs.intersection(rhs);
    return lhs;
}

/**
  * The multiply operator is used to calculate the set intersection of
  * the left and right.  Duplicates are omitted.
  */
inline rcstring_list
operator*(const rcstring_list &lhs, const rcstring_list &rhs)
{
    return lhs.intersection(rhs);
}

/**
  * The in situ exclusive-or operator is used to keep only those strings
  * on the left or on the right, but not both.  Duplicates are omitted.
  */
inline const rcstring_list &
operator^=(rcstring_list &lhs, const rcstring_list &rhs)
{
    lhs = lhs.exclusive_or(rhs);
    return lhs;
}

/**
  * The exclusive-or operator is used to calculate the set of strings on
  * the left or on the right, but not both.  Duplicates are omitted.
  */
inline rcstring_list
operator^(const rcstring_list &lhs, const rcstring_list &rhs)
{
    return lhs.exclusive_or(rhs);
}

#endif // LIB_RCSTRING_LIST_H
// vim: set ts=8 sw=4 et :
