/// @file Token.hpp Contains the Token class.


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

    /**
     *  Initializes it with a word.
     *
     * @param [in] tokenString The string of the token.
     */
    Token(std::string tokenString) : word(std::move(tokenString)), quoted(true)
    {
    }
};
} // namespace pfx
