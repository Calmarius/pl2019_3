/// @file Context.hpp Defines the Context class.

#pragma once

namespace pfx
{
class CommandNode;

class GroupNode;

/// Defines the context from which the library can be used.
class Context
{
    std::map<std::string, std::shared_ptr<CommandNode>> commands;

public:
    /**
     * Registers a command to be used for command nodes of the given name.
     *
     * @param [in] name The name of the command.
     * @param [in] command The command functor to register.
     *
     * @remarks
     *  If the command is already registered, it will be overwritten with the
     * new command.
     */
    void setCommand(const std::string &name,
                    const std::shared_ptr<Command> &command);

    /**
     * @param [in] name The command name to look for.
     *
     * @return The command for the name. If the command not found, nullptr is
     * returned.
     */
    std::shared_ptr<Command> getCommand(const std::string &name);

    /**
     * Compiles source from the given input source.
     *
     * @param[in,out] input The input the code is read from.
     *
     * @return The group node
     *
     * @throw error::ClosingBraceWithoutOpeningOne On finding a closing brace
     * without the corresponding opening one.
     * @throw error::ClosingBraceExpected When there are unclosed braces at the
     * end of the parsing.
     */
    std::shared_ptr<GroupNode> compileCode(Input &input);
};
} // namespace pfx
