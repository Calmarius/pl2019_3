namespace pfx
{
class Node;
class ArgIterator;
class Command;
using CommandRef = std::shared_ptr<Command>;

/// Represents a command to be evaluated in a command node.
struct Command
{
    /**
     * User defined operation to execute when the command node is evaluated.
     *
     * @param [in,out] iterator An iterator that can be used to fetch further
     * nodes during the evaluation.
     *
     * @return The result of the evaluation.
     */
    virtual std::shared_ptr<Node> execute(ArgIterator &iterator) = 0;
    /// Virtual destructor for polymorphism.
    virtual ~Command()
    {
    }
};
} // namespace pfx