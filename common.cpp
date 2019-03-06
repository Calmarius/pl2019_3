namespace pfx
{
std::string Position::toString()
{
    return ssprintf("%s %d:%d", fn, line, column);
}

std::string Error::toString()
{
    return ssprintf("%s: %s", position.toString().c_str(), reason.c_str());
}

std::shared_ptr<Node> ArgIterator::fetchNext()
{
    if (current == end)
    {
        return NullNode::instance;
    }
    return *++current;
}

std::shared_ptr<Node> ArgIterator::evaluateNext()
{
    return fetchNext()->evaluate(*this);
}

ArgIterator::IteratorType ArgIterator::dummy;

} // namespace pfx
