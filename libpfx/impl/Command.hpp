/// @file Command.hpp Contains the Command class.

namespace pfx
{

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
    virtual NodeRef execute(ArgIterator &iterator) = 0;
    /// Virtual destructor for polymorphism.
    virtual ~Command()
    {
    }

    Command()
    {
    }

private:
    // Owned via references, do not copy.
    Command(const Command &) = delete;
    Command &operator=(const Command &) = delete;
};
} // namespace pfx