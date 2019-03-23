namespace pfx
{
NodeRef ArgIterator::fetchNext()
{
    if (current == end)
    {
        return NullNode::instance;
    }
    NodeInfo tmp = *current;
    current++;
    return tmp.node;
}

NodeRef ArgIterator::evaluateNext()
{
    return fetchNext()->evaluate(*this);
}

ArgIterator::IteratorType ArgIterator::dummy;

Position ArgIterator::getPosition()
{
    if (current == end)
    {
        return Position();
    }
    return current->start;
}

NodeRef ArgIterator::next()
{
    if (current == end)
    {
        return NullNode::instance;
    }
    return current->node;
}

} // namespace pfx