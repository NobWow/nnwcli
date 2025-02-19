/**
 * argument.hpp - Struct for representing an argument of the command.
 * It is used by the command implementations, in constructor specifically.
 * For example, the struct could be represented as:
 * m_args = {{nnwcli::CT_STRING, "name", "Username for login"}, {nnwcli::CT_STRING, "password", "Password for login"}}
 * ... etc.
 * 
 * License: The MIT License.
 * Copyright 2025 dernisnw@neonw.su
 * */


#pragma once

#include <string>
#include "argument_types.hpp"
#include "globals.hpp"


namespace nnwcli
{
    struct DLL_PUBLIC ArgumentDefinition
    {
        ArgumentTypes   m_type;
        std::string     m_name;
        std::string     m_description;
    };
}
