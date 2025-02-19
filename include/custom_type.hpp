#pragma once


#include "globals.hpp"
#include <string>
#include <utility>
namespace nnwcli
{
    class DLL_PUBLIC AbstractCustomType
    {
    public:
        virtual AbstractCustomType& parse_from_string(const std::string& raw) = 0;
        virtual std::pair<AbstractCustomType&, bool> parse_from_argline(
                const std::string& raw, std::size_t start = 0, bool required = true) = 0;
        virtual std::string serialize() = 0;
    };
}
