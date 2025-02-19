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


#pragma once

#include <string>
#include <vector>
#include "argument.hpp"
#include "context.hpp"
#include "globals.hpp"


namespace nnwcli
{
    class DLL_PUBLIC Command
    {
    protected:
        std::string                     m_name;
        std::vector<ArgumentDefinition> m_args;
        std::vector<ArgumentDefinition> 
                                        m_optargs;
        std::string                     m_description;
    public:
        /**
         * Method is invoked when command is executed in the command line.
         * Returns true when the command is executed without any issues.
         * Returns false if the command couldn't execute for any apparent reason.
         * */
        virtual bool execute(CommandExecutorContext* context,
                void* data) = 0;

        /**
         * Method is not invoked by the library as of now,
         * therefore this method is unused unless an external implementation utilizes it.
         * */
        virtual std::vector<std::string> tab_complete(CommandExecutorContext& context,
                void* data) const;

        const std::string& get_name() const;
        const std::string& get_description() const;
        std::size_t get_args_count() const;
        std::size_t get_optargs_count() const;
        void set_name(const std::string name);
        void set_name(const char* name);
        void set_description(const std::string description);
        void set_description(const char* description);

        std::pair<std::vector<ArgumentDefinition>::const_iterator,
                  std::vector<ArgumentDefinition>::const_iterator>
            get_arg_iter() const;
        std::pair<std::vector<ArgumentDefinition>::const_iterator,
                  std::vector<ArgumentDefinition>::const_iterator>
            get_optarg_iter() const;
        // needed to construct a sorted set of commands
        const bool operator< (const Command&& other) const;

        // utility methods
        void format_usage_into(
                std::ostream& stream,
                const std::string alias,
                const std::string command_prefix = "/",
                const std::string arg_before = "(",
                const std::string arg_before_type = " <",
                const std::string arg_after_type = ">",
                const std::string arg_after = ")",
                const std::string optarg_before = "[",
                const std::string optarg_after = "]",
                const std::string description_before = ": ",
                const std::string description_after = ""
                ) const;
    };
}
