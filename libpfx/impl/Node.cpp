namespace pfx
{


std::string IntegerNode::toString() const
{
    return ssprintf("%d", value);
}


std::string FloatNode::toString() const
{
    return ssprintf("%g", value);
}


void GroupNode::dump(int indent) const
{
    printf("(\n");
    for (unsigned i = 0; i < nodes.size(); i++)
    {
        const NodeInfo &current = nodes[i];
        dumpIndent(indent + 1);
        current.node->dump(indent + 1);
        printf("\n");
    }
    dumpIndent(indent);
    printf(")");
}


std::string GroupNode::toString() const
{
    std::string str;

    /* Concatenate the string representation of all child nodes without
     * evaluation.*/
    for (auto childNode : nodes)
    {
        str += childNode.node->toString();
    }
    return str;
}


std::shared_ptr<Node> NullNode::instance = std::make_shared<NullNode>();


std::shared_ptr<Node> CommandNode::evaluate(ArgIterator &hIter) const
{
    return command->execute(hIter);
}


std::shared_ptr<Node> GroupNode::evaluate(ArgIterator &) const
{
    std::shared_ptr<Node> resultNode = NullNode::instance;

    /* Evaluate each node, but pass the iterator to the nodes just in case
     they would like to fetch more nodes.*/
    ArgIterator iter(nodes.begin(), nodes.end());
    while (!iter.ended())
    {
        resultNode = iter.evaluateNext();
    }
    return resultNode;
}


std::shared_ptr<GroupNode> GroupNode::evaluateAll() const
{
    auto newGroupNode = std::make_shared<GroupNode>();

    /* Evaluate each node, but pass the iterator to the nodes just in case
     they would like to fetch more nodes.*/
    ArgIterator iter(nodes.begin(), nodes.end());
    while (!iter.ended())
    {
        newGroupNode->nodes.push_back(iter.evaluateNext());
    }
    return newGroupNode;
}

} // namespace pfx