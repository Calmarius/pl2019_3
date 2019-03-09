namespace pfx
{
std::string Position::toString()
{
    return ssprintf("%s %d:%d", fn, line, column);
}

void Position::raiseErrorHere(std::string errorMsg)
{
    throw error::RuntimeError(*this, errorMsg);
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


} // namespace pfx
