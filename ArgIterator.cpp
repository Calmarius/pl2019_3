namespace pfx
{
std::shared_ptr<Node> ArgIterator::fetchNext()
{
    if (current == end)
    {
        return NullNode::instance;
    }
    NodeInfo tmp = *current;
    current++;
    return tmp.node;
}

std::shared_ptr<Node> ArgIterator::evaluateNext()
{
    return fetchNext()->evaluate(*this);
}

ArgIterator::IteratorType ArgIterator::dummy;

Position ArgIterator::getPosition() {return current->start;}
std::shared_ptr<Node> ArgIterator::next() {return current->node;}

}