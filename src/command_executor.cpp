/**
 * command_executor.cpp - Implementation of command line interface
 * abstraction layer for one-line command execution. dispatch_line is using ArglineParser,
 * made for parsing text-written argument lines.
 * 
 * License: The MIT License.
 * Copyright 2025 dernisnw@neonw.su
 * */


#include "command_executor.hpp"
#include "parser/argline_parser.hpp"
#include <cassert>
#include <iterator>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <utility>

using namespace nnwcli;


CommandExecutor::CommandExecutor() :
    m_context_factory(nullptr), m_latest_context(nullptr) {}
CommandExecutor::CommandExecutor(
        const std::function<std::shared_ptr<CommandExecutorContext>()>& context_factory) :
    m_context_factory(context_factory), m_latest_context(nullptr) {}
CommandExecutor::CommandExecutor(
        const std::function<std::shared_ptr<CommandExecutorContext>()>&& context_factory) :
    m_context_factory(context_factory), m_latest_context(nullptr) {}

const std::function<std::shared_ptr<CommandExecutorContext>()>&
CommandExecutor::get_factory()
{
    return m_context_factory;
}

void
CommandExecutor::set_factory(
        const std::function<std::shared_ptr<CommandExecutorContext>()>& factory)
{
    m_context_factory = factory;
}

bool CommandExecutor::register_command(
        const std::string name, const std::shared_ptr<Command> command)
{
    auto cmd = m_aliases.find(name);

    if(cmd != m_aliases.cend())
        return false;

    m_commands.insert(command);
    m_aliases[name] = command;

    return true;
}
bool CommandExecutor::register_command(std::shared_ptr<Command> command)
{
    return register_command(command->get_name(), command);
}
bool CommandExecutor::add_alias(const std::string target, const std::string src)
{
    if(m_aliases.find(target) != m_aliases.cend())
        return false;

    try {
        auto res = m_aliases.insert({target, m_aliases.at(src)});
        return res.second;
    } catch (const std::out_of_range& e) {
        return false;
    }
}
bool CommandExecutor::remove_alias(const std::string cmd)
{
    auto found = m_aliases.find(cmd);
    if(found == m_aliases.end())
        return false;

    m_aliases.erase(found);
    return true;
}
bool CommandExecutor::unregister_command(
        const std::string name, const bool delete_aliases)
{
    auto found = m_aliases.find(name);

    if(found == m_aliases.end())
        return false;

    auto& cmd = found->second;
    m_commands.erase(cmd);

    if(delete_aliases)
    {
        auto it = m_aliases.begin();

        for(; it != m_aliases.end();)
        {
            if(it->second.get() == cmd.get())
                it = m_aliases.erase(it);
            else
                it++;
        }
    }
    return true;
}
bool CommandExecutor::dispatch_line(
        const std::string line,
        std::shared_ptr<CommandExecutorContext> context_override,
        void* const data)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    // get the command name
    const std::size_t _spl = line.find_first_of(__whitespace);
    std::string cmdname;
    std::string argline;

    if(_spl != std::string::npos)
    {
        cmdname = line.substr(0, _spl);
        argline = line.substr(_spl + 1);
    }
    else
    {
        cmdname = line;
    }

    // create the argline parser
    auto parser = std::static_pointer_cast<AbstractParser>(
            std::make_shared<ArglineParser>(argline));
    if(context_override)
        m_latest_context = context_override;
    else
        m_latest_context = m_context_factory();
    auto ctx = m_latest_context.get();
    m_latest_context->set_parser(parser);
    m_latest_context->set_executor(this);
    auto cmd = m_aliases.find(cmdname);

    if(cmd == m_aliases.cend())
    {
        // command not found
        handle_unknown_command(cmdname, m_latest_context);
        return false;
    }

    // dispatch the command
    try
    {
        m_latest_context->set_command(cmdname, cmd->second);

        cmd->second->execute(m_latest_context.get(), data);
    }
    // parser errors
    catch(const unexpected_escape_character& e)
    {
        *m_latest_context << "Error: unexpected escape character encountered at the end of the line.\n";
        m_latest_context->flush();
        return false;
    }
    catch(const invalid_escape_format& e)
    {
        std::stringstream ss;
        std::string argname;
        const char* argtype;
        std::pair<
            std::vector<ArgumentDefinition>::const_iterator,
            std::vector<ArgumentDefinition>::const_iterator
            > it;
        if(parser->get_argument_pos() + 1 > cmd->second->get_args_count())
        {
            it = cmd->second->get_optarg_iter();
            std::advance(it.first, parser->get_argument_pos() - cmd->second->get_args_count());
        }
        else
        {
            it = cmd->second->get_arg_iter();
            std::advance(it.first, parser->get_argument_pos());
        }
        ss << "Invalid escape code sequence specified for argument \"" << it.first->m_name << "\":" << std::endl;
        ss << argline.substr(parser->get_pos()) << std::endl;
        *ctx << ss;
        ctx->flush();

    }
    catch(const std::out_of_range& e)
    {
        std::stringstream ss;
        std::string argname;
        const char* argtype;
        std::pair<
            std::vector<ArgumentDefinition>::const_iterator,
            std::vector<ArgumentDefinition>::const_iterator
            > it;
        if(parser->get_argument_pos() + 1 > cmd->second->get_args_count())
        {
            // report optional argument instead
            it = cmd->second->get_optarg_iter();
            std::advance(it.first, parser->get_argument_pos() - cmd->second->get_args_count());
        }
        else
        {
            // report mandatory argument
            it = cmd->second->get_arg_iter();
            std::advance(it.first, parser->get_argument_pos());
        }
        ss << "Value outside of the boundaries provided for argument \"" << it.first->m_name << "\"." << std::endl;
        *ctx << ss;
        ctx->flush();

    }
    catch(const std::invalid_argument& e)
    {
        std::stringstream ss;
        std::string argname;
        const char* argtype;
        std::pair<
            std::vector<ArgumentDefinition>::const_iterator,
            std::vector<ArgumentDefinition>::const_iterator
            > it;
        if(parser->get_argument_pos() + 1 > cmd->second->get_args_count())
        {
            it = cmd->second->get_optarg_iter();
            std::advance(it.first, parser->get_argument_pos() - cmd->second->get_args_count());
        }
        else
        {
            it = cmd->second->get_arg_iter();
            std::advance(it.first, parser->get_argument_pos());
        }
        if(!it.first.base())
        {
            throw std::runtime_error("invalid iterator");
        }
        ss << "Invalid value specified for argument \"" << it.first->m_name << "\"." << std::endl;

        cmd->second->format_usage_into(ss, ctx->get_alias());
        ss << std::endl;

        *ctx << ss;
        ctx->flush();
    }
    catch(const too_many_arguments& e)
    {
        std::stringstream ss;
        ss << "This command requires at most " << cmd->second->get_args_count() + cmd->second->get_optargs_count() << 
            " arguments, but received more." << std::endl;
        cmd->second->format_usage_into(ss, cmdname);
        ss << std::endl;
        *ctx << ss;
        ctx->flush();
    }
    catch(const not_enough_arguments& e)
    {
        std::stringstream ss;
        ss << "This command requires at least " << cmd->second->get_args_count() << " arguments, but received "
            << ctx->get_parser()->get_argument_pos() << "." << std::endl;
        cmd->second->format_usage_into(ss, ctx->get_alias());
        ss << std::endl;
        *ctx << ss;
        ctx->flush();
    }
    return true;
}

void CommandExecutor::handle_unknown_command(
        const std::string cmd, std::shared_ptr<CommandExecutorContext> context)
{
    *context << "Unknown command: " << cmd << "\n";
    context->flush();
}

std::shared_ptr<Command>& CommandExecutor::get_command(const std::string name)
{
    auto cmd = m_aliases.find(name);

    if(cmd == m_aliases.cend())
    {
        // command not found
        throw command_not_found();
    }

    return cmd->second;
}
const std::shared_ptr<CommandExecutorContext>& CommandExecutor::get_latest_context()
{
    return m_latest_context;
}
std::size_t CommandExecutor::get_command_count() const
{
    return m_commands.size();
}
std::pair<std::set<std::shared_ptr<Command>>::const_iterator,
          std::set<std::shared_ptr<Command>>::const_iterator> CommandExecutor::get_command_iter() const
{
    return std::make_pair(m_commands.cbegin(), m_commands.cend());
}
std::pair<std::map<std::string, std::shared_ptr<Command>>::const_iterator,
          std::map<std::string, std::shared_ptr<Command>>::const_iterator> CommandExecutor::get_alias_iter() const
{
    return std::make_pair(m_aliases.cbegin(), m_aliases.cend());
}
