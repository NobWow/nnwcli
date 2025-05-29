/**
 * context.cpp - Implementation of common methods for abstract command execution contexts.
 * 
 * License: The MIT License.
 * Copyright 2025 dernisnw@neonw.su
 * */


#include "context.hpp"
#include "parser/argline_parser.hpp"
#include <cstdarg>
#include <memory>

using namespace nnwcli;

// do not initialize anything, set executor and parser later
CommandExecutorContext::CommandExecutorContext(
        CommandExecutor* const executor, const std::string argline)
{
    m_executor = executor;
    m_parser = std::make_shared<ArglineParser>(argline);
}
CommandExecutorContext::CommandExecutorContext(
                CommandExecutor* const executor,
                std::shared_ptr<AbstractParser>& parser) :
    m_parser(parser)
{
    m_executor = executor;
}
CommandExecutorContext::CommandExecutorContext(CommandExecutor* const executor) :
    m_parser(nullptr)
{
    m_executor = executor;
}
CommandExecutorContext::CommandExecutorContext() : m_executor(nullptr), m_parser(nullptr) {}
CommandExecutorContext::~CommandExecutorContext()
{
    if(m_parser)
    {
        m_parser.reset();
    }
    if(m_executor)
    {
        m_executor = nullptr;
    }
}
CommandExecutor* CommandExecutorContext::get_executor()
{
    return m_executor;
}
void CommandExecutorContext::set_executor(CommandExecutor* const new_executor)
{
    m_executor = new_executor;
}
const std::shared_ptr<AbstractParser>& CommandExecutorContext::get_parser() const
{
    return m_parser;
}
void CommandExecutorContext::set_parser(std::shared_ptr<AbstractParser>& new_parser)
{
    m_parser = new_parser;
}
Command* CommandExecutorContext::get_command() const
{
    if(m_command.expired())
        return nullptr;
    return m_command.lock().get();
}
const std::string CommandExecutorContext::get_alias() const
{
    return m_alias;
}
void CommandExecutorContext::set_command(std::string alias, std::shared_ptr<Command>& command)
{
    m_alias = alias;
    m_command = command;
}
void CommandExecutorContext::nprintf(const char* format, std::size_t n, ...)
{
    va_list args;

    va_start(args, n);
    vnprintf(format, n, args);
    va_end(args);
}
void CommandExecutorContext::nprintf(const std::string& format, std::size_t n, ...)
{
    va_list args;

    va_start(args, n);
    vnprintf(format, n, args);
    va_end(args);
}
CommandExecutorContext& CommandExecutorContext::operator<<(const char* data)
{
    write(data);
    return *this;
}
CommandExecutorContext& CommandExecutorContext::operator<<(const std::string& data)
{
    write(data);
    return *this;
}
CommandExecutorContext& CommandExecutorContext::operator<<(const std::stringstream& data)
{
    write(data.str());
    return *this;
}
