namespace pfx
{
std::string Error::toString() const
{
    return ssprintf("%s: %s", position.toString().c_str(), reason.c_str());
}
} // namespace pfx
