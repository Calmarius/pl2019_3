namespace pfx
{

std::string IntegerNode::toString()
{
    return ssprintf("%d", value);
}

std::string FloatNode::toString()
{
    return ssprintf("%g", value);
}

void GroupNode::dumpPart(int indent)
{
    printf("(\n");
    for (unsigned i = 0; i < nodes.size(); i++)
    {
        nodes[i]->dump(indent + 1);
    }
    dumpIndent(indent);
    printf(")\n");
}

std::string GroupNode::toString()
{
    std::string str;
    for (auto childNode : nodes)
    {
        str += childNode->toString();
    }
    return str;
}

int stringToInteger(const std::string &str)
{
    return static_cast<int>(strtol(str.c_str(), nullptr, 10));
}

double stringToDouble(const std::string &str)
{
    return strtod(str.c_str(), nullptr);
}

std::shared_ptr<Node> NullNode::instance = std::make_shared<NullNode>();

std::shared_ptr<Node> CommandNode::evaluate(ArgIterator &hIter) const
{
    return command->execute(hIter);
}

std::shared_ptr<Node> GroupNode::evaluate(ArgIterator &) const
{
    const auto end = nodes.end();
    std::shared_ptr<Node> resultNode = NullNode::instance;

    for (auto i = nodes.begin(); i != nodes.end();)
    {
        try
        {
            ArgIterator iter(i, end);
            resultNode = (i++)->get()->evaluate(iter);
        }
        catch (Error e)
        {
            throw;
        }
    }
    return resultNode;
}

std::shared_ptr<GroupNode> GroupNode::evaluateAll() const
{
    auto newGroupNode = std::make_shared<GroupNode>();

    auto end = nodes.end();
    for (auto i = nodes.begin(); i != end;)
    {
        ArgIterator tmp(i, end);

        newGroupNode->nodes.push_back((i++)->get()->evaluate(tmp));
    }
    return newGroupNode;
}

} // namespace pfx