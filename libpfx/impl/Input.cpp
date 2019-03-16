namespace pfx
{
Input::Input(const char *file, int tabSize) : tabSize(tabSize)
{
    inputStream = std::make_unique<std::ifstream>(file, std::ios::binary);
    if (!*inputStream)
    {
        throw error::FailedToOpenFile(
            Position(), ssprintf("%s: %s", file, strerror(errno)));
    }
    fn = file;
}

int Input::get()
{
    int c = inputStream->get();

    switch (c)
    {
    case '\t':
    {
        // Then round it up to the next tab stop.
        column /= tabSize;
        column++;
        column *= tabSize;
    }
    break;
    case '\r':
    {
        crCount++;
        column = 0;
    }
    break;
    case '\n':
    {
        lfCount++;
        column = 0;
    }
    break;
    default:
        column++;
    }

    return c;
}
} // namespace pfx