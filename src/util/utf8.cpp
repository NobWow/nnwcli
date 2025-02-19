#include "util/utf8.hpp"

const char* nnwcli::unicode_too_short::what() const noexcept
{
    return "missing utf-8 octet";
}

unsigned int nnwcli::utf8_read_octets(const char* in, const std::size_t n)
{
    if(!n)
        return 0;

    if(~in[0] & 0b10000000)
    {
        // one octet
        return (in[0] & 0b01111111) << 0;
    }
    else if((in[0] & 0b11100000) == 0b11000000)
    {
        if(n < 2)
            throw unicode_too_short();
        // two octets
        return ((in[0] & 0b00011111) << 6) |
               ((in[1] & 0b00111111) << 0);
    }
    else if((in[0] & 0b11110000) == 0b11100000)
    {
        // three octets
        if(n < 3)
            throw unicode_too_short();
        return ((in[0] & 0b00001111) << 12) |
               ((in[1] & 0b00111111) << 6) |
               ((in[2] & 0b00111111) << 0);
    }
    else if((in[0] & 0b11111000) == 0b11110000)
    {
        // four octets
        if(n < 4)
            throw unicode_too_short();
        return ((in[0] & 0b00000111) << 18) |
               ((in[1] & 0b00111111) << 12) |
               ((in[2] & 0b00111111) << 6) |
               ((in[3] & 0b00111111) << 0);
    }
    else
    {
        // invalid unicode provided
        return 0;
    }
}
char nnwcli::utf8_write_octets(char out[4], const unsigned int value)
{
    if(value >= 0 && value < 0x80)
    {
        // out[0] = 0b00000000 | ((value >> 0) & 0b01111111);
        if(out)
            out[0] = value;
        return 1;
    }
    if(value >= 0x80 && value < 0x800)
    {
        if(out)
        {
            out[0] = 0b11000000 | ((value >> 6) & 0b00011111);
            out[1] = 0b10000000 | ((value >> 0) & 0b00111111);
        }
        return 2;
    }
    if(value >= 0x800 && value < 0x00010000)
    {
        if(out)
        {
            out[0] = 0b11100000 | ((value >> 12) & 0b00001111);
            out[1] = 0b10000000 | ((value >> 6) & 0b00111111);
            out[2] = 0b10000000 | ((value >> 0) & 0b00111111);
        }
        return 3;
    }
    if(value >= 0x00010000 && value <= 0x0010FFFF)
    {
        if(out)
        {
            out[0] = 0b11110000 | ((value >> 18) & 0b00000111);
            out[1] = 0b10000000 | ((value >> 12) & 0b00111111);
            out[2] = 0b10000000 | ((value >> 6) & 0b00111111);
            out[3] = 0b10000000 | ((value >> 0) & 0b00111111);
        }
        return 4;
    }
    return 0;
}
