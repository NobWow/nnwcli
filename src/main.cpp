/**
 * main.cpp - Source file for nnwcli_example executable.
 * Intended for test purposes only, doesn't serve any other purpose than prompting
 * the stdin for input and dispatching the lines as commands.
 * Implements StdoutContext for showing command output straight into the stdout.
 * Also contains an example of how to implement commands, parse arguments and register commands.
 * 
 * License: The MIT License.
 * Copyright 2025 dernisnw@neonw.su
 * */


#include "builtin/help.hpp"
#include "builtin/helpof.hpp"
#include "argument_types.hpp"
#include "command.hpp"
#include "command_executor.hpp"
#include "context.hpp"
#include "parser/abstract_parser.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>


// Context implementation: output everything straight into the stdout.
class StdoutContext : public nnwcli::CommandExecutorContext
{
public:
    virtual void write(const char* data, const std::size_t n) override
    {
        std::fwrite(data, sizeof(char), n, stdout);
    }
    virtual void write(const std::string& data) override
    {
        std::fwrite(data.c_str(), sizeof(char), data.size(), stdout);
    }
    virtual void vnprintf(const char* format, const std::size_t n, va_list args) override
    {
        char nullterm_buffer[n];
        int size = std::vsnprintf(nullterm_buffer, n, format, args);
        std::fwrite(nullterm_buffer, sizeof(char), size, stdout);
    }
    virtual void vnprintf(const std::string& format, const std::size_t n, va_list args) override
    {
        char nullterm_buffer[n];
        std::vsnprintf(nullterm_buffer, n, format.c_str(), args);
        std::fwrite(nullterm_buffer, sizeof(char), n, stdout);
    }
    virtual void flush() override
    {
        std::fflush(stdout);
    }
};

// Test command, receives two integers, prints out the sum
class SumCommand : public nnwcli::Command
{
public:
    SumCommand()
    {
        // define its mandatory arguments
        m_name = "sum";
        m_args = {{nnwcli::CT_INTEGER, "number1", "First number"},
                  {nnwcli::CT_INTEGER, "number2", "Second number"}};
        m_description = "Count the sum of two integers.";
    }
    virtual bool execute(nnwcli::CommandExecutorContext* const context, void* const data) override
    {
        // for receiving command arguments
        const auto parser = context->get_parser();

        // command arguments
        int arg1, arg2;

        // receive arguments
        *parser >> arg1;
        *parser >> arg2;
        parser->parse_finish();

        // print the result
        context->nprintf("Result: %d\n", 65535, arg1 + arg2);
        return true;
    }
};
class EchoCommand : public nnwcli::Command
{
public:
    EchoCommand()
    {
        // define its mandatory arguments
        m_name = "echo";
        m_args = {
            {nnwcli::CT_STRING, "name", "Title name of the message."},
            {nnwcli::CT_STRING, "text", "Message to show to the output."}};
        m_description = "Return the same text into the output.";
    }
    virtual bool execute(nnwcli::CommandExecutorContext* const context, void* const data) override
    {
        // for receiving command arguments
        const auto parser = context->get_parser();

        std::string name;
        std::string text;

        // receive arguments with exhaustion
        parser->parse_string(name);
        parser->parse_string(text);
        parser->parse_finish();

        // print the result
        *context << "Message [" << name << "]: " << text << "\n";
        context->flush();
        return true;
    }
};


int main(int argc, char* argv[])
{

    std::function<std::shared_ptr<StdoutContext>()> factory =
        nnwcli::CommandExecutorContext::create_factory<StdoutContext>();
    nnwcli::CommandExecutor executor(factory);

    executor.register_command(std::make_shared<SumCommand>());
    executor.register_command(std::make_shared<EchoCommand>());
    executor.register_command(std::make_shared<HelpCommand>());
    executor.register_command(std::make_shared<HelpOfCommand>());

    if(!executor.add_alias("msg", "echo"))
    {
        std::cout << "Failed to add an alias for msg." << std::endl;
        return 1;
    }

    std::cout << "This is a test prompt. Using getline, will direct the input into the executor." << std::endl;
    std::string line;
    const std::string quitword = "quit";
    while(line != quitword)
    {
        std::cout << "/";
        std::cout.flush();
        std::getline(std::cin, line);
        if (line == quitword)
            break;
        if (!line.empty())
        {
            executor.dispatch_line(line);
        }
    }
    return 0;
}
