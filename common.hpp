
namespace pfx
{
    enum class Error
    {
        Ok,
        FailedToOpenFile,
        InvalidParam,
        ClosingBraceWithoutOpeningOne,
        ClosingBraceExpected,
        RootNodeIsNotGroup,
        NonExecutableNode,
        NotImplented,
        NotACommand,
        InvalidOperation,
        Unexpected
    };

    enum class NodeType
    {
        Integer,       ///< Integer literal
        FloatingPoint, ///< Floating point literal
        String,        ///< String literal
        Command,       ///< Command
        Group,         ///< Group of nodes
        Null           ///< Unknown node
    };

    struct Node;

    typedef std::vector<std::shared_ptr<Node>>::iterator ArgIterator;


    struct Command
    {
        virtual std::shared_ptr<Node> execute(ArgIterator&) = 0;
        virtual ~Command() = 0;
    };



    struct Position
    {
        const char *fn; ///< The file the position is in
        int column;     ///< The column in the file.
        int line;       ///< The line in the file.
    };

    struct Token
    {
        Position start = Position();
        Position end = Position();
        std::string word;
        bool quoted;

        Token() {}

        Token(std::string str) : word(str) {quoted = true;}
    };
}

