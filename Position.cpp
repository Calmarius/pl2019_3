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



}