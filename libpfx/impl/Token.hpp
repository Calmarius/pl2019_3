namespace pfx
{

/// Represents a single word during parsing.
struct Token
{
    /// The starting character position
    Position start = Position();

    /// The ending character position (one character after the last)
    Position end = Position();

    /// The contained word itself.
    std::string word = std::string();

    /// Indicates the the word came from a quoted string.
    bool quoted = false;

    /// Default constructor creates empty token.
    Token()
    {
    }

    /// Initializes it with a word.
    Token(std::string str) : word(std::move(str)), quoted(true)
    {
    }
};
} // namespace pfx
