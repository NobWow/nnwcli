/**
 * util/string_case.hpp - Publicly available code for string case conversion.
 * */



#pragma once

#include <string>
#include "globals.hpp"

namespace nnwcli
{
    DLL_PUBLIC void make_lowercase(std::string& str);
    DLL_PUBLIC void make_uppercase(std::string& str);
}
