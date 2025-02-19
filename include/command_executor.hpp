/**
 * command_executor.hpp - The command manager class, core of the command line interface.
 * Typical usage involves registering custom commands, built-in commands: help, helpof, as well as custom aliases.
 * dispatch_line is used to possibly invoke the command and populate the output into the context.
 * In order to execute commands, it needs an implemented context factory, which
 * is an instance of std::function, returning objects of class extending CommandExecutorContext.
 * Right now, the custom type registry is not implemented and is WIP feature.
 *
 * Method dispatch_line() is thread-safe.
 * 
 * License: The MIT License.
 * Copyright 2025 dernisnw@neonw.su
 * */


#pragma once

#include <exception>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include "command.hpp"
#include "context.hpp"


namespace nnwcli
{
    class command_not_found : public std::exception
    {
        virtual const char* what() const noexcept override
        {
            return "specified command not found";
        }
    };
    class DLL_PUBLIC CommandExecutor
    {
    protected:
        // each command should be unique
        std::set<std::shared_ptr<Command>>      m_commands;

        std::map<std::string,
                 std::shared_ptr<Command>> 
                                                m_aliases;
        std::function<std::shared_ptr<CommandExecutorContext>()>
                                                m_context_factory;
        std::shared_ptr<CommandExecutorContext> m_latest_context;
    public:
        std::mutex m_mutex;

        explicit CommandExecutor() = delete;
        explicit CommandExecutor(CommandExecutor&) = delete;
        explicit CommandExecutor(CommandExecutor&&) = delete;

        CommandExecutor(const std::function<std::shared_ptr<CommandExecutorContext>()>& content_factory);

        const std::function<std::shared_ptr<CommandExecutorContext>()>& get_factory();
        const std::shared_ptr<CommandExecutorContext>& get_latest_context();
        void set_factory(const std::function<std::shared_ptr<CommandExecutorContext>()>& factory);

        bool register_command(const std::string name, std::shared_ptr<Command> command);
        bool register_command(std::shared_ptr<Command> command);
        bool add_alias(const std::string target, const std::string src);
        bool remove_alias(const std::string cmd);
        bool unregister_command(const std::string name, bool delete_aliases = true);

        bool dispatch_line(const std::string line,
                std::shared_ptr<CommandExecutorContext> context_override = nullptr, void* data = nullptr);
        virtual void handle_unknown_command(const std::string cmd, std::shared_ptr<CommandExecutorContext> context);

        std::shared_ptr<Command>& get_command(const std::string name);
        std::size_t get_command_count() const;
        std::pair<std::set<std::shared_ptr<Command>>::const_iterator,
                  std::set<std::shared_ptr<Command>>::const_iterator> get_command_iter() const;
        std::pair<std::map<std::string, std::shared_ptr<Command>>::const_iterator,
                  std::map<std::string, std::shared_ptr<Command>>::const_iterator> get_alias_iter() const;
    };
}
