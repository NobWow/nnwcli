/**
 * parser/abstract_parser.hpp - An abstraction layer uniting the ways of getting the arguments for a command.
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


#pragma once

#include <cstddef>
#include <string>
#include <sys/types.h>
#include <exception>
#include "globals.hpp"


namespace nnwcli
{
    class DLL_PUBLIC unclosed_quote : std::exception
    {
    public:
        virtual const char* what() const noexcept override;
    };
    class DLL_PUBLIC not_enough_arguments : std::exception
    {
    public:
        virtual const char* what() const noexcept override;
    };
    class DLL_PUBLIC too_many_arguments : std::exception
    {
    public:
        virtual const char* what() const noexcept override;
    };
    class DLL_PUBLIC unknown_custom_type : std::exception
    {
    public:
        virtual const char* what() const noexcept override;
    };

    // std::invalid_argument can be raised

    class DLL_PUBLIC AbstractParser
    {
    protected:
        std::size_t m_pos = 0, m_argument_pos = 0;
        virtual void _throw_if_exhausted() = 0;
    public:
        virtual bool exhausted() const = 0;
        virtual std::size_t get_pos() const;
        virtual std::size_t get_argument_pos() const;
        virtual void set_pos(std::size_t pos);
        virtual void reset_pos();
        virtual void reset_argument_pos();

        //
        // Parsers, will advance m_pos and give the next argument, or raise one of the exceptions.
        // These functions are to be used in Command::execute() call, as an instance of this
        // object is received as a first argument.
        //

        virtual void parse_finish();
        virtual bool parse_string(std::string& out, bool required = true) = 0;
        virtual bool parse_tinyint(char& out, bool required = true) = 0;
        virtual bool parse_shortint(short& out, bool required = true) = 0;
        virtual bool parse_integer(int& out, bool required = true) = 0;
        virtual bool parse_bigint(long& out, bool required = true) = 0;
        virtual bool parse_unsigned_tinyint(unsigned char& out, bool required = true) = 0;
        virtual bool parse_unsigned_shortint(unsigned short& out, bool required = true) = 0;
        virtual bool parse_unsigned_integer(unsigned int& out, bool required = true) = 0;
        virtual bool parse_unsigned_bigint(unsigned long& out, bool required = true) = 0;
        virtual bool parse_float(float& out, bool required = true) = 0;
        virtual bool parse_double(double& out, bool required = true) = 0;
        virtual bool parse_bool(bool& out, bool required = true) = 0;
        virtual bool parse_full(std::string& out, bool required = false) = 0;
        // This will attempt to access the custom parser registry, otherwise it will throw unknown_custom_type
        //bool parse_custom(void* out, const std::string& custom_type_name, bool required = true);

        //
        // Shorthand operator parsers, for C++ convenience. Interpreted as required parses.
        //

        virtual void operator>>(std::string& out);
        virtual void operator>>(char& out);
        virtual void operator>>(short& out);
        virtual void operator>>(int& out);
        virtual void operator>>(long& out);
        virtual void operator>>(unsigned char& out);
        virtual void operator>>(unsigned short& out);
        virtual void operator>>(unsigned int& out);
        virtual void operator>>(unsigned long& out);
        virtual void operator>>(float& out);
        virtual void operator>>(double& out);
        virtual void operator>>(bool& out);
    };
}
