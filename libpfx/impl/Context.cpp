namespace pfx
{
void Context::setCommand(std::string name, std::shared_ptr<Command> command)
{
    auto iter = commands.find(name);

    if (iter == commands.end())
    {
        commands[name] = std::make_shared<CommandNode>(command, name);
    }
    else
    {
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
        char *endptr;
        GroupNode *currentGroup = groupStack.top().get();

        int intValue = strtol(token.word.c_str(), &endptr, 10);
        if (*endptr == '\0')
        {
            std::shared_ptr<Node> newNode =
                std::make_shared<IntegerNode>(intValue);
            currentGroup->nodes.push_back(NodeInfo(newNode, token));
            continue;
        }

        double floatValue = strtod(token.word.c_str(), &endptr);
        if (*endptr == '\0')
        {
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
                throw error::ClosingBraceWithoutOpeningOne(token.start);
            }
            groupStack.top()->nodes.back().end = token.end;
            continue;
        }

        if (token.quoted)
        {
            std::shared_ptr<Node> newNode =
                std::make_shared<StringNode>(token.word);
            currentGroup->nodes.push_back(NodeInfo(newNode, token));
        }
        else
        {
            auto cmd = commands.find(token.word);

            std::shared_ptr<Node> newNode;
            if (cmd == commands.end())
            {
                std::shared_ptr<CommandNode> tmp =
                    std::make_shared<CommandNode>(
                        std::make_shared<UndefinedCommand>(token.start),
                        token.word);
                commands[token.word] = tmp;
                newNode = tmp;

                /*printf("%s: %s is a string node.\n",
                token.start.toString().c_str(), token.word.c_str()); newNode =
                std::make_shared<StringNode>(token.word);*/
            }
            else
            {
                newNode = cmd->second;
            }

            currentGroup->nodes.push_back(NodeInfo(newNode, token));
        }
    }
    if (groupStack.size() > 1)
    {
        throw error::ClosingBraceExpected(token.start);
    }

    // groupStack.top()->dump();

    return std::shared_ptr<GroupNode>(groupStack.top());
}


std::shared_ptr<Command> Context::getCommand(std::string name)
{
    auto iter = commands.find(name);

    if (iter == commands.end())
    {
        return std::shared_ptr<Command>();
    }
    return iter->second->command;
}

} // namespace pfx