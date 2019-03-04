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

    std::shared_ptr<Node> CommandNode::execute(ArgIterator& hIter)
    {
        return command->execute(hIter);
    }

    std::shared_ptr<Node> CommandNode::getValue(ArgIterator& hIter)
    {
        return execute(hIter);
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

}