namespace pfx
{
void Context::setCommand(const std::string &name,
                         const std::shared_ptr<Command> &command)
{
    auto iter = commands.find(name);

    if (iter == commands.end())
    {
        // New command
        commands[name] = std::make_shared<CommandNode>(command, name);
    }
    else
    {
        // Existing command overwrite.
        iter->second->prettyName = name;
        iter->second->command = command;
    }
}

struct UndefinedCommand : Command
{
    Position pos;

    UndefinedCommand(Position pos) : pos(pos)
    {
    }

    NodeRef execute(ArgIterator &) override
    {
        throw error::UndefinedCommand(pos);
    }
};


std::shared_ptr<GroupNode> Context::compileCode(Input &input)
{
    std::string word;

    Token token;
    std::stack<std::shared_ptr<GroupNode>> groupStack;

    groupStack.push(std::make_shared<GroupNode>());

    while (readWord(input, token))
    {
        // For each word...
        char *endptr;
        GroupNode *currentGroup = groupStack.top().get();

        if (token.quoted)
        {
            // Quoted strings always create a string node.
            std::shared_ptr<Node> newNode =
                std::make_shared<StringNode>(token.word);
            currentGroup->nodes.push_back(NodeInfo(newNode, token));
            continue;
        }

        int intValue = strtol(token.word.c_str(), &endptr, 10);
        if (*endptr == '\0')
        {
            // The whole word parsed as int.
            std::shared_ptr<Node> newNode =
                std::make_shared<IntegerNode>(intValue);
            currentGroup->nodes.push_back(NodeInfo(newNode, token));
            continue;
        }

        double floatValue = strtod(token.word.c_str(), &endptr);
        if (*endptr == '\0')
        {
            // The whole word parsed as double.
            std::shared_ptr<Node> newNode =
                std::make_shared<FloatNode>(floatValue);
            currentGroup->nodes.push_back(NodeInfo(newNode, token));
            continue;
        }

        if (token.word == "(")
        {
            // New Group
            std::shared_ptr<GroupNode> gn = std::make_shared<GroupNode>();
            currentGroup->nodes.push_back(NodeInfo(gn, token));
            groupStack.push(gn);
            continue;
        }

        if (token.word == ")")
        {
            // Close current group
            groupStack.pop();
            if (groupStack.size() == 0)
            {
                // We popped the root node, it shouldn't happen.
                throw error::ClosingBraceWithoutOpeningOne(token.start);
            }
            groupStack.top()->nodes.back().end = token.end;
            continue;
        }

        // The default case is that the word is a command.
        auto cmd = commands.find(token.word);

        std::shared_ptr<Node> newNode;
        if (cmd == commands.end())
        {
            // Unregistered commands will get the UndefinedCommand handler
            // registered for them.
            std::shared_ptr<CommandNode> tmp = std::make_shared<CommandNode>(
                std::make_shared<UndefinedCommand>(token.start), token.word);
            commands[token.word] = tmp;
            newNode = tmp;
        }
        else
        {
            // For registered commands the registered node is reused.
            newNode = cmd->second;
        }

        currentGroup->nodes.push_back(NodeInfo(newNode, token));
    }
    if (groupStack.size() > 1)
    {
        // At the end only the root node must be on the stack.
        throw error::ClosingBraceExpected(token.start);
    }

    return std::shared_ptr<GroupNode>(groupStack.top());
}


std::shared_ptr<Command> Context::getCommand(const std::string &name)
{
    auto iter = commands.find(name);

    if (iter == commands.end())
    {
        // When not found we return null.
        return std::shared_ptr<Command>();
    }
    return iter->second->command;
}

} // namespace pfx