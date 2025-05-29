/**
 * context.hpp - Abstract class for context implementations.
 * In order to use the command system, the commands need to show the output somewhere.
 * For this case, the context object is used. It has the methods such as 
 *     write(), nprintf() and << operator for strings to show the output somewhere.
 * Since the most popular choice is either a pipe or standard output, the output
 * needs to be flushed before it can be used.
 *
 * License: The MIT License.
 * Copyright 2025 dernisnw@neonw.su
 * */

#pragma once

#include <cstdarg>
#include <functional>
#include <sstream>
#include <string>
#include <memory>
#include "parser/abstract_parser.hpp"
#include "globals.hpp"


namespace nnwcli
{
    class Command;
    class CommandExecutor;

    /** Base class for execution context.
     *  Used when executing commands, showing output.
     *  Features like command output must be implemented. */

    class DLL_PUBLIC CommandExecutorContext
    {
    protected:
        CommandExecutor*    m_executor;
        std::shared_ptr<AbstractParser>
                            m_parser;
        std::weak_ptr<Command>
                            m_command;
        std::string         m_alias;
    public:
        CommandExecutorContext();
        CommandExecutorContext(
                CommandExecutor* executor,
                std::string argline);
        CommandExecutorContext(
                CommandExecutor* executor,
                std::shared_ptr<AbstractParser>& parser);
        CommandExecutorContext(
                CommandExecutor* executor);
        ~CommandExecutorContext();

        CommandExecutor* get_executor();
        const std::shared_ptr<AbstractParser>& get_parser() const;
        void set_executor(CommandExecutor* new_executor);
        void set_parser(std::shared_ptr<AbstractParser>& parser);

        Command* get_command() const;
        const std::string get_alias() const;
        void set_command(std::string alias, std::shared_ptr<Command>& command);

        template<typename Child>
        static inline std::function<std::shared_ptr<Child>()>
            create_factory()
        {
            return std::function<std::shared_ptr<Child>()>([](){ return std::make_shared<Child>(); });
        }

        virtual void write(const char* data, std::size_t n) = 0;
        virtual void write(const std::string& data) = 0;
        virtual void vnprintf(const char* format, std::size_t n, va_list args) = 0;
        virtual void vnprintf(const std::string& format, std::size_t n, va_list args) = 0;
        virtual void flush() = 0;
        void nprintf(const char* format, std::size_t n, ...);
        void nprintf(const std::string& format, std::size_t n, ...);

        /** Show the output, used by the commands. */
        virtual CommandExecutorContext& operator<<(const char* data);
        virtual CommandExecutorContext& operator<<(const std::string& data);
        virtual CommandExecutorContext& operator<<(const std::stringstream& data);
    };
}
