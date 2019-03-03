namespace pfx
{
std::string Position::toString()
{
    return ssprintf("%s %d:%d", fn, line, column);
}

std::string Error::toString()
{
    return ssprintf("%s: %s", position.toString().c_str(), reason);
}

std::shared_ptr<Node> ArgIterator::next()
{
    if (current == end)
    {
        return NullNode::instance;
    }
    return *++current;
}

} // namespace pfx
