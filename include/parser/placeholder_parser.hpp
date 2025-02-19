/**
 * parser/placeholder_parser.hpp - Passive parser implementation for non-textual command invocation.
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


#pragma once

#include "argument_types.hpp"
#include "custom_type.hpp"
#include "globals.hpp"
#include "parser/abstract_parser.hpp"
#include <deque>
namespace nnwcli
{
    class DLL_PUBLIC PlaceholderParser : public AbstractParser
    {
        std::deque<ArgumentTypes> m_queue_types;
        std::deque<std::string> m_queue_string;
        std::deque<char> m_queue_tinyint;
        std::deque<short> m_queue_shortint;
        std::deque<int> m_queue_integer;
        std::deque<long> m_queue_bigint;
        std::deque<float> m_queue_float;
        std::deque<double> m_queue_double;
        std::deque<bool> m_queue_bool;
        std::deque<AbstractCustomType> m_queue_custom;
        std::string m_full_string;

        template<typename T>
        bool pick_from_queue(T& out, ArgumentTypes expected_type, std::deque<T>& deque, bool required = true);
    public:
        virtual std::deque<ArgumentTypes>* get_types_queue();
        virtual std::deque<std::string>* get_string_queue();
        virtual std::deque<char>* get_tinyint_queue();
        virtual std::deque<short>* get_shortint_queue();
        virtual std::deque<int>* get_integer_queue();
        virtual std::deque<long>* get_bigint_queue();
        virtual std::deque<float>* get_float_queue();
        virtual std::deque<double>* get_double_queue();
        virtual std::deque<bool>* get_bool_queue();

        virtual void push_string(const std::string& value);
        virtual void push_tinyint(char value);
        virtual void push_shortint(short value);
        virtual void push_integer(int value);
        virtual void push_bigint(long value);
        virtual void push_unsigned_tinyint(unsigned char value);
        virtual void push_unsigned_shortint(unsigned short value);
        virtual void push_unsigned_integer(unsigned int value);
        virtual void push_unsigned_bigint(unsigned long value);
        virtual void push_float(float value);
        virtual void push_double(double value);
        virtual void push_bool(bool value);

        const std::string& get_full_string() const;
        void set_full_string(const std::string& value);
        void set_full_string(const char* value);

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
    };
}
