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


#include "argument_types.hpp"


const char* nnwcli::argtype_to_name(const ArgumentTypes type)
{
    switch (type)
    {
        case CT_STRING:
            return "text";
        case CT_TINYINT:
            return "tiny int";
        case CT_SHORTINT:
            return "short int";
        case CT_INTEGER:
            return "int";
        case CT_BIGINT:
            return "big int";
        case CT_UTINYINT:
            return "+tiny int";
        case CT_USHORTINT:
            return "+short int";
        case CT_UINTEGER:
            return "+int";
        case CT_UBIGINT:
            return "+big int";
        case CT_FLOAT:
            return "float";
        case CT_DOUBLE:
            return "double float";
        case CT_BOOL:
            return "yes/no";
        case CT_FULL:
            return "full text...";
        case CT_STRING_CUSTOM:
            return "[predefined]";
    }
    return "unknown";
}
