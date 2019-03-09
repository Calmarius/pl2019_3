namespace pfx
{

struct Token
{
    Position start = Position();
    Position end = Position();
    std::string word;
    bool quoted;

    Token() {}

    Token(std::string str) : word(str) { quoted = true; }
};
} // namespace pfx
