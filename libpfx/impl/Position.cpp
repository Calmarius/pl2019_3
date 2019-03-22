namespace pfx
{

std::string Position::toString() const
{
    return ssprintf("%s %d:%d", fn, line, column);
}


void Position::raiseErrorHere(std::string errorMsg)
{
    throw error::RuntimeError(*this, std::move(errorMsg));
}


} // namespace pfx
