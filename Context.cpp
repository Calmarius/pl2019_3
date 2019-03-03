namespace pfx
{
void Context::registerCommand(const char *name, std::shared_ptr<Command> command)
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

void Context::executeGroup(const GroupNode &groupNode)
{
    auto end = groupNode.nodes.end();
    for (auto i = groupNode.nodes.begin(); i != groupNode.nodes.end(); ++i)
    {
        std::shared_ptr<Node> resultNode;
        try
        {
            ArgIterator iter(i, end);
            std::shared_ptr<Node> resultNode = i->get()->execute(iter);
        }
        catch (Error e)
        {
            throw;
        }
    }
}

std::shared_ptr<GroupNode> Context::evaluateGroup(const GroupNode &groupNode)
{
    auto newGroupNode = std::make_shared<GroupNode>();

    auto end = groupNode.nodes.end();
    for (auto i = groupNode.nodes.begin(); i != end; ++i)
    {
        std::shared_ptr<Node> resultNode;
        try
        {
            ArgIterator tmp(i, end);
            std::shared_ptr<Node> resultNode = i->get()->getValue(tmp);
            newGroupNode->nodes.push_back(resultNode);
        }
        catch (Error e)
        {
            throw;
        }
    }
    return newGroupNode;
}

std::shared_ptr<GroupNode> Context::compileCode(Input &input, Position &errorPosition)
{
    std::string word;

    Token token;
    std::stack<std::shared_ptr<GroupNode>> groupStack;

    groupStack.push(std::make_shared<GroupNode>());

    errorPosition = Position();

    while (readWord(input, token))
    {
        char *endptr;
        GroupNode *currentGroup = groupStack.top().get();

        int intValue = strtol(token.word.c_str(), &endptr, 10);
        if (*endptr == '\0')
        {
            std::shared_ptr<Node> newNode = std::make_shared<IntegerNode>(intValue);
            newNode->setToken(token);
            currentGroup->nodes.push_back(newNode);
            continue;
        }

        double floatValue = strtod(token.word.c_str(), &endptr);
        if (*endptr == '\0')
        {
            std::shared_ptr<Node> newNode = std::make_shared<FloatNode>(floatValue);
            newNode->setToken(token);
            currentGroup->nodes.push_back(newNode);
            continue;
        }

        if (token.word == "(")
        {
            // New Group
            std::shared_ptr<GroupNode> gn = std::make_shared<GroupNode>();
            gn->setToken(token);
            currentGroup->nodes.push_back(std::static_pointer_cast<Node>(gn));
            groupStack.push(gn);
            continue;
        }

        if (token.word == ")")
        {
            // Close current group
            currentGroup->close(token);
            groupStack.pop();
            if (groupStack.size() == 0)
            {
                throw error::ClosingBraceWithoutOpeningOne(token.start);
            }
            continue;
        }

        if (token.quoted)
        {
            std::shared_ptr<Node> newNode = std::make_shared<StringNode>(token.word);
            newNode->setToken(token);
            currentGroup->nodes.push_back(std::static_pointer_cast<Node>(newNode));
        }
        else
        {
            auto cmd = commands.find(token.word);

            std::shared_ptr<Node> newNode;
            if (cmd == commands.end())
            {
                /*std::shared_ptr<CommandNode> cmdNode = std::make_shared<CommandNode>(PFX_CommandClosure{emptyCommand, "", nullptr, nullptr});
                    newNode = cmdNode;
                    commands[token.word] = cmdNode;*/

                printf("%s: %s is a string node.\n", token.start.toString().c_str(), token.word.c_str());
                newNode = std::make_shared<StringNode>(token.word);
            }
            else
            {
                newNode = cmd->second;
            }

            newNode->setToken(token);
            currentGroup->nodes.push_back(std::static_pointer_cast<Node>(newNode));
        }
    }
    if (groupStack.size() > 1)
    {
        throw error::ClosingBraceExpected(token.start);
    }

    //groupStack.top()->dump();

    return std::shared_ptr<GroupNode>(groupStack.top());
}
} // namespace pfx