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
     * @return The command for the name. If the command not found, nullptr is
     * returned.
     */
    std::shared_ptr<Command> getCommand(const std::string &name);

    std::shared_ptr<GroupNode> compileCode(Input &input);
};
} // namespace pfx
