namespace pfx
{
std::string Error::toString()
{
    return ssprintf("%s: %s", position.toString().c_str(), reason.c_str());
}
}

