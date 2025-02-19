/**
 * builtin/help.hpp - Out-of-the-box help command that shows the paginated list of all the commands.
 * /helpof [page <+int>]
 * 
 * License: The MIT License.
 * Copyright 2025 dernisnw@neonw.su
 * */


#pragma once

#include "command.hpp"
#include "command_executor.hpp"
#include <algorithm>


class HelpCommand : public nnwcli::Command
{
    unsigned int m_elements_per_page = 10U;
public:
    HelpCommand()
    {
        m_name = "help";
        m_optargs = {
            {nnwcli::CT_UINTEGER, "page", "Help page to show."}
        };
        m_description = "Show commands, their usage and their description. For showing information about a specific command, use /helpof command.";
    }

    void show_help_entry_into(std::ostream& stream, Command* const command)
    {
        // the usage goes first
        command->format_usage_into(stream, command->get_name());
        stream << ": " << command->get_description() << std::endl;
    }

    virtual bool execute(nnwcli::CommandExecutorContext* const context, void* const data) override
    {
        const auto parser = context->get_parser();
        nnwcli::CommandExecutor* const executor = context->get_executor();
        const std::size_t csize = executor->get_command_count();

        unsigned int page = 0U;
        const unsigned int maxpage = ((csize - 1U) / m_elements_per_page) + 1U;

        parser->parse_unsigned_integer(page, false);
        page = std::clamp(page, 1U, maxpage);
        std::size_t i = (page - 1) * m_elements_per_page;
        const std::size_t end = page * m_elements_per_page;
        std::stringstream ss;
        ss << "--- Help (page " << page << " of " << maxpage << ") ---" << std::endl;
        auto it = executor->get_command_iter();

        // const iterators of set do not support linear advancing
        std::advance(it.first, i);

        for(; it.first != it.second && i < end; i++, it.first++)
        {
            show_help_entry_into(ss, it.first->get());
        }

        if(page < maxpage)
            ss << "--- Next page: /" << context->get_alias() << " " << page + 1 << "---" << std::endl;
        else
            ss << "--- This is the last page ---" << std::endl;

        *context << ss;
        context->flush();

        return true;
    }
};
