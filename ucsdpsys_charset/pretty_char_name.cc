//
// UCSD p-System cross compiler
// Copyright (C) 2012 Peter Miller
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

#include <lib/ac/ctype.h>
#include <lib/ac/stdio.h>
#include <lib/ac/string.h>

#include <ucsdpsys_charset/pretty_char_name.h>


rcstring
pretty_char_name(int ic)
{
    if (ic == EOF)
        return "EOF";
    if (ic < 0 || ic > 0x100)
        return rcstring::printf("%d", ic);
    unsigned char c(ic);
    if (c == 0)
        return "'\\0'";
    if (isprint(c))
    {
        if (c == '\'' || c == '\\')
            return rcstring::printf("'\\%c'", c);
        return rcstring::printf("'%c'", c);
    }
    static char escape[] = "\aa\bb\ff\nn\rr\tt\vv";
    const char *esc = strchr(escape, c);
    if (esc)
        return rcstring::printf("'\\%c'", esc[1]);
    return rcstring::printf("%d", c);
}


rcstring
unicode_char_name(int ic)
{
    if (ic == EOF)
        return "EOF";
    if (ic < 0 || ic > 0x100)
        return rcstring::printf("%d", ic);
    unsigned char c(ic);

    //
    // Provenance: derived from
    // http://www.unicode.org/Public/UNIDATA/NamesList.txt
    //
    static const char *const names[0x100] =
    {
        "NUL", // 0x00
        "SOH, START OF HEADING", // 0x01
        "SOT, START OF TEXT", // 0x02
        "END OF TEXT", // 0x03
        "END OF TRANSMISSION", // 0x04
        "ENQUIRY", // 0x05
        "ACK, ACKNOWLEDGE", // 0x06
        "BELL", // 0x07
        "BS, BACKSPACE", // 0x08
        "TAB, CHARACTER TABULATION", // 0x09
        "LF, LINE FEED", // 0x0A
        "VT, vertical tabulation", // 0x0B
        "FF, FORM FEED", // 0x0C
        "CR, CARRIAGE RETURN", // 0x0D
        "SI, SHIFT OUT", // 0x0E
        "SO, SHIFT IN", // 0x0F
        "DLE, DATA LINK ESCAPE", // 0x10
        "DC1, DEVICE CONTROL ONE", // 0x11
        "DC2, DEVICE CONTROL TWO", // 0x12
        "DC3, DEVICE CONTROL THREE", // 0x13
        "DC4, DEVICE CONTROL FOUR", // 0x14
        "NEGATIVE ACKNOWLEDGE", // 0x15
        "SYNCHRONOUS IDLE", // 0x16
        "END OF TRANSMISSION BLOCK", // 0x17
        "CANCEL", // 0x18
        "END OF MEDIUM", // 0x19
        "SUBSTITUTE", // 0x1A
        "ESC, ESCAPE", // 0x1B
        "INFORMATION SEPARATOR FOUR", // 0x1C
        "INFORMATION SEPARATOR THREE", // 0x1D
        "RS, INFORMATION SEPARATOR TWO", // 0x1E
        "US, INFORMATION SEPARATOR ONE", // 0x1F
        "SPACE", // 0x20
        "EXCLAMATION MARK", // 0x21
        "QUOTATION MARK", // 0x22
        "HASH", // 0x23
        "DOLLAR SIGN", // 0x24
        "PERCENT SIGN", // 0x25
        "AMPERSAND", // 0x26
        "APOSTROPHE", // 0x27
        "LEFT PARENTHESIS", // 0x28
        "RIGHT PARENTHESIS", // 0x29
        "ASTERISK", // 0x2A
        "PLUS SIGN", // 0x2B
        "COMMA", // 0x2C
        "HYPHEN, MINUS", // 0x2D
        "DOT, FULL STOP, period, decimal point", // 0x2E
        "SLASH, SOLIDUS, virgule", // 0x2F
        "DIGIT ZERO", // 0x30
        "DIGIT ONE", // 0x31
        "DIGIT TWO", // 0x32
        "DIGIT THREE", // 0x33
        "DIGIT FOUR", // 0x34
        "DIGIT FIVE", // 0x35
        "DIGIT SIX", // 0x36
        "DIGIT SEVEN", // 0x37
        "DIGIT EIGHT", // 0x38
        "DIGIT NINE", // 0x39
        "COLON", // 0x3A
        "SEMICOLON", // 0x3B
        "LESS-THAN SIGN", // 0x3C
        "EQUALS SIGN", // 0x3D
        "GREATER-THAN SIGN", // 0x3E
        "QUESTION MARK", // 0x3F
        "AT, COMMERCIAL", // 0x40
        "LATIN CAPITAL LETTER A", // 0x41
        "LATIN CAPITAL LETTER B", // 0x42
        "LATIN CAPITAL LETTER C", // 0x43
        "LATIN CAPITAL LETTER D", // 0x44
        "LATIN CAPITAL LETTER E", // 0x45
        "LATIN CAPITAL LETTER F", // 0x46
        "LATIN CAPITAL LETTER G", // 0x47
        "LATIN CAPITAL LETTER H", // 0x48
        "LATIN CAPITAL LETTER I", // 0x49
        "LATIN CAPITAL LETTER J", // 0x4A
        "LATIN CAPITAL LETTER K", // 0x4B
        "LATIN CAPITAL LETTER L", // 0x4C
        "LATIN CAPITAL LETTER M", // 0x4D
        "LATIN CAPITAL LETTER N", // 0x4E
        "LATIN CAPITAL LETTER O", // 0x4F
        "LATIN CAPITAL LETTER P", // 0x50
        "LATIN CAPITAL LETTER Q", // 0x51
        "LATIN CAPITAL LETTER R", // 0x52
        "LATIN CAPITAL LETTER S", // 0x53
        "LATIN CAPITAL LETTER T", // 0x54
        "LATIN CAPITAL LETTER U", // 0x55
        "LATIN CAPITAL LETTER V", // 0x56
        "LATIN CAPITAL LETTER W", // 0x57
        "LATIN CAPITAL LETTER X", // 0x58
        "LATIN CAPITAL LETTER Y", // 0x59
        "LATIN CAPITAL LETTER Z", // 0x5A
        "LEFT SQUARE BRACKET", // 0x5B
        "backslash, REVERSE SOLIDUS", // 0x5C
        "RIGHT SQUARE BRACKET", // 0x5D
        "carat, CIRCUMFLEX ACCENT", // 0x5E
        "underscore, LOW LINE", // 0x5F
        "GRAVE ACCENT, back tick", // 0x60
        "LATIN SMALL LETTER A", // 0x61
        "LATIN SMALL LETTER B", // 0x62
        "LATIN SMALL LETTER C", // 0x63
        "LATIN SMALL LETTER D", // 0x64
        "LATIN SMALL LETTER E", // 0x65
        "LATIN SMALL LETTER F", // 0x66
        "LATIN SMALL LETTER G", // 0x67
        "LATIN SMALL LETTER H", // 0x68
        "LATIN SMALL LETTER I", // 0x69
        "LATIN SMALL LETTER J", // 0x6A
        "LATIN SMALL LETTER K", // 0x6B
        "LATIN SMALL LETTER L", // 0x6C
        "LATIN SMALL LETTER M", // 0x6D
        "LATIN SMALL LETTER N", // 0x6E
        "LATIN SMALL LETTER O", // 0x6F
        "LATIN SMALL LETTER P", // 0x70
        "LATIN SMALL LETTER Q", // 0x71
        "LATIN SMALL LETTER R", // 0x72
        "LATIN SMALL LETTER S", // 0x73
        "LATIN SMALL LETTER T", // 0x74
        "LATIN SMALL LETTER U", // 0x75
        "LATIN SMALL LETTER V", // 0x76
        "LATIN SMALL LETTER W", // 0x77
        "LATIN SMALL LETTER X", // 0x78
        "LATIN SMALL LETTER Y", // 0x79
        "LATIN SMALL LETTER Z", // 0x7A
        "LEFT CURLY BRACKET, left brace", // 0x7B
        "VERTICAL LINE, vertical bar", // 0x7C
        "RIGHT CURLY BRACKET, right brace", // 0x7D
        "TILDE", // 0x7E
        "DEL, DELETE", // 0x7F
        NULL, // 0x80
        NULL, // 0x81
        "BREAK PERMITTED HERE", // 0x82
        "NO BREAK HERE", // 0x83
        "formerly known as INDEX", // 0x84
        "NEL, NEXT LINE", // 0x85
        "START OF SELECTED AREA", // 0x86
        "END OF SELECTED AREA", // 0x87
        "CHARACTER TABULATION SET", // 0x88
        "CHARACTER TABULATION WITH JUSTIFICATION", // 0x89
        "LINE TABULATION SET", // 0x8A
        "PARTIAL LINE FORWARD", // 0x8B
        "PARTIAL LINE BACKWARD", // 0x8C
        "REVERSE LINE FEED", // 0x8D
        "SINGLE SHIFT TWO", // 0x8E
        "SINGLE SHIFT THREE", // 0x8F
        "DEVICE CONTROL STRING", // 0x90
        "PRIVATE USE ONE", // 0x91
        "PRIVATE USE TWO", // 0x92
        "SET TRANSMIT STATE", // 0x93
        "CANCEL CHARACTER", // 0x94
        "MESSAGE WAITING", // 0x95
        "START OF GUARDED AREA", // 0x96
        "END OF GUARDED AREA", // 0x97
        "START OF STRING", // 0x98
        NULL, // 0x99
        "SINGLE CHARACTER INTRODUCER", // 0x9A
        "CONTROL SEQUENCE INTRODUCER", // 0x9B
        "STRING TERMINATOR", // 0x9C
        "OPERATING SYSTEM COMMAND", // 0x9D
        "PRIVACY MESSAGE", // 0x9E
        "APPLICATION PROGRAM COMMAND", // 0x9F
        "nbsp, NO-BREAK SPACE", // 0xA0
        "INVERTED EXCLAMATION MARK", // 0xA1
        "CENT SIGN", // 0xA2
        "POUND SIGN", // 0xA3
        "CURRENCY SIGN", // 0xA4
        "YEN SIGN, yuan sign", // 0xA5
        "BROKEN BAR", // 0xA6
        "SECTION SIGN", // 0xA7
        "DIAERESIS", // 0xA8
        "COPYRIGHT SIGN", // 0xA9
        "FEMININE ORDINAL INDICATOR", // 0xAA
        "left guillemet, LEFT-POINTING DOUBLE ANGLE QUOTATION MARK", // 0xAB
        "NOT SIGN", // 0xAC
        "SHY, SOFT HYPHEN, discretionary hyphen", // 0xAD
        "REGISTERED SIGN, registered trade mark sign", // 0xAE
        "MACRON, overline, APL overbar", // 0xAF
        "DEGREE SIGN, ring above", // 0xB0
        "PLUS-MINUS SIGN", // 0xB1
        "SUPERSCRIPT TWO, squared", // 0xB2
        "SUPERSCRIPT THREE, cubed", // 0xB3
        "ACUTE ACCENT, prime", // 0xB4
        "MICRO SIGN, greek small letter mu", // 0xB5
        "PILCROW SIGN, paragraph sign", // 0xB6
        "MIDDLE DOT", // 0xB7
        "CEDILLA", // 0xB8
        "SUPERSCRIPT ONE", // 0xB9
        "MASCULINE ORDINAL INDICATOR", // 0xBA
        "right guillemet, RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK", // 0xBB
        "VULGAR FRACTION ONE QUARTER", // 0xBC
        "VULGAR FRACTION ONE HALF", // 0xBD
        "VULGAR FRACTION THREE QUARTERS", // 0xBE
        "INVERTED QUESTION MARK", // 0xBF
        "LATIN CAPITAL LETTER A WITH GRAVE", // 0xC0
        "LATIN CAPITAL LETTER A WITH ACUTE", // 0xC1
        "LATIN CAPITAL LETTER A WITH CIRCUMFLEX", // 0xC2
        "LATIN CAPITAL LETTER A WITH TILDE", // 0xC3
        "LATIN CAPITAL LETTER A WITH DIAERESIS", // 0xC4
        "LATIN CAPITAL LETTER A WITH RING ABOVE, angstrom sign", // 0xC5
        "LATIN CAPITAL LETTER AE, latin capital ligature ae", // 0xC6
        "LATIN CAPITAL LETTER C WITH CEDILLA", // 0xC7
        "LATIN CAPITAL LETTER E WITH GRAVE", // 0xC8
        "LATIN CAPITAL LETTER E WITH ACUTE", // 0xC9
        "LATIN CAPITAL LETTER E WITH CIRCUMFLEX", // 0xCA
        "LATIN CAPITAL LETTER E WITH DIAERESIS", // 0xCB
        "LATIN CAPITAL LETTER I WITH GRAVE", // 0xCC
        "LATIN CAPITAL LETTER I WITH ACUTE", // 0xCD
        "LATIN CAPITAL LETTER I WITH CIRCUMFLEX", // 0xCE
        "LATIN CAPITAL LETTER I WITH DIAERESIS", // 0xCF
        "LATIN CAPITAL LETTER ETH", // 0xD0
        "LATIN CAPITAL LETTER N WITH TILDE", // 0xD1
        "LATIN CAPITAL LETTER O WITH GRAVE", // 0xD2
        "LATIN CAPITAL LETTER O WITH ACUTE", // 0xD3
        "LATIN CAPITAL LETTER O WITH CIRCUMFLEX", // 0xD4
        "LATIN CAPITAL LETTER O WITH TILDE", // 0xD5
        "LATIN CAPITAL LETTER O WITH DIAERESIS", // 0xD6
        "MULTIPLICATION SIGN", // 0xD7
        "LATIN CAPITAL LETTER O WITH STROKE", // 0xD8
        "LATIN CAPITAL LETTER U WITH GRAVE", // 0xD9
        "LATIN CAPITAL LETTER U WITH ACUTE", // 0xDA
        "LATIN CAPITAL LETTER U WITH CIRCUMFLEX", // 0xDB
        "LATIN CAPITAL LETTER U WITH DIAERESIS", // 0xDC
        "LATIN CAPITAL LETTER Y WITH ACUTE", // 0xDD
        "LATIN CAPITAL LETTER THORN", // 0xDE
        "LATIN SMALL LETTER SHARP S, Eszett", // 0xDF
        "LATIN SMALL LETTER A WITH GRAVE", // 0xE0
        "LATIN SMALL LETTER A WITH ACUTE", // 0xE1
        "LATIN SMALL LETTER A WITH CIRCUMFLEX", // 0xE2
        "LATIN SMALL LETTER A WITH TILDE", // 0xE3
        "LATIN SMALL LETTER A WITH DIAERESIS", // 0xE4
        "LATIN SMALL LETTER A WITH RING ABOVE", // 0xE5
        "LATIN SMALL LETTER AE, latin small ligature ae, ash", // 0xE6
        "LATIN SMALL LETTER C WITH CEDILLA", // 0xE7
        "LATIN SMALL LETTER E WITH GRAVE", // 0xE8
        "LATIN SMALL LETTER E WITH ACUTE", // 0xE9
        "LATIN SMALL LETTER E WITH CIRCUMFLEX", // 0xEA
        "LATIN SMALL LETTER E WITH DIAERESIS", // 0xEB
        "LATIN SMALL LETTER I WITH GRAVE", // 0xEC
        "LATIN SMALL LETTER I WITH ACUTE", // 0xED
        "LATIN SMALL LETTER I WITH CIRCUMFLEX", // 0xEE
        "LATIN SMALL LETTER I WITH DIAERESIS", // 0xEF
        "LATIN SMALL LETTER ETH, partial differential", // 0xF0
        "LATIN SMALL LETTER N WITH TILDE", // 0xF1
        "LATIN SMALL LETTER O WITH GRAVE", // 0xF2
        "LATIN SMALL LETTER O WITH ACUTE", // 0xF3
        "LATIN SMALL LETTER O WITH CIRCUMFLEX", // 0xF4
        "LATIN SMALL LETTER O WITH TILDE", // 0xF5
        "LATIN SMALL LETTER O WITH DIAERESIS", // 0xF6
        "DIVISION SIGN", // 0xF7
        "LATIN SMALL LETTER O WITH STROKE", // 0xF8
        "LATIN SMALL LETTER U WITH GRAVE", // 0xF9
        "LATIN SMALL LETTER U WITH ACUTE", // 0xFA
        "LATIN SMALL LETTER U WITH CIRCUMFLEX", // 0xFB
        "LATIN SMALL LETTER U WITH DIAERESIS", // 0xFC
        "LATIN SMALL LETTER Y WITH ACUTE", // 0xFD
        "LATIN SMALL LETTER THORN", // 0xFE
        "LATIN SMALL LETTER Y WITH DIAERESIS", // 0xFF
    };
    const char *s = names[c];
    if (s)
        return s;
    return rcstring::printf("%d", c);
}


// vim: set ts=8 sw=4 et :
