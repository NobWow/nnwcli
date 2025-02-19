/**
 * util/utf8.hpp - Publicly available code for reading and writing UTF8 octets.
 * */



#pragma once

#include <cstddef>
#include <exception>
#include <string>
#include "globals.hpp"


namespace nnwcli
{
    class DLL_PUBLIC unicode_too_short : std::exception
    {
        virtual const char* what() const noexcept;
    };
    DLL_PUBLIC unsigned int utf8_read_octets(const char* in, std::size_t n);
    DLL_PUBLIC char utf8_write_octets(char out[4], unsigned int value);
    DLL_PUBLIC std::size_t utf8_count_octets(std::string& out);
}
