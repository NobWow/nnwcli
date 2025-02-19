/**
 * parser/abstract_parser.cpp - An abstraction layer uniting the ways of getting the arguments for a command.
 * The concept of getting the command arguments is by creating an each method for every
 * argument type that is defined in "argument_types.hpp".
 * Also provides a convenient way of getting an argument out by using >> operator. However,
 * this forces an argument to be mandatory.
 * In order to get an optional argument, for example, parse_*(out, false), the second bool argument
 * should be set to false.
 * When the arguments should end, parse_finish() method should be called by the command implementation,
 * indicating that the arguments should not be parsed anymore. It will throw too_many_arguments when
 * the parser is not exhausted yet.
 * 
 * License: The MIT License.
 * Copyright 2025 dernisnw@neonw.su
 * */


#include "parser/abstract_parser.hpp"

using namespace nnwcli;

// exceptions

const char* unclosed_quote::what() const noexcept
{
    return "unclosed quote encountered";
}
const char* not_enough_arguments::what() const noexcept
{
    return "not enough arguments specified";
}
const char* too_many_arguments::what() const noexcept
{
    return "too many arguments specified";
}


std::size_t AbstractParser::get_pos() const
{
    return m_pos;
}
std::size_t AbstractParser::get_argument_pos() const
{
    return m_argument_pos;
}
void AbstractParser::set_pos(const std::size_t pos)
{
    m_pos = pos;
}
void AbstractParser::reset_pos()
{
    m_pos = 0;
}
void AbstractParser::reset_argument_pos()
{
    m_argument_pos = 0;
}

void AbstractParser::operator>>(std::string& out)
{
    parse_string(out, true);
}
void AbstractParser::operator>>(char& out)
{
    parse_tinyint(out, true);
}
void AbstractParser::operator>>(short& out)
{
    parse_shortint(out, true);
}
void AbstractParser::operator>>(int& out)
{
    parse_integer(out, true);
}
void AbstractParser::operator>>(long& out)
{
    parse_bigint(out, true);
}
void AbstractParser::operator>>(float& out)
{
    parse_float(out, true);
}
void AbstractParser::operator>>(double& out)
{
    parse_double(out, true);
}
void AbstractParser::operator>>(bool& out)
{
    parse_bool(out, true);
}
void AbstractParser::operator>>(unsigned char& out)
{
    parse_unsigned_tinyint(out, true);
}
void AbstractParser::operator>>(unsigned short& out)
{
    parse_unsigned_shortint(out, true);
}
void AbstractParser::operator>>(unsigned int& out)
{
    parse_unsigned_integer(out, true);
}
void AbstractParser::operator>>(unsigned long& out)
{
    parse_unsigned_bigint(out, true);
}
void AbstractParser::parse_finish()
{
    // will throw too many arguments exception when the parser is not exhausted yet
    if(!exhausted())
        throw too_many_arguments();
}
