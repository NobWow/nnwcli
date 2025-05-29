/**
 * parser/argline_parser.hpp - CLI parser for extracting textual space-separated arguments.
 * Capable of translating escape sequences and handling format errors, argument-wise errors thrown by the parser.
 * 
 * License: The MIT License.
 * Copyright 2025 dernisnw@neonw.su
 * */



#pragma once

#include <cstddef>
#include <sys/types.h>
#include <exception>
#include "globals.hpp"
#include "parser/abstract_parser.hpp"


namespace nnwcli
{
    const char __single_quote = '\'';
    const char __double_quote = '"';
    const char __escape = '\\';
    const char __whitespace = ' ';

    class DLL_PUBLIC unexpected_escape_character : std::exception
    {
    public:
        int m_pos;
        unexpected_escape_character(const std::size_t i)
        {
            m_pos = i;
        }
        virtual const char* what() const noexcept override;
    };
    class DLL_PUBLIC invalid_escape_format : std::exception
    {
    public:
        virtual const char* what() const noexcept override;
    };

    class DLL_PUBLIC ArglineParser : public AbstractParser
    {
        std::string m_argline;

        // moves m_pos to the beginning of the next argument, otherwise sets m_pos to -1
        // if m_pos already was -1 then throws not_enough_arguments
        void _next();

        static std::size_t _find_unescaped_quote(const std::string& argline, const std::size_t start = 0);
        static std::size_t _find_unescaped_whitespace(const std::string& argline, const std::size_t start = 0);
        static void _unescape_into(std::string& out, const std::string& in, const std::size_t start = 0);
        static std::size_t _interpret_escape_into(std::string& out, std::size_t n, const char* seq);
    protected:
        virtual void _throw_if_exhausted() override;

    public:
        explicit ArglineParser() = delete;
        explicit ArglineParser(ArglineParser&) = delete;
        explicit ArglineParser(ArglineParser&&) = delete;

        ArglineParser(
                const std::string argline,
                const std::size_t pos = 0);

        virtual bool exhausted() const override;

        //
        // Parsers, will advance m_pos and give the next argument, or raise one of the exceptions.
        // These functions are to be used in Command::execute() call, as an instance of this
        // object is received as a first argument.
        //

        virtual bool parse_string(std::string& out, bool required = true) override;
        virtual bool parse_tinyint(char& out, bool required = true) override;
        virtual bool parse_shortint(short& out, bool required = true) override;
        virtual bool parse_integer(int& out, bool required = true) override;
        virtual bool parse_bigint(long& out, bool required = true) override;
        template<typename T>
        bool parse_unsigned(T& out, const bool required = false);
        virtual bool parse_unsigned_tinyint(unsigned char& out, bool required = true) override;
        virtual bool parse_unsigned_shortint(unsigned short& out, bool required = true) override;
        virtual bool parse_unsigned_integer(unsigned int& out, bool required = true) override;
        virtual bool parse_unsigned_bigint(unsigned long& out, bool required = true) override;
        virtual bool parse_float(float& out, bool required = true) override;
        virtual bool parse_double(double& out, bool required = true) override;
        virtual bool parse_bool(bool& out, bool required = true) override;
        virtual bool parse_full(std::string& out, bool required = false) override;
        // This will attempt to access the custom parser registry, otherwise it will throw unknown_custom_type
        //bool parse_custom(void* out, const std::string& custom_type_name, bool required = true);


        //
        // Only for an argline parser.
        //
        const std::string& get_argline() const;

        //
        // Shorthand operator parsers, for C++ convenience. Interpreted as required parses.
        //
    };
}
