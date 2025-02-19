/**
 * command.hpp - An abstract class for commands.
 * Typical command needs a constructor which assigns the protected members of the class,
 * and implements the execute method, which receives the context and the custom data pointer.
 * The arguments for the command are received from the parser:
 *     context->get_parser()
 *
 * which has a bunch of methods for extracting specific type of arguments.
 * Arguments should be extracted sequentally.
 * m_name should always be initialized.
 * 
 * License: The MIT License.
 * Copyright 2025 dernisnw@neonw.su
 * */


#include "command.hpp"
#include "argument_types.hpp"
#include <utility>

using namespace nnwcli;


const std::string& Command::get_name() const
{ 
    return m_name;
}
std::size_t Command::get_args_count() const
{
    return m_args.size();
}
std::size_t Command::get_optargs_count() const
{
    return m_optargs.size();
}
const std::string& Command::get_description() const
{
    return m_description;
}
void Command::set_name(const std::string name)
{
    m_name = name;
}
void Command::set_name(const char* name)
{
    m_name = name;
}
void Command::set_description(const std::string description)
{
    m_description = description;
}
void Command::set_description(const char* description)
{
    m_description = description;
}
const bool Command::operator< (const Command&& other) const
{
    return m_name < other.m_name;
}

std::vector<std::string> Command::tab_complete(
        CommandExecutorContext& context, void* const data) const
{
    return {};
}

std::pair<std::vector<ArgumentDefinition>::const_iterator,
          std::vector<ArgumentDefinition>::const_iterator>
Command::get_arg_iter() const
{
    return std::make_pair(m_args.cbegin(), m_args.cend());
}
std::pair<std::vector<ArgumentDefinition>::const_iterator,
          std::vector<ArgumentDefinition>::const_iterator>
Command::get_optarg_iter() const
{
    return std::make_pair(m_optargs.cbegin(), m_optargs.cend());
}

void Command::format_usage_into(
        std::ostream& stream,
        const std::string alias,
        const std::string command_prefix,
        const std::string arg_before,
        const std::string arg_before_type,
        const std::string arg_after_type,
        const std::string arg_after,
        const std::string optarg_before,
        const std::string optarg_after,
        const std::string description_before,
        const std::string description_after
        ) const
{
    stream << command_prefix << alias << " ";

    if(!m_args.empty())
    {
        for(auto arg_it = m_args.cbegin(); arg_it != m_args.cend(); arg_it++)
        {
            const char* type_name = argtype_to_name(arg_it->m_type);
            
            stream << arg_before << arg_it->m_name << arg_before_type << type_name << arg_after_type << arg_after;
            if(arg_it + 1 != m_args.cend())
                stream << " ";
        }
    }
    if(!m_optargs.empty())
    {
        for(auto optarg_it = m_optargs.cbegin(); optarg_it != m_optargs.cend(); optarg_it++)
        {
            const char* type_name = argtype_to_name(optarg_it->m_type);

            stream << optarg_before << optarg_it->m_name << arg_before_type << type_name << arg_after_type << optarg_after;
            if(optarg_it + 1 != m_optargs.cend())
                stream << " ";
        }
    }
}
