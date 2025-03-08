/**
 * parser/argline_parser.cpp - CLI parser for extracting textual space-separated arguments.
 * Capable of translating escape sequences and handling format errors, argument-wise errors thrown by the parser.
 * 
 * License: The MIT License.
 * Copyright 2025 dernisnw@neonw.su
 * */


#include "parser/argline_parser.hpp"
#include "parser/abstract_parser.hpp"
#include "util/string_case.hpp"
#include "util/utf8.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <stdexcept>
#include <string>

using namespace nnwcli;

// exceptions

const char* unexpected_escape_character::what() const noexcept
{
    return "argument ends with an unexpected escape character";
}
const char* invalid_escape_format::what() const noexcept
{
    return "escape format specified incorrectly";
}

std::size_t ArglineParser::_find_unescaped_quote(const std::string& argline, const std::size_t start)
{
    const char quote = argline[start];

    assert(quote == __single_quote || quote == __double_quote);

    if(argline.size() <= start + 1)
    {
        throw unclosed_quote();
    }

    std::size_t i = start + 1;
    for(; i < argline.size(); i++)
    {
        if((argline[i - 1] == __escape && (i < 2 || argline[i - 2] != __escape)))
        {
            i++;
            continue;
        }

        if(argline[i] == quote)
            // encountered the same quote symbol that is not preceded
            // by an escape symbol, which is itself not escaped
            return i;
    }
    throw unclosed_quote();
}
std::size_t ArglineParser::_find_unescaped_whitespace(const std::string& argline, std::size_t i)
{
    for(; i < argline.size(); i++)
    {
        if((i >= 1 && argline[i - 1] == __escape && (i < 2 || argline[i - 2] != __escape)))
        {
            i++;
            continue;
        }

        if(argline[i] == __whitespace)
            return i;
    }
    if(argline[i - 1] == __escape)
        throw unexpected_escape_character(i);
    return std::string::npos;
}
void ArglineParser::_unescape_into(std::string& out, const std::string& in, std::size_t i)
{
    out.clear();
    out.reserve(in.size());

    for(; i < in.size(); i++)
    {
        if(in[i] == __escape)
        {
            if(i == in.size() - 1)
                throw unexpected_escape_character(i);
            i += _interpret_escape_into(out, in.size() - i, &in.c_str()[i + 1]);
        }
        else
        {
            out.push_back(in[i]);
        }
    }
}
std::size_t ArglineParser::_interpret_escape_into(std::string& out, std::size_t n, const char* seq)
{
    if(!n)
        return 0;

    // prefixed escape sequences
    switch(seq[0])
    {
        // single character escapes
        case ' ':
            out.push_back(' ');
            return 1;
        case '"':
            out.push_back('"');
            return 1;
        case '\'':
            out.push_back('\'');
            return 1;
        case '?':
            out.push_back('\?');
            return 1;
        case '\\':
            out.push_back('\\');
            return 1;
        case 'a':
            out.push_back('\a');
            return 1;
        case 'b':
            out.push_back('\b');
            return 1;
        case 'f':
            out.push_back('\f');
            return 1;
        case 'n':
            out.push_back('\n');
            return 1;
        case 'r':
            out.push_back('\r');
            return 1;
        case 't':
            out.push_back('\t');
            return 1;
        case 'v':
            out.push_back('\v');
            return 1;

        // multi-character escapes
        case 'x':
        {
            if(n < 3)
                throw invalid_escape_format();

            std::string raw_value(&seq[1], 2);
            make_lowercase(raw_value);

            // Note that this can have values greater than 127,
            // this will allow to create unicode-invalid strings.
            const unsigned long value = std::stoul(raw_value, nullptr, 16);

            out.push_back(value);

            return 3;
        }
        case 'u':
        {
            // this will create an unicode escape sequence and push it into the output.
            if(n < 5)
                throw invalid_escape_format();

            std::string raw_value(&seq[1], 4);
            make_lowercase(raw_value);

            char octal_buffer[4] = {};
            const unsigned long value = std::stoul(raw_value, nullptr, 16);

            const std::size_t n_written = utf8_write_octets(octal_buffer, value);
            // add the unicode sequence into the output.
            out.append(octal_buffer, n_written);
            return 5;
        }
        default:
        {
            unsigned long value;
            // octal value encountered
            if(n < 4)
                throw invalid_escape_format();

            std::string raw_value(&seq[1], 3);
            try
            {
                value = std::stoul(raw_value, nullptr, 8);
            } catch(std::invalid_argument& exc)
            {
                // wrong data specified, rethrow as an invalid escape format
                throw invalid_escape_format();
            }
            out.push_back(value);
            return 4;
        }
    }
}

/*
bool parse_args(const std::string argline,
        const std::vector<ArgumentTypes> types, std::vector<ArgumentValue> result)
{
    std::size_t pos = 0;

    result.clear();
    result.reserve(types.size());
    ArgumentValue _h = {._bool=false};

    for(std::size_t i = 0; i < types.size(); i++)
    {
        const ArgumentTypes type = types[i];
        switch (type)
        {
            case CT_STRING:
                break;
            case CT_TINYINT:
                break;
        }
        pos = argline.find_first_not_of(__whitespace, pos);
    }
    return false;
}*/

void ArglineParser::_throw_if_exhausted()
{
    if(m_pos >= m_argline.size())
        throw not_enough_arguments();
}
void ArglineParser::_next()
{
    if(exhausted()) return;
    m_pos = std::min(m_argline.find_first_not_of(' ', m_pos), m_argline.size());
}

ArglineParser::ArglineParser(
        const std::string argline,
        const std::size_t pos)
{
    m_argline = argline;
    m_pos = pos;
}

bool ArglineParser::exhausted() const
{
    return m_pos == std::string::npos || m_pos >= m_argline.size();
}
bool ArglineParser::parse_string(std::string& out, const bool required)
{
    std::size_t end;

    _next();
    if(required)
        _throw_if_exhausted();
    else if(exhausted())
        return false;

    if(m_argline[m_pos] == __single_quote || m_argline[m_pos] == __double_quote)
    {
        end = _find_unescaped_quote(m_argline, m_pos);
        m_pos++;
        const std::string escaped = m_argline.substr(m_pos, end - m_pos);
        _unescape_into(out, escaped);
        m_pos = end + 1;
        m_argument_pos++;
        return true;
    }
    else
    {
        end = _find_unescaped_whitespace(m_argline, m_pos);
        if (end == std::string::npos)
            end = m_argline.size();
    }

    const std::string escaped = m_argline.substr(m_pos, end - m_pos);
    _unescape_into(out, escaped);

    m_pos = end;
    m_argument_pos++;
    return true;
}
bool ArglineParser::parse_bool(bool& out, const bool required)
{
    std::size_t end;

    _next();
    if(required)
        _throw_if_exhausted();
    else if(exhausted())
        return false;

    end = std::min(m_argline.find_first_of(' ', m_pos), m_argline.size());
    std::string arg = m_argline.substr(m_pos, end - m_pos);
    make_lowercase(arg);
    if(arg == "yes" || arg == "on" || arg == "true" || arg == "y" || arg == "t")
    {
        out = true;
        m_argument_pos++;
        return true;
    }
    else if(arg == "no" || arg == "off" || arg == "false" || arg == "n" || arg == "f")
    {
        out = false;
        m_argument_pos++;
        return true;
    }
    throw std::invalid_argument("bool can be either on or off, yes or no, true or false");
}
bool ArglineParser::parse_bigint(long& out, const bool required)
{
    std::size_t end;

    _next();
    if(required)
        _throw_if_exhausted();
    else if(exhausted())
        return false;

    end = std::min(m_argline.find_first_of(' ', m_pos), m_argline.size());

    // attempt to convert
    out = std::stol(m_argline.substr(m_pos, end - m_pos));
    m_pos = end;
    m_argument_pos++;
    return true;
}
bool ArglineParser::parse_double(double& out, const bool required)
{
    std::size_t end;

    _next();
    if(required)
        _throw_if_exhausted();
    else if(exhausted())
        return false;

    end = std::min(m_argline.find_first_of(' ', m_pos), m_argline.size());

    // attempt to convert
    out = std::stod(m_argline.substr(m_pos, end - m_pos));
    m_pos = end;
    m_argument_pos++;
    return true;
}
bool ArglineParser::parse_float(float& out, const bool required)
{
    std::size_t end;

    _next();
    if(required)
        _throw_if_exhausted();
    else if(exhausted())
        return false;

    end = std::min(m_argline.find_first_of(' ', m_pos), m_argline.size());

    // attempt to convert
    out = std::stof(m_argline.substr(m_pos, end - m_pos));
    m_pos = end;
    m_argument_pos++;
    return true;
}
bool ArglineParser::parse_integer(int& out, const bool required)
{
    std::size_t end;

    _next();
    if(required)
        _throw_if_exhausted();
    else if(exhausted())
        return false;

    end = std::min(m_argline.find_first_of(' ', m_pos), m_argline.size());

    // attempt to convert
    out = std::stoi(m_argline.substr(m_pos, end - m_pos));
    m_pos = end;
    m_argument_pos++;
    return true;
}
bool ArglineParser::parse_shortint(short& out, const bool required)
{
    std::size_t end;

    _next();
    if(required)
        _throw_if_exhausted();
    else if(exhausted())
        return false;

    end = std::min(m_argline.find_first_of(' ', m_pos), m_argline.size());

    // attempt to convert
    out = std::stoi(m_argline.substr(m_pos, end - m_pos));
    m_pos = end;
    m_argument_pos++;
    return true;
}
bool ArglineParser::parse_tinyint(char& out, const bool required)
{
    std::size_t end;

    _next();
    if(required)
        _throw_if_exhausted();
    else if(exhausted())
        return false;

    end = std::min(m_argline.find_first_of(' ', m_pos), m_argline.size());

    // attempt to convert
    out = std::stoi(m_argline.substr(m_pos, end - m_pos));
    m_pos = end;
    m_argument_pos++;
    return true;
}
bool ArglineParser::parse_full(std::string& out, const bool required)
{
    _next();
    if(required)
        _throw_if_exhausted();
    else if(exhausted())
        return false;

    const std::string escaped = m_argline.substr(m_pos, m_argline.size() - m_pos);
    _unescape_into(out, escaped);
    
    m_argument_pos++;
    return true;
}
template<typename T>
bool ArglineParser::parse_unsigned(T& out, const bool required)
{
    std::size_t end;

    _next();
    if(required)
        _throw_if_exhausted();
    else if(exhausted())
        return false;

    end = std::min(m_argline.find_first_of(' ', m_pos), m_argline.size());

    // attempt to convert
    out = std::stoul(m_argline.substr(m_pos, end - m_pos));
    m_pos = end;
    m_argument_pos++;
    return true;
}
bool ArglineParser::parse_unsigned_bigint(unsigned long& out, const bool required)
{
    return parse_unsigned<unsigned long>(out, required);
}
bool ArglineParser::parse_unsigned_integer(unsigned int& out, const bool required)
{
    return parse_unsigned<unsigned int>(out, required);
}
bool ArglineParser::parse_unsigned_shortint(unsigned short& out, const bool required)
{
    return parse_unsigned<unsigned short>(out, required);
}
bool ArglineParser::parse_unsigned_tinyint(unsigned char& out, const bool required)
{
    return parse_unsigned<unsigned char>(out, required);
}
