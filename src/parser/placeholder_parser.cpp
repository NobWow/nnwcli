/**
 * parser/placeholder_parser.cpp - Passive parser implementation for non-textual command invocation.
 * Can be useful, for example, when an alternative interface, other than CLI is used,
 * but the toolkit should be shared.
 * This class utilizes deques for each type of command arguments and it isn't used directly
 * by the CommandExecutor, especially not in dispatch_line.
 * Instance can be used to directly invoke commands.
 * For example, using methods such as push_string("Hello World") and push_integer(10), then,
 * passing it into a context as a new parser, the command will receive the arguments
 * as if they were textually written.
 * 
 * License: The MIT License.
 * Copyright 2025 dernisnw@neonw.su
 * */


#include "parser/placeholder_parser.hpp"
#include "argument_types.hpp"
#include "parser/abstract_parser.hpp"

using namespace nnwcli;


const std::string& PlaceholderParser::get_full_string() const
{
    return m_full_string;
}
void PlaceholderParser::set_full_string(const std::string& value)
{
    m_full_string = value;
}
void PlaceholderParser::set_full_string(const char* value)
{
    m_full_string = value;
}

bool PlaceholderParser::exhausted() const
{
    return m_pos >= m_queue_types.size();
}
void PlaceholderParser::_throw_if_exhausted()
{
    if(m_pos >= m_queue_types.size())
        throw not_enough_arguments();
}
std::deque<ArgumentTypes>* PlaceholderParser::get_types_queue()
{
    return &m_queue_types;
}
std::deque<std::string>* PlaceholderParser::get_string_queue()
{
    return &m_queue_string;
}
std::deque<char>* PlaceholderParser::get_tinyint_queue()
{
    return &m_queue_tinyint;
}
std::deque<short>* PlaceholderParser::get_shortint_queue()
{
    return &m_queue_shortint;
}
std::deque<int>* PlaceholderParser::get_integer_queue()
{
    return &m_queue_integer;
}
std::deque<long>* PlaceholderParser::get_bigint_queue()
{
    return &m_queue_bigint;
}
std::deque<float>* PlaceholderParser::get_float_queue()
{
    return &m_queue_float;
}
std::deque<double>* PlaceholderParser::get_double_queue()
{
    return &m_queue_double;
}
std::deque<bool>* PlaceholderParser::get_bool_queue()
{
    return &m_queue_bool;
}

void PlaceholderParser::push_string(const std::string& value)
{
    m_queue_types.push_back(ArgumentTypes::CT_STRING);
    m_queue_string.push_back(value);
}
void PlaceholderParser::push_tinyint(const char value)
{
    m_queue_types.push_back(ArgumentTypes::CT_TINYINT);
    m_queue_tinyint.push_back(value);
}
void PlaceholderParser::push_shortint(const short value)
{
    m_queue_types.push_back(ArgumentTypes::CT_SHORTINT);
    m_queue_shortint.push_back(value);
}
void PlaceholderParser::push_integer(const int value)
{
    m_queue_types.push_back(ArgumentTypes::CT_INTEGER);
    m_queue_integer.push_back(value);
}
void PlaceholderParser::push_bigint(const long value)
{
    m_queue_types.push_back(ArgumentTypes::CT_BIGINT);
    m_queue_bigint.push_back(value);
}
void PlaceholderParser::push_unsigned_tinyint(const unsigned char value)
{
    m_queue_types.push_back(ArgumentTypes::CT_UTINYINT);
    m_queue_tinyint.push_back(value);
}
void PlaceholderParser::push_unsigned_shortint(const unsigned short value)
{
    m_queue_types.push_back(ArgumentTypes::CT_USHORTINT);
    m_queue_shortint.push_back(value);
}
void PlaceholderParser::push_unsigned_integer(const unsigned int value)
{
    m_queue_types.push_back(ArgumentTypes::CT_UINTEGER);
    m_queue_integer.push_back(value);
}
void PlaceholderParser::push_unsigned_bigint(const unsigned long value)
{
    m_queue_types.push_back(ArgumentTypes::CT_UBIGINT);
    m_queue_bigint.push_back(value);
}
void PlaceholderParser::push_float(const float value)
{
    m_queue_types.push_back(ArgumentTypes::CT_FLOAT);
    m_queue_float.push_back(value);
}
void PlaceholderParser::push_double(const double value)
{
    m_queue_types.push_back(ArgumentTypes::CT_DOUBLE);
    m_queue_double.push_back(value);
}
void PlaceholderParser::push_bool(const bool value)
{
    m_queue_types.push_back(ArgumentTypes::CT_BOOL);
    m_queue_bool.push_back(value);
}

template<typename T>
bool PlaceholderParser::pick_from_queue(T& out, ArgumentTypes expected_type, std::deque<T>& deque, const bool required)
{
    if(m_queue_types.empty() || deque.empty() || m_queue_types.front() != expected_type)
    {
        if(required)
            throw not_enough_arguments();
        else
            return false;
    }

    out = deque.front();

    m_queue_types.pop_front();
    deque.pop_front();
    m_argument_pos++;
    return true;
}
bool PlaceholderParser::parse_string(std::string& out, const bool required) 
{
    return pick_from_queue<std::string>(out, ArgumentTypes::CT_STRING, m_queue_string, required);
}
bool PlaceholderParser::parse_tinyint(char& out, const bool required) 
{
    return pick_from_queue<char>(out, ArgumentTypes::CT_TINYINT, m_queue_tinyint, required);
}
bool PlaceholderParser::parse_shortint(short& out, const bool required) 
{
    return pick_from_queue<short>(out, ArgumentTypes::CT_SHORTINT, m_queue_shortint, required);
}
bool PlaceholderParser::parse_integer(int& out, const bool required) 
{
    return pick_from_queue<int>(out, ArgumentTypes::CT_INTEGER, m_queue_integer, required);
}
bool PlaceholderParser::parse_bigint(long& out, const bool required) 
{
    return pick_from_queue<long>(out, ArgumentTypes::CT_BIGINT, m_queue_bigint, required);
}
bool PlaceholderParser::parse_unsigned_tinyint(unsigned char& out, const bool required) 
{
    return pick_from_queue<unsigned char>(out, ArgumentTypes::CT_UTINYINT,
            reinterpret_cast<std::deque<unsigned char>&>(m_queue_tinyint), required);
}
bool PlaceholderParser::parse_unsigned_shortint(unsigned short& out, const bool required) 
{
    return pick_from_queue<unsigned short>(out, ArgumentTypes::CT_USHORTINT,
            reinterpret_cast<std::deque<unsigned short>&>(m_queue_shortint), required);
}
bool PlaceholderParser::parse_unsigned_integer(unsigned int& out, const bool required) 
{
    return pick_from_queue<unsigned int>(out, ArgumentTypes::CT_UINTEGER,
            reinterpret_cast<std::deque<unsigned int>&>(m_queue_integer), required);
}
bool PlaceholderParser::parse_unsigned_bigint(unsigned long& out, const bool required) 
{
    return pick_from_queue<unsigned long>(out, ArgumentTypes::CT_UBIGINT,
            reinterpret_cast<std::deque<unsigned long>&>(m_queue_bigint), required);
}
bool PlaceholderParser::parse_float(float& out, const bool required) 
{
    return pick_from_queue<float>(out, ArgumentTypes::CT_FLOAT, m_queue_float, required);
}
bool PlaceholderParser::parse_double(double& out, const bool required) 
{
    return pick_from_queue<double>(out, ArgumentTypes::CT_DOUBLE, m_queue_double, required);
}
bool PlaceholderParser::parse_bool(bool& out, const bool required) 
{
    return pick_from_queue<bool>(out, ArgumentTypes::CT_BOOL, m_queue_bool, required);
}
bool PlaceholderParser::parse_full(std::string& out, const bool required) 
{
    const std::string& full_string = get_full_string();
    if(full_string.empty())
    {
        if(required)
            throw not_enough_arguments();
        else
            return false;
    }

    out = full_string;
    m_argument_pos++;
    return true;
}
