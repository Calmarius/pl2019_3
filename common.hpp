
namespace pfx
{
struct Position
{
    const char *fn; ///< The file the position is in
    int column;     ///< The column in the file.
    int line;       ///< The line in the file.

    std::string toString();
};

struct Error
{
    Position position;
    const char *reason;

    Error(Position pos, const char *reason) : position(pos), reason(reason) {}
    std::string toString();
    /*Ok,
        FailedToOpenFile,
        InvalidParam,
        ClosingBraceWithoutOpeningOne,
        ClosingBraceExpected,
        RootNodeIsNotGroup,
        NonExecutableNode,
        NotImplented,
        NotACommand,
        InvalidOperation,
        Unexpected*/
};

namespace error
{
#define DECLARE_ERROR(Typename, message)            \
    struct Typename : Error                         \
    {                                               \
        Typename(Position p) : Error(p, message) {} \
    };

DECLARE_ERROR(NonExecutableNode, "This node cannot be executed.")
DECLARE_ERROR(NotImplemented, "This functionality is not implemented for this class.")
DECLARE_ERROR(InvalidOperation, "This operation shouldn't run on this class.")
DECLARE_ERROR(ClosingBraceWithoutOpeningOne, "You have a ')' but not the corresponding (.");
DECLARE_ERROR(ClosingBraceExpected, "For forgot to close a '('.");
DECLARE_ERROR(UndefinedCommand, "This command is undefined.");

#undef DECLARE_ERROR

} // namespace error

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

class ArgIterator
{
    typedef std::vector<std::shared_ptr<Node>>::const_iterator IteratorType;
    IteratorType& current;
    IteratorType end;

  public:
    ArgIterator(IteratorType& current, IteratorType end) : current(current), end(end) {}

    std::shared_ptr<Node> evaluateNext();
    std::shared_ptr<Node> fetchNext();
};

struct Command
{
    virtual std::shared_ptr<Node> execute(ArgIterator &) = 0;
    virtual ~Command() {}
};

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
