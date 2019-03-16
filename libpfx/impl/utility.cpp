namespace pfx
{

std::string ssprintfv(const char *format, va_list args)
{
    va_list args2;
    va_copy(args2, args);

    int n = vsnprintf(nullptr, 0, format, args);
    auto c_str = std::make_unique<char[]>(n + 1);
    vsnprintf(c_str.get(), n + 1, format, args2);
    va_end(args2);

    return std::string(c_str.get());
}

std::string ssprintf(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    std::string ret = ssprintfv(format, args);
    va_end(args);

    return ret;
}

template <class T, class... Args>
std::shared_ptr<T> *createSharedReference(Args &&... args)
{
    return new std::shared_ptr<T>(new T(args...));
}

template <class T> std::shared_ptr<T> *resolveSharedReference(void *ptr)
{
    return static_cast<std::shared_ptr<T> *>(ptr);
}

template <class T> std::shared_ptr<T> *duplicateSharedReference(void *ptr)
{
    return new std::shared_ptr<T>(*resolveSharedReference<T>(ptr));
}

template <class T> T *resolveShared(void *ptr)
{
    return resolveSharedReference<T>(ptr)->get();
}

bool isWhitespace(char c)
{
    return ((9 <= c) && (c <= 13)) || (c == ' ');
}

bool readWord(Input &input, Token &token)
{
    token = Token();
    token.quoted = false;
    // Consume whitespace.
    while (isWhitespace(input.peek()))
    {
        input.get();
    }
    if (input.eof())
    {
        // Reached the end, we are done.
        return false;
    }
    // Read the word
    token.start = input.getPosition();
    if (input.peek() == '"')
    {
        // Quoted string mode.
        while (input.peek() == '"')
        {
            // This loop is here to handle escaped quotes.
            input.get();
            while (input.peek() != '"')
            {
                // Read everything until quote.
                int c = input.get();
                if (c == EOF) break;
                token.word.push_back(c);
            }
            input.get();
            if (input.peek() == '"')
            {
                // It was an escaped "
                token.word.push_back('"');
            }
        }
        token.quoted = true;
    }
    else
    {
        // Non-quoted
        while (!isWhitespace(input.peek()))
        {
            // Read until next whitespace (or end of file.)
            int c = input.get();
            if (c == EOF) break;
            token.word.push_back(c);
        }
    }
    token.end = input.getPosition();

    return true;
}

int stringToInteger(const std::string &str)
{
    return static_cast<int>(strtol(str.c_str(), nullptr, 10));
}

double stringToDouble(const std::string &str)
{
    return strtod(str.c_str(), nullptr);
}

} // namespace pfx
