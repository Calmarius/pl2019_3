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


void GroupNode::dump(int indent)
{
    printf("(\n");
    for (unsigned i = 0; i < nodes.size(); i++)
    {
        NodeInfo &current = nodes[i];
        dumpIndent(indent + 1);
        current.node->dump(indent + 1);
        // printf(" %s-%s\n", current.start.toString().c_str(),
        // current.end.toString().c_str());
        printf("\n");
    }
    dumpIndent(indent);
    printf(")");
}


std::string GroupNode::toString()
{
    std::string str;
    for (auto childNode : nodes)
    {
        str += childNode.node->toString();
    }
    return str;
}


std::shared_ptr<Node> NullNode::instance = std::make_shared<NullNode>();


std::shared_ptr<Node> CommandNode::evaluate(ArgIterator &hIter)
{
    return command->execute(hIter);
}


std::shared_ptr<Node> GroupNode::evaluate(ArgIterator &)
{
    const auto end = nodes.end();
    std::shared_ptr<Node> resultNode = NullNode::instance;

    for (auto i = nodes.begin(); i != nodes.end();)
    {
        ArgIterator iter(i, end);
        resultNode = (i++)->node->evaluate(iter);
    }
    return resultNode;
}


std::shared_ptr<GroupNode> GroupNode::evaluateAll()
{
    auto newGroupNode = std::make_shared<GroupNode>();

    auto end = nodes.end();
    for (auto i = nodes.begin(); i != end;)
    {
        ArgIterator tmp(i, end);

        newGroupNode->nodes.push_back((i++)->node->evaluate(tmp));
    }
    return newGroupNode;
}

} // namespace pfx