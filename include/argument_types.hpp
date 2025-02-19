/**
 * argument_types.hpp - Basic argument types that are recognized by parsers and used by commands.
 * Supports basic numeric types from 1 to 8 bytes, float, double,
 * and two ways of representing a string.
 * One way to represent a string is the limited string,
 * that allows the possibility of the next argument coming next after.
 * It is marked as CT_STRING.
 * Another way of representing a textual argument is by consuming the rest of the
 * argument line, making it impossible to have more arguments after.
 * It is marked as CT_FULL. No other arguments are to follow, and such argument type
 * should always be the last argument.
 * 
 * License: The MIT License.
 * Copyright 2025 dernisnw@neonw.su
 * */


#pragma once

#include <sys/types.h>

namespace nnwcli
{
    enum ArgumentTypes : unsigned short
    {
        // one, 'two words', "two words"
        CT_STRING = 0,
        // 123
        CT_TINYINT,
        CT_SHORTINT,
        CT_INTEGER,
        // 99999999999999
        CT_BIGINT,
        // ... same but unsigned variants
        CT_UTINYINT,
        CT_USHORTINT,
        CT_UINTEGER,
        CT_UBIGINT,
        // 123.456
        CT_FLOAT,
        CT_DOUBLE,
        // yes | no | y | n | t | f
        CT_BOOL,
        // string with multiple words
        CT_FULL,
        // parser is custom, and it is parsed as a string
        CT_STRING_CUSTOM = 999,
    };

    /**
     * Returns textual representation of the enumerated argument type.
     * */
    const char* argtype_to_name(ArgumentTypes type);

    /*
     * Using std::variant or unions is not possible when dealing with types with a destructor, such as std::string.
     *
    struct ArgumentValue
    {
        union _argument_value
        {
            std::string     str;
            char            tinyint;
            short           shortint;
            int             integer;
            long            bigint;
            u_char          utinyint;
            ushort          ushortint;
            uint            uinteger;
            ulong           ubigint;
            float           _float;
            double          _double;
            bool            _bool;
            ~_argument_value() {};
        };

        _argument_value type;
        ~ArgumentValue();

    };
    */
    // Only for C++17
    /*
    using ArgumentValue = std::variant<
        char, short, int, long, u_char, ushort, uint, ulong, float, double, bool, std::string>;
    */
}
