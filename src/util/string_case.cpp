#include "util/string_case.hpp"
#include <algorithm>
#include <cctype>


void DLL_PUBLIC nnwcli::make_lowercase(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(),
            [](unsigned char chr) { return std::tolower(chr); });
}

void DLL_PUBLIC nnwcli::make_uppercase(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(),
            [](unsigned char chr) { return std::toupper(chr); });
}
